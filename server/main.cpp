#include <QCoreApplication>
#include <QList>
#include <QLibrary>
#include "stdio.h"
#include <QTextCodec>
#include <QFile>
#include <QtNetwork/QHostAddress>
#include <QProcess>
#include <QDir>
#include <QPair>

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <QJsonDocument>
#include <QJsonObject>

#include "../global/sv_abstract_device.h"
#include "../global/sql_defs.h"
#include "../global/global_defs.h"
#include "../global/sv_signal.h"
#include "../global/sv_dbus.h"
#include "../global/defs.h"

//#include "../ksuts_devices/oht/sv_oht.h"
//#include "../ksuts_devices/opa/sv_opa.h"
//#include "../ksuts_devices/skm/sv_skm.h"
//#include "../ksuts_devices/sktv/sv_ktv.h"

#include "../../svlib/sv_sqlite.h"
#include "../../svlib/sv_exception.h"
#include "../../svlib/sv_config.h"

#include "sv_storage.h"

SvPGDB* PG = nullptr;

QMap<int, ad::SvAbstractDevice*> DEVICES;
QMap<int, SvStorage*> STORAGES;
QMap<int, SvSignal*> SIGNALS;

QMap<int, sv::SvAbstractLogger*> LOGGERS;

SvException exception;

//sv::SvConcoleLogger dbus;
sv::SvDBus dbus;

const OptionStructList AppOptions = {
    {{OPTION_DEBUG}, "debug","", "", ""},
    {{OPTION_CONFIG_FILE}, "Файл конфигурации сервера","config.json", "", ""},
    {{OPTION_DB_HOST}, "Адрес сервера базы данных.","localhost", "", ""},
    {{OPTION_DB_PORT}, "Порт сервера базы данных.", "5432", "", ""},
    {{OPTION_DB_NAME}, "Имя базы данных для подключения.", "db", "", ""},
    {{OPTION_DB_USER}, "Имя пользователя базы данных.", "postgres", "", ""},
    {{OPTION_DB_PASS}, "Пароль для подключения к базе данных.", "postgres", "", ""},
    {{OPTION_DEVICE_INDEX}, "Индекс устройства'.", "-1", "", ""},
    {{OPTION_LOGGING}, "Включить/выключить логирование.", "off", "", ""},
    {{OPTION_LOG_LEVEL}, "Уровень логирования.", "warning", "", ""},
    {{OPTION_LOG_DEVICE}, "Устройство записи логов.", "file", "", ""},
    {{OPTION_LOG_DIRECTORY}, "Каталог для хранения файлов логов.", "log", "", ""},
    {{OPTION_LOG_FILENAME}, "Шаблон имени файла логов.", "%p_%d%M%y_%h.log", "", ""},
    {{OPTION_LOG_TRUNCATE_ON_ROTATION}, "Переписывать лог файл при ротации.", "on", "", ""},
    {{OPTION_LOG_ROTATION_AGE}, "Максимальное время записи одного файла логов.", "1h", "", ""},
    {{OPTION_LOG_ROTATION_SIZE}, "Максимальный размер одного файла логов.", "10MB", "", ""},
    {{OPTION_LOG_SENDER_NAME_FORMAT}, "Формат имени отправителя для логирования.", "", "", ""}

};

bool parse_params(const QStringList &args, AppConfig& cfg, const QString& file_name);

bool initConfig(const AppConfig &cfg);
void close_db();
bool readDevices(const AppConfig& cfg);
bool readStorages();
//bool readCOBs();
//bool cobToRepository(QString storage_field_name);
bool readSignals();

ad::SvAbstractDevice* create_device(const QJsonObject *o) throw(SvException);
SvStorage* create_storage(QSqlQuery *q);
//SvCOB* create_cob(const QSqlQuery* q);
SvSignal* create_signal(const QSqlQuery* q);

sv::SvAbstractLogger* create_logger(const sv::log::Options& options, const QString& sender);

QJsonObject JSON;

bool openDevices();
bool initStorages();

void signal_handler(int sig);

void closeDevices();
void deinitStorages();
void deleteSignals();


sv::log::sender me = sv::log::sender("main");
sv::log::Level llerr  = sv::log::llError;
sv::log::Level llinf  = sv::log::llInfo;
sv::log::Level lldbg  = sv::log::llDebug;
sv::log::Level lldbg2 = sv::log::llDebug2;
sv::log::Level llall  = sv::log::llAll;
sv::log::MessageTypes mtdbg = sv::log::mtDebug;
sv::log::MessageTypes mterr = sv::log::mtError;
sv::log::MessageTypes mtinf = sv::log::mtInfo;
sv::log::MessageTypes mtscc = sv::log::mtSuccess;
sv::log::MessageTypes mtfal = sv::log::mtFail;


bool parse_params(const QStringList& args, AppConfig &cfg, const QString& file_name)
{
  try {

    /** читаем параметры конфигурации из файла .cfg **/
    SvConfigFileParser cfg_parser(AppOptions);
    if(!cfg_parser.parse(file_name))
        exception.raise(cfg_parser.lastError());

    /** разбираем параметры командной строки **/
    SvCommandLineParser cmd_parser(AppOptions);

    cmd_parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    cmd_parser.setApplicationDescription(QString("\nСервер сбора и обработки данных КСУТС v.%1").arg(APP_VERSION));
    cmd_parser.addHelpOption();
    cmd_parser.addVersionOption();

    if (!cmd_parser.parse(args))
      exception.raise(-1, QString("%1\n\n%2").arg(cmd_parser.errorText()).arg(cmd_parser.helpText()));

    if (cmd_parser.isSetVersionOption())
      exception.raise(-1, QString("Сервер сбора и обработки данных КСУТС v.%1").arg(APP_VERSION));

    if (cmd_parser.isSetHelpOption())
      exception.raise(-1, cmd_parser.helpText());

    /** назначаем значения параметров */
    bool ok;
    QString val;

    // config_file_name
    cfg.config_file_name = cmd_parser.isSet(OPTION_CONFIG_FILE) ? cmd_parser.value(OPTION_CONFIG_FILE) :
                                                     cfg_parser.value(OPTION_CONFIG_FILE);

    // db_host
    cfg.db_host = cmd_parser.isSet(OPTION_DB_HOST) ? cmd_parser.value(OPTION_DB_HOST) :
                                                     cfg_parser.value(OPTION_DB_HOST);
    if ((cfg.db_host != "localhost") && QHostAddress(cfg.db_host).isNull())
      exception.raise(-1, QString("Неверный адрес сервера баз данных: %1").arg(cfg.db_host));

    // db_port
    val = cmd_parser.isSet(OPTION_DB_PORT) ? cmd_parser.value(OPTION_DB_PORT) :
                                             cfg_parser.value(OPTION_DB_PORT);
    cfg.db_port = val.toUInt(&ok);
    if(!ok) exception.raise(-1, QString("Неверный порт: %1").arg(val));

    // soeg_port
//    val = cmd_parser.isSet(OPTION_SOEG_PORT) ? cmd_parser.value(OPTION_SOEG_PORT) :
//                                               cfg_parser.value(OPTION_SOEG_PORT);
//    cfg.soeg_port = val.toUInt(&ok);
//    if(!ok) exception.raise(-1, QString("Неверный порт СОЭЖ: %1").arg(val));

    // db_name
    cfg.db_name = cmd_parser.isSet(OPTION_DB_NAME) ? cmd_parser.value(OPTION_DB_NAME) :
                                                     cfg_parser.value(OPTION_DB_NAME);

    // db_user
    cfg.db_user = cmd_parser.isSet(OPTION_DB_USER) ? cmd_parser.value(OPTION_DB_USER) :
                                                     cfg_parser.value(OPTION_DB_USER);

    // db_pass
    cfg.db_pass = cmd_parser.isSet(OPTION_DB_PASS) ? cmd_parser.value(OPTION_DB_PASS) :
                                                     cfg_parser.value(OPTION_DB_PASS);

//    // single device mode
//    val = cmd_parser.isSet(OPTION_SINGLE_DEVICE_MODE) ? cmd_parser.value(OPTION_SINGLE_DEVICE_MODE) :
//                                                        cfg_parser.value(OPTION_SINGLE_DEVICE_MODE);
//    cfg.single_device_mode = sv::log::stringToBool(val);

    // device index
    // !!! ЭТОТ ПАРАМЕТР МОЖЕТ БЫТЬ ЗАДАН ТОЛЬКО В КОМАНДНОЙ СТРОКЕ
    val = cmd_parser.isSet(OPTION_DEVICE_INDEX) ? cmd_parser.value(OPTION_DEVICE_INDEX) : "-1";

    cfg.device_index = val.toInt(&ok);
    if(!ok) exception.raise(-1, QString("Неверный индекс устройства: %1").arg(val));


    // logging
    val = cmd_parser.isSet(OPTION_LOGGING) ? cmd_parser.value(OPTION_LOGGING) :
                                             cfg_parser.value(OPTION_LOGGING);
    cfg.log_options.logging = sv::log::stringToBool(val);

    // log_level
    val = cmd_parser.isSet(OPTION_LOG_LEVEL) ? cmd_parser.value(OPTION_LOG_LEVEL) :
                                               cfg_parser.value(OPTION_LOG_LEVEL);
    cfg.log_options.log_level = sv::log::stringToLevel(val, &ok);
    if(!ok) exception.raise(-1, QString("Неверный уровень логирования: %1").arg(val));

    // log_device
    val = cmd_parser.isSet(OPTION_LOG_DEVICE) ? cmd_parser.value(OPTION_LOG_DEVICE) :
                                                cfg_parser.value(OPTION_LOG_DEVICE);
    QStringList vals = val.split(',');
    cfg.log_options.log_devices.clear(); // обязательно
    for (int i = 0; i < vals.count(); ++i) {

        cfg.log_options.log_devices.append(sv::log::stringToDevice(vals.at(i), &ok));
        if(!ok) exception.raise(-1, QString("Неверное устройство логирования: %1").arg(val));
    }


    // log_directory
    cfg.log_options.log_directory = cmd_parser.isSet(OPTION_LOG_DIRECTORY) ? cmd_parser.value(OPTION_LOG_DIRECTORY) :
                                                                 cfg_parser.value(OPTION_LOG_DIRECTORY);

    // log_filename
    cfg.log_options.log_filename = cmd_parser.isSet(OPTION_LOG_FILENAME) ? cmd_parser.value(OPTION_LOG_FILENAME) :
                                                               cfg_parser.value(OPTION_LOG_FILENAME);

    // log_truncate_on_rotation
    val = cmd_parser.isSet(OPTION_LOG_TRUNCATE_ON_ROTATION) ? cmd_parser.value(OPTION_LOG_TRUNCATE_ON_ROTATION) :
                                                              cfg_parser.value(OPTION_LOG_TRUNCATE_ON_ROTATION);
    cfg.log_options.log_truncate_on_rotation = sv::log::stringToBool(val);

    // log_rotation_age
    val = cmd_parser.isSet(OPTION_LOG_ROTATION_AGE) ? cmd_parser.value(OPTION_LOG_ROTATION_AGE) :
                                                      cfg_parser.value(OPTION_LOG_ROTATION_AGE);
    cfg.log_options.log_rotation_age = sv::log::stringToSeconds(val, &ok);
    if(!ok) exception.raise(-1, QString("Неверный формат времени: %1").arg(val));


    // log_rotation_size
    val = cmd_parser.isSet(OPTION_LOG_ROTATION_SIZE) ? cmd_parser.value(OPTION_LOG_ROTATION_SIZE) :
                                                       cfg_parser.value(OPTION_LOG_ROTATION_SIZE);
    cfg.log_options.log_rotation_size = sv::log::stringToSize(val, &ok);
    if(!ok) exception.raise(-1, QString("Неверный формат размера файла: %1").arg(val));

    // шаблон имени отправителя по dbus
    cfg.log_options.log_sender_name_format = cmd_parser.isSet(OPTION_LOG_SENDER_NAME_FORMAT) ? cmd_parser.value(OPTION_LOG_SENDER_NAME_FORMAT) :
                                                                                               cfg_parser.value(OPTION_LOG_SENDER_NAME_FORMAT);
//    QString(DBUS_SENDER_NAME);

    return true;

  }

  catch(SvException &e) {
    dbus << llerr << me << mterr << QString("%1\n").arg(e.error) << sv::log::endl;
//    dbus << llerr << QString("%1\n").arg(e.error) << clog::endl;
    return false;
  }
}

int main(int argc, char *argv[])
{
  // запрос версии для монитора
  if((argc > 1) && (QString(argv[1]).trimmed() == "-v")) {
    std::cout << QString(APP_VERSION).toStdString().c_str() << std::endl;
    return 0;
  }

  // запуск в режиме отладки с выводом в терминал
  bool debug = (argc > 1) && (QString(argv[1]).trimmed() == "-debug");

#ifdef Q_OS_WIN32
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
#else // Q_OS_LINUX
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif


  QCoreApplication a(argc, argv);

  if(!debug)
  {
    // создаем потомка
    switch (fork()) {

      case -1:   // если не удалось запустить потомка выведем на экран ошибку и её описание

        printf("Ошибка при запуске службы сервера (%s)\n", strerror(errno));
        return -1;
        break;

      case 0:
        break;

      default:

        return 0;
        break;

    }
  }

  // инициализируем dbus ПОСЛЕ запуска потомка
  dbus.init();

  AppConfig cfg;

  try {

    QString cfg_file_name = QString("%1%2%3.cfg")
            .arg(QCoreApplication::applicationDirPath())
            .arg(QDir::separator())
            .arg(QCoreApplication::applicationName());

    // проверяем наличие файла cfg. если отсутствует - пытаемся создать его
    QFile f(cfg_file_name);
    if(!f.exists()) {
      if(!f.open(QIODevice::ReadWrite))
        exception.raise(-1, QString("Файл %1 отсутствует и не удается создать его").arg(cfg_file_name));

      f.close();
    }

    if(!parse_params(a.arguments(), cfg, cfg_file_name))
        exception.raise(-1, "Ошибка разбора командной строки");

    dbus.setOptions(cfg.log_options);
//    dbus.setSender("main");

    dbus << lldbg2 << me << mtdbg
         << "cfg_file_name=" << cfg.config_file_name
         << "\ndb_host=" << cfg.db_host
         << "\ndb_port=" << cfg.db_port
         << "\ndb_name=" << cfg.db_name << "\ndb_user=" << cfg.db_user << "\ndb_pass=" << cfg.db_pass
         << "\nlogging=" << (cfg.log_options.logging ? "on" : "off")
         << "\nlog_level=" << sv::log::levelToString(cfg.log_options.log_level)
         << "\nlog_devices=" << sv::log::deviceListToString(cfg.log_options.log_devices)
         << "\nlog_directory=" << cfg.log_options.log_directory
         << "\nlog_filename=" << cfg.log_options.log_filename
         << "\nlog_truncate_on_rotation=" << (cfg.log_options.log_truncate_on_rotation ? "on" : "off")
         << "\nlog_rotation_age=" << cfg.log_options.log_rotation_age << " (seconds)"
         << "\nlog_rotation_size=" << cfg.log_options.log_rotation_size << " (bytes)"
         << "\ndevice_index=" << cfg.device_index << "\n"
         << "log_sender_name_format" << cfg.log_options.log_sender_name_format << "\n"
         << sv::log::endl;

  }

  catch(SvException &e) {
    dbus << llerr << me << mterr << QString("%1\n").arg(e.error) << sv::log::endl;
    return e.code;
  }


  /// перехватываем момент закрытия программы, чтобы корректно завершить
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  dbus << llinf << mtinf << me
       << QString("Сервер КСУТС v.%1\n").arg(APP_VERSION)
       << sv::log::endl;

  int result = 0;


  try {

    /** открываем БД, читаем конфигурацию **/
    if(!initConfig(cfg)) exception.raise(-10);

//    /** читаем устройства, репозитории и сигналы. СИГНАЛЫ В ПОСЛЕДНЮЮ ОЧЕРЕДЬ! **/
    if(!readDevices(cfg)) exception.raise(-20);
//    if(!readStorages()) exception.raise(-30);

//    if(!readSignals()) exception.raise(-40);

//    close_db();

//    /** подключаемся к устройствам и к репозиториям и начинаем работу **/
//    if(!openDevices()) exception.raise(-50);

//    /** подключаемся к серверам баз данных - хранилищам **/
//    initStorages();

  }
  
  catch(SvException& e) {
    result = e.code;
  }
  
  if(result == 0) {

      setsid(); // отцепляемся от родителя

      if(!debug)
      {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
      }

      return a.exec();

  }
  else {

    return result;
  }

}

void signal_handler(int sig)
{
  Q_UNUSED(sig);


//  qDebug() << "close_db()";
//  close_db();

//  qDebug() << "closeDevices()";
//  closeDevices();

//  qDebug() << "deinitStorages()";
//  deinitStorages();

//  qDebug() << "deleteSignals()";
//  deleteSignals();

//  /* логеры в последнюю очередь */
//  foreach (sv::SvAbstractLogger* logger, LOGGERS)
//    delete logger;

//  qDebug() << "_Exit(0)";
  _Exit(0);

}

void close_db()
{
  if(PG) {
    QString db_connect_name = PG->db.connectionName();
    delete PG; // хз как это работает. но работает
    QSqlDatabase::removeDatabase(db_connect_name);
  }

  PG = nullptr;

}

bool initConfig(const AppConfig& cfg)
{
  
  try {
      
    dbus << llinf << mtinf << me << QString("Читаем файл конфигурации %1: ").arg(cfg.config_file_name) << sv::log::endl;

    QFile json_file(cfg.config_file_name);
    if(!json_file.open(QIODevice::ReadWrite))
      exception.raise(json_file.errorString());

    /* загружаем json конфигурацию в QJSonDocument */
    QJsonParseError parse_error;
    QJsonDocument jdoc = QJsonDocument::fromJson(json_file.readAll(), &parse_error);

    if(parse_error.error != QJsonParseError::NoError)
      exception.raise(parse_error.errorString());

    JSON = jdoc.object();


//    PG = new SvPGDB();

////    lout << dbname << dbhost << dbport << dbuser << dbpass << sv::log::endl;
//    PG->setConnectionParams(cfg.db_name, cfg.db_host, cfg.db_port, cfg.db_user, cfg.db_pass);

//    QSqlError serr = PG->connectToDB();
//    if(serr.type() != QSqlError::NoError)
//      exception.raise(serr.databaseText());


//    // проверяем соответствие версии БД версии сервера
//    QSqlQuery q(PG->db);
//    serr = PG->execSQL(SQL_SELECT_DB_INFO, &q);
//    if(serr.type() != QSqlError::NoError)
//      exception.raise(serr.text());

//    if(!q.next())
//      exception.raise(1, "Нет информации о версии.\n");

//    QString db_version = q.value("db_version").toString();

    if(JSON.contains("version"))
      dbus << llinf << mtinf << me << QString("Версия %1").arg(JSON.value("version").toString()) << sv::log::endl;
    
//    if(QString(ACTUAL_DB_VERSION) != db_version)
//      exception.raise(1, QString("Версия БД не соответствует версии программы. Требуется версия БД %1\n")
//                      .arg(ACTUAL_DB_VERSION));

    dbus << llinf << mtinf << me << mtscc << "OK\n" << sv::log::endl;

    return true;
    
  }
  
  catch(SvException& e) {
    
    dbus << llerr << mterr << me << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    return false;
  }
}


bool readDevices(const AppConfig& cfg)
{

  dbus << llinf << mtinf << me
       << QString("Читаем данные устройств: ") << sv::log::endl;
  
//  QSqlQuery q(PG->db);
  
  try {

//    QSqlError serr;

////    if(cfg.single_device_mode)
////      serr = PG->execSQL(QString(SQL_SELECT_SINGLE_INVOLVED_DEVICE).arg(cfg.single_device_index), &q);

////    else
//    serr = PG->execSQL(SQL_SELECT_INVOLVED_DEVICES, &q);

//    if(serr.type() != QSqlError::NoError) exception.raise(serr.text());
    
    int counter = 0;
//    while(q.next()) {

    if(!JSON.contains("devices"))
      exception.raise("Неверный файл конфигурации. Отсутствует раздел 'devices'.");

    QJsonArray device_list = JSON.value("devices").toArray();

    for(QJsonValue v: device_list) {
       
      QJsonObject o = v.toObject();

      ad::SvAbstractDevice* newdev = create_device(&o);

      if(newdev) {

//        DEVICES.insert(newdev->info()->index, newdev);

//        if(cfg.log_options.logging)
//        {
//          LOGGERS.insert(newdev->info()->index, new sv::SvDBus(cfg.log_options));

//          newdev->setLogger(LOGGERS.value(newdev->info()->index));
//        }

        dbus << lldbg << mtdbg << me
             << QString("  %1 [Индекс %2]\n Параметры: %3\n  Интерфейс: %4 %5").
                arg(newdev->info()->name).
                arg(newdev->info()->id).
                arg(newdev->info()->device_params).
                arg(newdev->info()->ifc_name).arg(newdev->info()->ifc_params)
             << sv::log::endl;

        counter++;

      }

//      else {

//        dbus << llerr << me << mterr
//             << QString("Не удалось добавить устройство '%1' [Индекс %2]\n")
//                                        .arg(q.value(1).toString())
//                                        .arg(q.value(0).toInt())
//             << sv::log::endl;

////        exception.raise(QString("Не удалось добавить устройство %1 (id %2)")
////                        .arg(q->value("device_name").toString())
////                        .arg(q->value("device_index").toInt()));
//      }
    }

//    q.finish();

//    if(counter == 0)
//      exception.raise("Устройства в конфигурации не найдены");

    dbus << llinf << me << mtscc
         << QString("OK [прочитано %1]\n").arg(counter) << sv::log::endl;
    
    return true;
    
  }
  
  catch(SvException& e) {

    dbus << llerr << me << mterr
         << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    return false;
    
  }
}

bool readStorages()
{
  dbus << llinf << me << mtinf << QString("Читаем данные хранилищ:") << sv::log::endl;
  
  QSqlQuery* q = new QSqlQuery(PG->db);
  
  try {
    
    QSqlError serr = PG->execSQL(SQL_SELECT_STORAGES_LIST, q);
    
    if(serr.type() != QSqlError::NoError) exception.raise(serr.text());
    
    int counter = 0;
    while(q->next()) { 
      
      SvStorage* newstorage = create_storage(q);
      
      if(newstorage) {
        
        STORAGES.insert(newstorage->index(), newstorage);

        dbus << lldbg << me << mtdbg << QString("  %1 [Индекс %2] %3:%4").arg(newstorage->params()->name).arg(newstorage->params()->index).arg(newstorage->params()->host).arg(newstorage->params()->port) << sv::log::endl;
        
        counter++;
      }
      else
         dbus << llerr << mterr << me
              << QString("Не удалось добавить хранилище %1 [Индекс %2]\n")
                        .arg(q->value("storage_name").toString())
                        .arg(q->value("storage_index").toInt())
              << sv::log::endl;
        
    }
    q->finish();
    delete q;
    
    dbus << llinf << me << mtscc << QString("OK [Прочитано %1]\n").arg(counter) << sv::log::endl;
    
    return true;
    
  }
  
  catch(SvException& e) {
    
    delete q;
    dbus << llerr<< me << mterr  << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    return false;
    
  }
}

bool readSignals()
{
  dbus << llinf << me << mtinf << QString("Читаем данные сигналов:") << sv::log::endl;
  
  QSqlQuery* q = new QSqlQuery(PG->db);
  
  try {
                                  
    QSqlError serr = PG->execSQL(QString(SQL_SELECT_INVOLVED_SIGNALS), q);
    
    if(serr.type() != QSqlError::NoError) exception.raise(serr.text());
    
    int counter = 0;
    while(q->next()) { 

      SvSignal* newsig = nullptr;

//      if(!cfg.single_device_mode) {

        newsig = create_signal(q);
//      }
//      else
//      {

//        // в режиме 'одной устройство' создаем сигнал, только если его устройство равно устройству при запуске
//        if(q->value("signal_device_index").toInt() == cfg.single_device_index)
//          newsig = create_signal(q);

//      }

      
      if(newsig) {

        SIGNALS.insert(newsig->index(), newsig);
        
        dbus /*<< llall << me << mtdbg*/ << QString("  %1 [Индекс %2]").arg(newsig->params()->name).arg(newsig->index());

        // раскидываем сигналы по устройствам
        if(DEVICES.contains(newsig->params()->device_index)) {


          DEVICES.value(newsig->params()->device_index)->addSignal(newsig);

          dbus /*<< llall << me << mtdbg*/ << QString("%1 %2").arg(QString(31 - newsig->params()->name.length(), QChar('-'))).arg(DEVICES.value(newsig->params()->device_index)->info()->name);

          counter++;

          // раскидываем сигналы по хранилищам
          if(STORAGES.contains(0) && newsig->params()->storage0_linked) {

            STORAGES.value(0)->addSignal(newsig);

            dbus /*<< llall << me << mtdbg*/ << QString("%1 %2").arg(QString(6, QChar('-'))).arg(STORAGES.value(0)->params()->name);

          }

          if(STORAGES.contains(1) && newsig->params()->storage1_linked) {

            STORAGES.value(1)->addSignal(newsig);

            dbus /*<< llall << me << mtdbg*/ << QString("%1 %2").arg(QString(6, QChar('-'))).arg(STORAGES.value(1)->params()->name);

          }

          if(STORAGES.contains(2) && newsig->params()->storage2_linked) {

            STORAGES.value(2)->addSignal(newsig);

            dbus /*<< llall << me << mtdbg*/ << QString("%1 %2").arg(QString(6, QChar('-'))).arg(STORAGES.value(2)->params()->name);

          }
        }

        dbus << llall << me << mtdbg << sv::log::endl;

      }
      
      else {

//        if(!cfg.single_device_mode)
//          exception.raise(QString("Не удалось добавить сигнал %1 [Индекс %2]")
//                        .arg(q->value("signal_name").toString())
//                        .arg(q->value("signal_index").toInt()));
      }
    }

    q->finish();
    delete q;
    
    dbus << llinf << me << mtscc << QString("OK [Прочитано %1]\n").arg(counter)  << sv::log::endl;
    
    return true;
    
  }
  
  catch(SvException& e) {
    
    q->finish();
    delete q;
    dbus << llerr << me << mterr << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    return false;
    
  }
  
}

ad::SvAbstractDevice* create_device(const QJsonObject* o) throw(SvException)
{  
  ad::SvAbstractDevice* newdev = nullptr;
  
  try {
    // проверяем наличие основных полей
    QStringList l = QStringList() << P_ID << P_NAME << P_DRIVER << P_DEV_PARAMS << P_IFC << P_IFC_PARAMS << P_DESCRIPTION;
    for(QString v: l)
      if(o->value(v).isUndefined())
        throw exception.assign(QString("В разделе 'devices' отсутствует или не задан параметр '%1'").arg(v));

    ad::DeviceInfo info;

    info.id = o->value(P_ID).toInt(-1);
    if(info.id == -1)
      throw exception.assign(QString(L_WRONG_PARAM).arg(P_ID).arg(o->value(P_ID).toVariant().toString()));

    info.name = o->value(P_NAME).toString();

//    info.hardware_type = dev::HARDWARE_CODES.value(o->value("device_hardware_code").toString());
  //  info.ifc_type = dev::IFC_CODES.value(o->value("device_ifc_name").toString());
    info.ifc_name = o->value(P_IFC).toString();
    info.ifc_params = o->value(P_IFC_PARAMS).toString(); // QString(QJsonDocument(o->value(P_IFC).toObject()).toJson(QJsonDocument::Compact));
    info.device_params = o->value(P_DEV_PARAMS).toString(); // QString(QJsonDocument(o->value(P_PARAMS).toObject()).toJson(QJsonDocument::Compact));
    info.driver_lib_name = o->value(P_DRIVER).toString();
    info.is_involved = o->value(P_ACTIVE).toBool(true);
    info.debug_mode = o->value(P_DEBUG).toBool(false);
    info.timeout = o->value(P_TIMEOUT).toInt(0);
  //  info.re= q->value("device_timeout").toUInt();
  //  QString params = q->value("device_params").toString();
  qDebug() << info.driver_lib_name;
    QLibrary devlib(info.driver_lib_name);
qDebug() << 2;
    if(!devlib.load())
      throw exception.assign(devlib.errorString());
qDebug() << 3;

//    switch (info.hardware_type) {
      
//      case dev::OHT:
////        newdev = new SvOHT();
//        break;
        
//      case dev::OPA:
////        newdev = new SvOPA();
//        break;
        
//      case dev::SKM:
////        newdev = new SvSKM();
//        break;

//    case dev::KTV:
////        newdev = new SvKTV();
//        break;

//      default:
//        exception.raise(QString("Неизвестное устройство: %1 [Индекс %2]").arg(info.name).arg(info.index));
//        break;
        
//    }

    if(!newdev)
      exception.raise("Неизвестная ошибка при создании устройства");
    
    if(!newdev->setup(info))
      exception.raise(newdev->lastError());
//    if(!newdev->setParams(params)) exception.raise(newdev->lastError());

    return newdev;
    
  }
  
  catch(SvException& e) {
    
//    dbus << llerr << me << mterr << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    if(newdev)
      delete newdev;

    newdev = nullptr;

    throw e;
    
  }
}

SvStorage* create_storage(QSqlQuery* q)
{
  
  SvStorage* newstorage = 0;
  
  StorageParams params;
  
  params.index = q->value("storage_index").toInt();
  params.name = q->value("storage_name").toString();
  params.host = q->value("storage_host").toString();
  params.port = q->value("storage_port").toUInt();
  params.login = q->value("storage_login").toString();
  params.pass = q->value("storage_pass").toString();
  params.database_name = q->value("storage_dbname").toString();
  params.table_name = q->value("storage_table_name").toString();
  
  try {
    
    newstorage = new SvStorage(params, dbus);
    
    return newstorage;
    
  }
  
  catch(SvException& e) {
    
    if(newstorage)
      delete newstorage;
    
    dbus << llerr << me << mterr << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    
    return Q_NULLPTR;
    
  }
}

SvSignal* create_signal(const QSqlQuery* q)
{
  SignalParams params;

  params.index = q->value("signal_index").toInt();
//  params.cob_id = q->value("signal_cob_id").toInt();
  params.device_index = q->value("signal_device_index").toInt();
  params.name = q->value("signal_name").toString();
  params.timeout = q->value("signal_timeout").toInt();
  params.timeout_value = q->value("signal_timeout_value").toInt();
  params.timeout_signal_index = q->value("signal_timeout_index").toInt();
  params.data_offset = q->value("signal_data_offset").toInt();
  params.data_length = q->value("signal_data_length").toInt();
  params.data_type = SignalDataTypes(q->value("signal_data_type").toInt());
  params.description = q->value("signal_description").toString();
//  params.is_involved = q->value("signal_is_involved").toBool();
  params.storage0_linked = q->value("signal_storage0_linked").toBool();
  params.storage1_linked = q->value("signal_storage1_linked").toBool();
  params.storage2_linked = q->value("signal_storage2_linked").toBool();

    
  return new SvSignal(params);;
    
}

bool openDevices()
{
  dbus << llinf << me << mtinf << "Открываем устройства:" << sv::log::endl;
 
  try {
    
    for(int key: DEVICES.keys()) {
      
      ad::SvAbstractDevice* device = DEVICES.value(key);

      if(!device->open()) exception.raise(QString("%1 [Индекс %2]: %3")
                                          .arg(device->info()->name)
                                          .arg(device->info()->id)
                                          .arg(device->lastError()));

      dbus << lldbg << me << mtdbg<< QString("  %1: OK").arg(device->info()->name) << sv::log::endl;
        
    }
    
    dbus << llinf << me << mtscc << QString("OK\n") << sv::log::endl;
    
    return true;
    
  }
  
  catch(SvException& e) {
    
    dbus << llerr << me << mterr << QString("Ошибка: %1\n").arg(e.error) << sv::log::endl;
    return false;
    
  }
  
}

bool initStorages()
{
   dbus << llinf << me << mtinf << "Инициализируем хранилища:" <<  sv::log::endl;
  
   try {
     
     foreach(SvStorage* storage, STORAGES.values()) {

       if(storage->signalsCount()) {

         if(!storage->init())
           exception.raise(QString("%1: %2").arg(storage->params()->name)
                                            .arg(storage->lastError()));

         storage->start();

         dbus << lldbg << me << mtdbg
              << QString("  %1 (%2:%3): OK")
                   .arg(storage->params()->name)
                   .arg(storage->params()->host)
                   .arg(storage->params()->port)
                << sv::log::endl;

       }
     }
       
         
     dbus << llinf << me << mtscc << QString("OK\n") << sv::log::endl;

     return true;
     
   }
   
   catch(SvException& e) {
     
     dbus << llerr << me << mterr << QString("Ошибка: %1").arg(e.error) << sv::log::endl;
     return false;
     
   }
  
}


void closeDevices()
{
  dbus << llinf << me << mtinf << "Закрываем устройства:" << sv::log::endl;

  try {

    int counter = 0;

    foreach (int key, DEVICES.keys())
    {
      ad::SvAbstractDevice* device = DEVICES.value(key);

      dbus << llinf << me << mtinf << QString("  %1 (%2):").arg(device->info()->name).arg(device->info()->ifc_name) << sv::log::endl;

//      device->close();
      delete DEVICES.take(key);

      counter++;

    }

    dbus << llinf << me << mtinf << QString("OK [Закрыто %1]\n").arg(counter)  << sv::log::endl;

  }

  catch(SvException& e) {

    dbus << llerr << me << mterr << QString("Ошибка: %1").arg(e.error) << sv::log::endl;

  }

}

void deinitStorages()
{

  dbus << llinf << me << mtinf << "Закрываем хранилища:" << sv::log::endl;

  int counter = 0;
  foreach (int key, STORAGES.keys()) {

    SvStorage* storage = STORAGES.value(key);

//    if(detiled)
//      lout << QString("  %1\t%2:%3:").arg(storage->params()->name).arg(storage->params()->host).arg(storage->params()->port) << sv::log::endi;

    storage->stop();
    delete STORAGES.take(key);

//    lout << llinf << "\tOK" << sv::log::endl;

    counter++;

  }

//  lout << llinf << QString("OK\n") << sv::log::endl;
  dbus << llinf << me << mtinf << QString("OK [Закрыто %1]\n").arg(counter)  << sv::log::endl;

}

void deleteSignals()
{
  dbus << llinf << me << mtinf << "Удаляем сигналы:" << sv::log::endl;

  int counter = 0;
  foreach (int key, SIGNALS.keys()) {

//    SvSignal* signal = SIGNALS.value(key);

//    if(detiled)
//      lout << QString("  %1 [index %2]:").arg(signal->params()->name).arg(signal->params()->index) << sv::log::endi;

    delete SIGNALS.take(key);

    counter++;
//    lout << llinf << "\tOK" << sv::log::endl;

  }

  dbus << llinf << me << mtinf << QString("OK [Удалено %1]\n").arg(counter)  << sv::log::endl;

}

