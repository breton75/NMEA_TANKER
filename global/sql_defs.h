#ifndef DEFS_H
#define DEFS_H

#define CR "\n"

#define MAIN_TREE_HEADERS "| ;<Имя" //"<Система/Сигнал;|Индекс;|Интерфейс;<Параметры;|%1;|%2;|%3;<Описание"
#define DEVICES_TREE_HEADERS "<Система;|Индекс;|Интерфейс;|<Параметры;<Описание"
#define SIGNALS_TREE_HEADERS "<Сигнал;|Индекс;|Тип;|Размер;|Смещение;<Описание"
#define STORAGES_TREE_HEADERS "|Хранилище;|Сервер;|Порт;|Логин;|База данных;|Таблица"


/** *********** POSTGRES *************** **/

/** определения для версии 2 **/
#ifdef VERSION_2 // в файле проекта

#define SQL_SELECT_FROM_DEVICES \
  "SELECT " CR \
  "   devices.device_index as device_index, " CR \
  "   devices.device_name as device_name, " CR \
  "   devices.device_params as device_params, " CR \
  "   devices.hardware_code as device_hardware_code, " CR \
  "   devices.ifc_name as device_ifc_name, " CR \
  "   devices.ifc_params as device_ifc_params, " CR \
  "   devices.protocol_id as device_protocol_id, " CR \
  "   devices.description as device_description, " CR \
  "   devices.driver_lib_name as device_driver_lib_name, " CR \
  "   devices.is_involved as device_is_involved, " CR \
  "   devices.debug as device_debug, " CR \
  "   devices.timeout as device_timeout, " CR \
  "   protocols.protocol_name as device_protocol_name " CR \
  "FROM devices " CR \
  "LEFT JOIN protocols  ON protocols.id = devices.protocol_id "

#define SQL_SELECT_DEVICES_LIST (SQL_SELECT_FROM_DEVICES CR " ORDER BY device_ifc_name ASC")
#define SQL_SELECT_INVOLVED_DEVICES (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_involved = true ORDER BY device_name ASC")
#define SQL_SELECT_SINGLE_INVOLVED_DEVICE (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_involved = true AND device_index = %1")
#define SQL_SELECT_NOT_INVOLVED_DEVICES (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_involved = false ORDER BY device_name ASC")
#define SQL_SELECT_ONE_DEVICE (SQL_SELECT_FROM_DEVICES CR " WHERE device_index = %1")
#define SQL_SELECT_DEVICES_WHERE_NOT_IN (SQL_SELECT_FROM_DEVICES CR " WHERE device_index NOT IN (%1) ORDER BY device_name ASC")


#define SQL_SELECT_FROM_SIGNALS \
  "SELECT " CR \
  "   signals.signal_index as signal_index, " CR \
  "   signals.device_index as signal_device_index, " CR \
  "   signals.signal_name as signal_name, " CR \
  "   signals.timeout as signal_timeout, " CR \
  "   signals.timeout_value as signal_timeout_value, " CR \
  "   signals.timeout_signal_index as signal_timeout_index, " CR \
  "   signals.data_type as signal_data_type, " CR \
  "   signals.data_offset as signal_data_offset, " CR \
  "   signals.data_length as signal_data_length, " CR \
  "   signals.is_involved as signal_is_involved, " CR \
  "   signals.signal_params as signal_params, " CR \
  "   signals.description as signal_description, " CR \
  "   signals.storage0_linked as signal_storage0_linked, " CR \
  "   signals.storage1_linked as signal_storage1_linked, " CR \
  "   signals.storage2_linked as signal_storage2_linked " CR \
  "FROM signals " CR
/* исключено "   signals.is_configured as signal_is_configured*/

#define SQL_SELECT_DEVICE_SIGNALS (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1")

#define SQL_SELECT_INVOLVED_SIGNALS (SQL_SELECT_FROM_SIGNALS " WHERE signals.is_involved = true ORDER BY signals.signal_index ASC")
#define SQL_SELECT_INVOLVED_SIGNALS_DEVICE (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.is_involved = true ORDER BY signals.signal_index ASC")
//#define SQL_SELECT_NOT_INVOLVED_SIGNALS_HARDWARE (SQL_SELECT_FROM_SIGNALS " WHERE signals.hardware_code = '%1' AND signals.device_index is NULL ORDER BY signals.signal_index ASC")
#define SQL_SELECT_NOT_INVOLVED_SIGNALS_DEVICE (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.is_involved = false ORDER BY signals.signal_index ASC")

#define SQL_SELECT_LINKED_SIGNALS_STORAGE_INDEX (SQL_SELECT_FROM_SIGNALS " WHERE signals.storage%1_linked = true AND signals.is_involved = true ORDER BY signals.signal_index ASC")
#define SQL_SELECT_NOT_LINKED_SIGNALS_STORAGE_INDEX (SQL_SELECT_FROM_SIGNALS " WHERE signals.storage%1_linked = false  AND signals.is_involved = false ORDER BY signals.signal_index ASC")


#define SQL_SELECT_SIGNALS_LIST (SQL_SELECT_FROM_SIGNALS " ORDER BY signals.data_offset ASC")
#define SQL_SELECT_ONE_SIGNAL (SQL_SELECT_FROM_SIGNALS " WHERE signals.signal_index = %1")
#define SQL_SELECT_SIGNALS_IN_NOT (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.%2 %3 (%4)")


#define SQL_SELECT_FROM_STORAGES \
  "SELECT storages.id as storage_id, " CR \
  "   storages.storage_name as storage_name, " CR \
  "   storages.storage_index as storage_index, " CR \
  "   storages.host as storage_host, " CR \
  "   storages.port as storage_port, " CR \
  "   storages.login as storage_login, " CR \
  "   storages.pass as storage_pass, " CR \
  "   storages.dbname as storage_dbname, " CR \
  "   storages.table_name as storage_table_name " CR \
  "FROM storages "

#define SQL_SELECT_STORAGES_LIST (SQL_SELECT_FROM_STORAGES " WHERE storages.storage_index in (0,1,2) ORDER BY storages.storage_index ASC")
#define SQL_SELECT_ONE_STORAGE (SQL_SELECT_FROM_STORAGES " WHERE storages.storage_index = %1")


#define SQL_NEW_DEVICE \
  "INSERT INTO devices (device_name, device_index, ifc_index, protocol_id, ifc_params, description)" CR \
  "VALUES ('%1', %2, %3, %4, '%5', '%6');"

#define SQL_CONFIGURE_DEVICE \
  "UPDATE devices SET device_name = '%1', device_params = '%2', is_involved = true, ifc_name = '%3', "\
  "ifc_params = '%4', protocol_id = %5, description = '%6', debug = %7 " CR \
  "WHERE device_index = %8;"

#define SQL_UPDATE_DEVICE \
  "UPDATE devices SET device_name='%1', device_index=%2, ifc_index=%3, protocol_id=%4, ifc_params='%5', description='%6' " CR \
  "WHERE id = %5;"


#define SQL_NEW_STORAGE \
  "INSERT INTO storages ( " CR \
  "     storage_name, host, port, login, pass, dbname, table_name, storage_index) " CR \
  "VALUES ('%1', '%2', %3, '%4', '%5', '%6', '%7', %8);"

#define SQL_UPDATE_STORAGE \
  "UPDATE storages SET " CR \
  "   storage_name='%1', host='%2', port=%3, login='%4', pass='%5', " \
  "   dbname='%6', table_name='%7' " CR \
  "WHERE storage_index = %8;"


#define SQL_NEW_SIGNAL \
  "INSERT INTO signals ( " CR \
  "   signal_index, device_index, signal_name, timeout, timeout_value, timeout_signal_index, " \
  "   data_type, data_offset, data_length, description, storage1, storage2, storage3 ) " CR \
  "VALUES (%1, %2, '%3', %4, %5, %6, %7, %8, %9, '%10', %12, %13, %14);" CR // в конце ; и CR !!

#define SQL_UPDATE_SIGNAL \
  "UPDATE signals SET " CR \
  "   device_index=%1, signal_name='%2', timeout=%3, data_type=%4, data_offset=%5, " \
  "   data_length=%6, description='%7', storage1_index=%8, " \
  "   storage2_index=%9, storage3_index=%10 " CR \
  "WHERE signal_index = %11;"


#define SQL_SET_DEVICES_NOT_INVOLVED "UPDATE devices SET is_involved = false "
#define SQL_DELETE_ALL_DEVICES (SQL_SET_DEVICES_NOT_INVOLVED)
#define SQL_DELETE_SELECTED_DEVICES (SQL_SET_DEVICES_NOT_INVOLVED "WHERE devices.device_index IN (%1)")

#define SQL_SET_DEVICES_INVOLVED "UPDATE devices SET is_involved = true "
#define SQL_ADD_ALL_DEVICES (SQL_SET_DEVICES_INVOLVED)
#define SQL_ADD_SELECTED_DEVICES (SQL_SET_DEVICES_INVOLVED " WHERE devices.device_index IN (%1)")


#define SQL_SET_SIGNALS_NOT_INVOLVED "UPDATE signals SET is_involved = false "
#define SQL_DELETE_ALL_SIGNALS (SQL_SET_SIGNALS_NOT_INVOLVED " WHERE device_index = %1")
#define SQL_DELETE_SELECTED_SIGNALS (SQL_SET_SIGNALS_NOT_INVOLVED "WHERE signals.signal_index IN (%1)")
#define SQL_DELETE_SIGNALS_WHERE_DEVICE_IN (SQL_SET_SIGNALS_NOT_INVOLVED "WHERE signals.device_index IN (%1)")
//#define SQL_UPDATE_DEVICE_INDEX_OPA_AFTER_DELETE "UPDATE signals SET device_index = 2000 WHERE device_index IN (20,21,22,23,24,25,26,27,28) AND is_configured = false"

#define SQL_SET_SIGNALS_INVOLVED "UPDATE signals SET is_involved = true "
#define SQL_ADD_ALL_SIGNALS (SQL_SET_SIGNALS_INVOLVED " WHERE device_index = %1")
#define SQL_ADD_SELECTED_SIGNALS (SQL_SET_SIGNALS_INVOLVED " WHERE signals.signal_index IN (%2)")
//#define SQL_ADD_SELECTED_SIGNALS_OPA (SQL_SET_SIGNALS_INVOLVED " WHERE signals.signal_index IN (%2)")

#define SQL_SET_SIGNALS_NOT_LINKED "UPDATE signals SET storage%1_linked = false "
#define SQL_UNLINK_ALL_SIGNALS (SQL_SET_SIGNALS_NOT_LINKED " WHERE device_index is not NULL")
#define SQL_UNLINK_SELECTED_SIGNALS (SQL_SET_SIGNALS_NOT_LINKED "WHERE signals.signal_index IN (%2)")

#define SQL_SET_SIGNALS_LINKED "UPDATE signals SET storage%1_linked = true "
#define SQL_LINK_ALL_SIGNALS (SQL_SET_SIGNALS_LINKED " WHERE device_index is not NULL")
#define SQL_LINK_SELECTED_SIGNALS (SQL_SET_SIGNALS_LINKED " WHERE signals.signal_index IN (%2)")


#define SQL_SELECT_IFCES "SELECT ifc_index, ifc_name FROM ifces ORDER BY ifc_name"
#define SQL_SELECT_PROTOCOLS "SELECT id, protocol_name FROM protocols ORDER BY protocol_name"


#define SQL_SELECT_DEVICES_COUNT_STR \
    "with " CR \
    "   alldev as (select count(device_index) cnt from devices)," CR \
    "   cfgdev as (select count(device_index) cnt from devices where is_involved = true) " CR \
    "select ' [' || cfgdev.cnt || ' / ' || alldev.cnt || ']'" CR \
    "from alldev,cfgdev;"

//#define SQL_SELECT_SIGNALS_COUNT_STR \
//    "with " CR \
//    "   dev as (select %1 as idx), " CR \
//    "   onedev as (select count(signal_index) cnt from signals where device_index = %1)," CR \
//    "   opadev as (select count(signal_index) cnt from signals where device_index in (%1,2000))," CR \
//    "   cfgdev as (select count(signal_index) cnt from signals where device_index = %1 and is_involved = true) " CR \
//    "select ' [' || cfgdev.cnt || ' / ' || " CR \
//    "               CASE WHEN (dev.idx in (20,21,22,23,24,25,26,27,28)) THEN opadev.cnt ELSE onedev.cnt END " CR \
//    "           || ']'" CR \
//    "from dev,onedev,opadev,cfgdev;"

#define SQL_SELECT_SIGNALS_COUNT_STR \
    "with " CR \
    "   total as (select count(signal_index) cnt from signals where device_index = %1)," CR \
    "   involved as (select count(signal_index) cnt from signals where device_index = %1 and is_involved = true) " CR \
    "select ' [' || involved.cnt || ' / ' || total.cnt || ']'" CR \
    "from involved, total;"


#define SQL_SELECT_DEVICES_BY_STORAGE \
    "select device_index, device_name " \
    "from devices " \
    "where device_index in (select distinct device_index " \
    "                       from signals where storage%1_linked = true) " \
    "      and is_involved = true " \
    "order by device_name ASC"


#define SQL_SELECT_SIGNALS_BY_STORAGE_AND_DEVICE \
    "select signal_index, signal_name, data_type " \
    "from signals where device_index = %1 and storage%2_linked = true " \
    "order by signal_name ASC"

/** определения для версии 1 **/
#else

#define OPA_gamma119  20
#define OPA_gamma122  21
#define OPA_gamma218  22
#define OPA_gamma8    23
#define OPA_gamma38   24
#define OPA_gamma67   25
#define OPA_gamma93   26
#define OPA_gamma123  27
#define OPA_gamma150  28

#define OPA_gamma119_SIGNALS  "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,924,925,926,927,2405,2406,2407,2408,2426"
#define OPA_gamma122_SIGNALS  "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,916,924,925,926,927,2405,2406,2407,2408,2427"
#define OPA_gamma218_SIGNALS  "917,1301,1302,1303,1304,1305,1306,1307,1308,1309,1310,1311,1312,1313,1314,1315,1316,1317,1318,1319,1320,1321,1322,1323,1324,1325,1326,1327,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1340,1341,1342,1343,1344,1345,1346,1347,1348,1349,1350,1351,1352,1353,1354,1355,1356,1357,1358,1359,1360,2430"
#define OPA_gamma8_SIGNALS    "918,1301,1302,1303,1304,1305,1306,1307,1308,1309,1355,2422"
#define OPA_gamma38_SIGNALS   "919,1310,1311,1312,1313,1314,1315,1316,1317,1318,1356,2423"
#define OPA_gamma67_SIGNALS   "920,1319,1320,1321,1322,1323,1324,1325,1326,1327,1357,2424"
#define OPA_gamma93_SIGNALS   "921,1328,1329,1330,1331,1332,1333,1334,1335,1336,1358,2425"
#define OPA_gamma123_SIGNALS  "922,1337,1338,1339,1340,1341,1342,1343,1344,1345,1359,2428"
#define OPA_gamma150_SIGNALS  "923,1346,1347,1348,1349,1350,1351,1352,1353,1354,1360,2429"

#define SQL_SELECT_FROM_DEVICES \
  "SELECT " CR \
  "   devices.device_index as device_index, " CR \
  "   devices.device_name as device_name, " CR \
  "   devices.ifc_id as device_ifc_id, " CR \
  "   devices.protocol_id as device_protocol_id, " CR \
  "   devices.ifc_params as device_ifc_params, " CR \
  "   devices.description as device_description, " CR \
  "   devices.driver_lib_name as device_driver_lib_name, " CR \
  "   devices.is_configured as device_is_configured, " CR \
  "   devices.debug as device_debug, " CR \
  "   devices.timeout as device_timeout, " CR \
  "   ifces.ifc_name as device_ifc_name, " CR \
  "   protocols.protocol_name as device_protocol_name " CR \
  "FROM devices " CR \
  "LEFT JOIN ifces ON ifces.id = devices.ifc_id " CR \
  "LEFT JOIN protocols  ON protocols.id = devices.protocol_id "

#define SQL_SELECT_DEVICES_LIST (SQL_SELECT_FROM_DEVICES CR " ORDER BY device_ifc_name ASC")
#define SQL_SELECT_CONFIGURED_DEVICES (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_configured = true ORDER BY device_name ASC")
#define SQL_SELECT_SINGLE_CONFIGURED_DEVICE (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_configured = true AND device_index = %1")
#define SQL_SELECT_NOT_CONFIGURED_DEVICES (SQL_SELECT_FROM_DEVICES CR " WHERE devices.is_configured = false ORDER BY device_name ASC")
#define SQL_SELECT_ONE_DEVICE (SQL_SELECT_FROM_DEVICES CR " WHERE device_index = %1")
#define SQL_SELECT_DEVICES_WHERE_NOT_IN (SQL_SELECT_FROM_DEVICES CR " WHERE device_index NOT IN (%1) ORDER BY device_name ASC")


#define SQL_SELECT_FROM_SIGNALS \
  "SELECT " CR \
  "   signals.signal_index as signal_index, " CR \
  "   signals.device_index as signal_device_index, " CR \
  "   signals.signal_name as signal_name, " CR \
  "   signals.timeout as signal_timeout, " CR \
  "   signals.timeout_value as signal_timeout_value, " CR \
  "   signals.timeout_signal_index as signal_timeout_index, " CR \
  "   signals.data_type as signal_data_type, " CR \
  "   signals.data_offset as signal_data_offset, " CR \
  "   signals.data_length as signal_data_length, " CR \
  "   signals.description as signal_description, " CR \
  "   signals.is_configured as signal_is_configured, " CR \
  "   signals.storage0_linked as signal_storage0_linked, " CR \
  "   signals.storage1_linked as signal_storage1_linked, " CR \
  "   signals.storage2_linked as signal_storage2_linked, " CR \
  "FROM signals " CR

#define SQL_SELECT_DEVICE_SIGNALS (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1")

#define SQL_SELECT_CONFIGURED_SIGNALS (SQL_SELECT_FROM_SIGNALS " WHERE signals.is_configured = true ORDER BY signals.signal_index ASC")
#define SQL_SELECT_CONFIGURED_SIGNALS_DEVICE (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.is_configured = true ORDER BY signals.signal_index ASC")
#define SQL_SELECT_NOT_CONFIGURED_SIGNALS_DEVICE (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.is_configured = false ORDER BY signals.signal_index ASC")

#define SQL_SELECT_LINKED_SIGNALS_STORAGE_INDEX (SQL_SELECT_FROM_SIGNALS " WHERE signals.storage%1_linked = true AND signals.is_configured = true ORDER BY signals.signal_index ASC")
#define SQL_SELECT_NOT_LINKED_SIGNALS_STORAGE_INDEX (SQL_SELECT_FROM_SIGNALS " WHERE signals.storage%1_linked = false AND signals.is_configured = true ORDER BY signals.signal_index ASC")

//#define SQL_SELECT_NOT_CONFIGURED_SIGNALS_DEVICE_OPA (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index in (20,21,22,23,24,25,26,27,28) AND signals.is_configured = false ORDER BY signals.signal_index ASC")
#define SQL_SELECT_NOT_CONFIGURED_SIGNALS_DEVICE_OPA (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = 2000 AND signals.signal_index in (%1) AND signals.is_configured = false ORDER BY signals.signal_index ASC")


#define SQL_SELECT_SIGNALS_LIST (SQL_SELECT_FROM_SIGNALS " ORDER BY signals.data_offset ASC")
#define SQL_SELECT_ONE_SIGNAL (SQL_SELECT_FROM_SIGNALS " WHERE signals.signal_index = %1")
#define SQL_SELECT_SIGNALS_IN_NOT (SQL_SELECT_FROM_SIGNALS " WHERE signals.device_index = %1 AND signals.%2 %3 (%4)")



#define SQL_SELECT_FROM_STORAGES \
  "SELECT storages.id as storage_id, " CR \
  "   storages.storage_name as storage_name, " CR \
  "   storages.storage_index as storage_index, " CR \
  "   storages.host as storage_host, " CR \
  "   storages.port as storage_port, " CR \
  "   storages.login as storage_login, " CR \
  "   storages.pass as storage_pass, " CR \
  "   storages.dbname as storage_dbname, " CR \
  "   storages.table_name as storage_table_name " CR \
  "FROM storages "

#define SQL_SELECT_STORAGES_LIST (SQL_SELECT_FROM_STORAGES " WHERE storages.storage_index in (0,1,2) ORDER BY storages.storage_index ASC")
#define SQL_SELECT_ONE_STORAGE (SQL_SELECT_FROM_STORAGES " WHERE storages.storage_index = %1")


#define SQL_NEW_DEVICE \
  "INSERT INTO devices (device_name, device_index, ifc_id, protocol_id, ifc_params, description)" CR \
  "VALUES ('%1', %2, %3, %4, '%5', '%6');"

#define SQL_CONFIGURE_DEVICE \
  "UPDATE devices SET device_name='%1', is_configured=true, ifc_id=%2, protocol_id=%3, "\
  "ifc_params='%4', description='%5', debug=%6 " CR \
  "WHERE device_index = %7;"

#define SQL_UPDATE_DEVICE \
  "UPDATE devices SET device_name='%1', device_index=%2, ifc_id=%3, protocol_id=%4, ifc_params='%5', description='%6' " CR \
  "WHERE id = %5;"


#define SQL_NEW_STORAGE \
  "INSERT INTO storages ( " CR \
  "     storage_name, host, port, login, pass, dbname, table_name, storage_index) " CR \
  "VALUES ('%1', '%2', %3, '%4', '%5', '%6', '%7', %8);"

#define SQL_UPDATE_STORAGE \
  "UPDATE storages SET " CR \
  "   storage_name='%1', host='%2', port=%3, login='%4', pass='%5', " \
  "   dbname='%6', table_name='%7' " CR \
  "WHERE storage_index = %8;"


#define SQL_NEW_SIGNAL \
  "INSERT INTO signals ( " CR \
  "   signal_index, device_index, signal_name, timeout, timeout_value, timeout_signal_index, " \
  "   data_type, data_offset, data_length, description, storage1, storage2, storage3 ) " CR \
  "VALUES (%1, %2, '%3', %4, %5, %6, %7, %8, %9, '%10', %12, %13, %14);" CR // в конце ; и CR !!

#define SQL_UPDATE_SIGNAL \
  "UPDATE signals SET " CR \
  "   device_index=%1, signal_name='%2', timeout=%3, data_type=%4, data_offset=%5, " \
  "   data_length=%6, description='%7', storage1_index=%8, " \
  "   storage2_index=%9, storage3_index=%10 " CR \
  "WHERE signal_index = %11;"


#define SQL_SET_DEVICES_NOT_CONFIGURED "UPDATE devices SET is_configured = false "
#define SQL_DELETE_ALL_DEVICES (SQL_SET_DEVICES_NOT_CONFIGURED)
#define SQL_DELETE_SELECTED_DEVICES (SQL_SET_DEVICES_NOT_CONFIGURED "WHERE devices.device_index IN (%1)")

#define SQL_SET_DEVICES_CONFIGURED "UPDATE devices SET is_configured = true "
#define SQL_ADD_ALL_DEVICES (SQL_SET_DEVICES_CONFIGURED)
#define SQL_ADD_SELECTED_DEVICES (SQL_SET_DEVICES_CONFIGURED " WHERE devices.device_index IN (%1)")

#define SQL_SET_SIGNALS_NOT_CONFIGURED "UPDATE signals SET is_configured = false "
#define SQL_DELETE_ALL_SIGNALS (SQL_SET_SIGNALS_NOT_CONFIGURED " WHERE device_index = %1")
#define SQL_DELETE_SELECTED_SIGNALS (SQL_SET_SIGNALS_NOT_CONFIGURED "WHERE signals.signal_index IN (%1)")
#define SQL_DELETE_SIGNALS_WHERE_DEVICE_IN (SQL_SET_SIGNALS_NOT_CONFIGURED "WHERE signals.device_index IN (%1)")
#define SQL_UPDATE_DEVICE_INDEX_OPA_AFTER_DELETE "UPDATE signals SET device_index = 2000 WHERE device_index IN (20,21,22,23,24,25,26,27,28) AND is_configured = false"

#define SQL_SET_SIGNALS_CONFIGURED "UPDATE signals SET is_configured = true, device_index = %1"
//#define SQL_ADD_ALL_SIGNALS (SQL_SET_SIGNALS_CONFIGURED " WHERE device_index = %1")
#define SQL_ADD_SELECTED_SIGNALS (SQL_SET_SIGNALS_CONFIGURED " WHERE signals.signal_index IN (%2)")
#define SQL_ADD_SELECTED_SIGNALS_OPA (SQL_SET_SIGNALS_CONFIGURED " WHERE signals.signal_index IN (%2)")

#define SQL_SET_SIGNALS_NOT_LINKED "UPDATE signals SET storage%1_linked = false "
#define SQL_UNLINK_ALL_SIGNALS (SQL_SET_SIGNALS_NOT_LINKED " WHERE is_configured = true")
#define SQL_UNLINK_SELECTED_SIGNALS (SQL_SET_SIGNALS_NOT_LINKED "WHERE signals.signal_index IN (%2)")

#define SQL_SET_SIGNALS_LINKED "UPDATE signals SET storage%1_linked = true "
#define SQL_LINK_ALL_SIGNALS (SQL_SET_SIGNALS_LINKED " WHERE is_configured = true")
#define SQL_LINK_SELECTED_SIGNALS (SQL_SET_SIGNALS_LINKED " WHERE signals.signal_index IN (%2)")

#define SQL_SELECT_IFCES "SELECT id, ifc_name FROM ifces ORDER BY ifc_name"

#define SQL_SELECT_PROTOCOLS "SELECT id, protocol_name FROM protocols ORDER BY protocol_name"


#define SQL_SELECT_DEVICES_COUNT_STR \
    "with " CR \
    "   alldev as (select count(device_index) cnt from devices)," CR \
    "   cfgdev as (select count(device_index) cnt from devices where is_configured = true) " CR \
    "select ' [' || cfgdev.cnt || ' / ' || alldev.cnt || ']'" CR \
    "from alldev,cfgdev;"

#define SQL_SELECT_SIGNALS_COUNT_STR \
    "with " CR \
    "   dev as (select %1 as idx), " CR \
    "   onedev as (select count(signal_index) cnt from signals where device_index = %1)," CR \
    "   opadev as (select count(signal_index) cnt from signals where device_index in (%1,2000))," CR \
    "   cfgdev as (select count(signal_index) cnt from signals where device_index = %1 and is_configured = true) " CR \
    "select ' [' || cfgdev.cnt || ' / ' || " CR \
    "               CASE WHEN (dev.idx in (20,21,22,23,24,25,26,27,28)) THEN opadev.cnt ELSE onedev.cnt END " CR \
    "           || ']'" CR \
    "from dev,onedev,opadev,cfgdev;"


#define SQL_SELECT_DEVICES_BY_STORAGE \
    "select device_index, device_name " \
    "from devices " \
    "where device_index in (select distinct device_index " \
    "                       from signals where storage%1_linked = true) " \
    "      and is_configured = true " \
    "order by device_name ASC"


#define SQL_SELECT_SIGNALS_BY_STORAGE_AND_DEVICE \
    "select signal_index, signal_name, data_type " \
    "from signals where device_index = %1 and storage%2_linked = true and is_configured = true " \
    "order by signal_name ASC"


#endif


#define SQL_SELECT_DB_INFO \
  "select db_version, stand_name from stand_info"



/** *********** POSTGRES *************** **/

#define SQL_UPDATE_VALUE \
  "UPDATE %1 SET value=%2, last_update='%3' WHERE signal_name = '%4';" CR

#define PG_FUNC_SET_VALUES \
  "select set_values('%1');" CR

#define PG_FUNC_SET_RESERVE_VALUES \
  "select set_reserve_values('%1');" CR

#define PG_SQL_EXEC_SET_SIGNAL_VALUE \
  "select set_val(%1, %2);" CR

#define PG_SQL_EXEC_SET_SIGNAL_LOST_VALUE \
  "select set_lost(%1, %2);" CR

#define PG_SQL_EXEC_SET_SIGNAL_FROM_RESERVED \
  "select set_rsrv(%1, %2);" CR


#endif // DEFS_H
