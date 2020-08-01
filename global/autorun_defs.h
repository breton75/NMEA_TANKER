#ifndef DEFS
#define DEFS

#define PARAM_WAIT_POSTGRES  "WAIT_POSTGRES"
#define PARAM_WAIT_AGGREGATE "WAIT_AGGREGATE"


#define PARAM_WIDGET_USER   "WIDGET_USER"
#define PARAM_WIDGET_PASS   "WIDGET_PASS"
#define PARAM_WIDGET_NAME   "WIDGET_NAME"
#define PARAM_WIDGET_HOST_n "WIDGET_HOST_%1"
#define PARAM_WIDGET_CMD    "WIDGET_CMD"
#define PARAM_WIDGET_XMS    "WIDGET_XMS"
#define PARAM_WIDGET_XMX    "WIDGET_XMX"

#define COMMAND_BEFORE_n    "COMMAND_BEFORE_%1"
#define COMMAND_AFTER_n     "COMMAND_AFTER_%1"

#define T_WIDGET_HOST       "[%HOST]"
#define T_WIDGET_PORT       "[%PORT]"
#define T_WIDGET_USER       "[%USER]"
#define T_WIDGET_PASS       "[%PASS]"
#define T_WIDGET_NAME       "[%WIDGET_NAME]"
#define T_XMS               "[%XMS]"
#define T_XMX               "[%XMX]"

#define DEFAULT_WIDGET_CMD "sudo /opt/AggreGate/widget_player -f " T_WIDGET_HOST " "\
                                                                   T_WIDGET_PORT " "\
                                                                   T_WIDGET_USER " "\
                                                                   T_WIDGET_PASS " users."\
                                                                   T_WIDGET_USER ".widgets."\
                                                                   T_WIDGET_NAME


#define JAVA_WIDGET_RUN_SCRIPT     "/opt/AggreGate/jre/bin/java -Dinstall4j.jvmDir=/opt/AggreGate/jre "\
"-Dexe4j.moduleName=/opt/AggreGate/widget_player -Dinstall4j.launcherId=3163 "\
"-Dinstall4j.swt=false -Di4jv=0 -Di4jv=0 -Di4jv=0 -Di4jv=0 -Di4jv=0 -Xms" T_XMS "m "\
"-Xmx" T_XMX "m -XX:+UseConcMarkSweepGC -Duser.language=ru -Di4j.vpt=true -classpath "\
"/opt/AggreGate/.install4j/i4jruntime.jar:/opt/AggreGate/jar/aggregate-api-libs.jar"\
":/opt/AggreGate/jar/aggregate-api.jar:/opt/AggreGate/jar/aggregate-api-libs.jar:"\
"/opt/AggreGate/jar/aggregate-commons.jar:/opt/AggreGate/jar/aggregate-components.jar:"\
"/opt/AggreGate/jar/aggregate-extensions.jar:/opt/AggreGate/jar/client.jar:"\
"/opt/AggreGate/jar/client-libs.jar:/opt/AggreGate/jar/commons-libs.jar:"\
"/opt/AggreGate/jar/components-libs.jar:/opt/AggreGate/jar/extensions-libs.jar:"\
"/opt/AggreGate/jar/gui-builder.jar:/opt/AggreGate/jar/jboss-client.jar:"\
"/opt/AggreGate/jar/mysql-connector-java-5.1.30-bin.jar:/opt/AggreGate/jar/postgresql-9.4.1208.jre6.jar:"\
"/opt/AggreGate/jar/server-core.jar:/opt/AggreGate/jar/server-libs.jar:/opt/AggreGate/jar/upgrade-app.jar:"\
"/opt/AggreGate/jar/widget-commons.jar:/opt/AggreGate/jar/widget-libs.jar:"\
"/opt/AggreGate/jar/widget-runtime.jar com.install4j.runtime.launcher.UnixLauncher launch d7e2bce4 "\
"/opt/AggreGate/error.log '' "\
"com.tibbo.aggregate.common.widget.runtime.WidgetPlayer -f " T_WIDGET_HOST " "\
                                                             T_WIDGET_PORT " "\
                                                             T_WIDGET_USER " "\
                                                             T_WIDGET_PASS " users."\
                                                             T_WIDGET_USER ".widgets."\
                                                             T_WIDGET_NAME


#define STATE_STARTED "Запущен"
#define STATE_AWAITING "Не запущен"


enum {

  editMode,
  runMode,
  statusMode

};

#endif // DEFS

