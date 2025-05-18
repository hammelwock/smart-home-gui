#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Com.h"
#include "SmartItems.h"
#include "Core.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    allElements = new AllElements();

    ComPortAdapter *comPortAdapter = new ComPortAdapter("ttyUSB0", "config.json");
    ComPortAdapter *comPortAdapter2 = new ComPortAdapter("ttyACM0", "config1.json");

    Home *home = new Home();

    home->addController(new Controller(comPortAdapter2->readConfig(), comPortAdapter2));
    home->addController(new Controller(comPortAdapter->readConfig(), comPortAdapter));

    qobject_cast<Controller*>(home->getControllerList().at(0))->saveJson();

    engine.rootContext()->setContextProperty("home", home);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
                &engine,
                &QQmlApplicationEngine::objectCreated,
                &app,
                [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
