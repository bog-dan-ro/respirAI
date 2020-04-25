#include "alarmsmanager.h"
#include "sensorsdatamanager.h"
#include "startupmanager.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#ifdef DUMMY_DATA
# include <QTimer>
#endif
int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("800"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("480"));

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    // Register types
    qmlRegisterUncreatableType<StartupManager>("respirAi", 1, 0, "StartupManager", "Use __startupManager");
    qmlRegisterUncreatableType<SensorsData>("respirAi", 1, 0, "SensorsData", "Can't create this type");

    // Set context objects
    auto rootContext = engine.rootContext();
    SensorsDataManager sensorsDataManager;
    rootContext->setContextProperty("__sensorsDataManager", &sensorsDataManager);

    StartupManager startupManager;
    rootContext->setContextProperty("__startupManager", &startupManager);

    AlarmsManager alarmManager;
    rootContext->setContextProperty("__alamManager", &alarmManager);

    engine.load(url);

#ifdef DUMMY_DATA
    QTimer dummyData;
    QTimer::connect(&dummyData, &QTimer::timeout, [&sensorsDataManager] {
        SensorsData data;
        data.pressure = (qrand() % 6500) / 100 - 5;
        data.flow = (qrand() % 1000) / 10 - 50;
        data.volume = qrand() % 500;
        sensorsDataManager.setData(data);
    });
    dummyData.start(100);
#endif
    return app.exec();
}
