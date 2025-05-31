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
    home = new Home();
    QList<ComPortAdapter*> comPortAdapterList;

    QFile file("ports");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString line = in.readLine();
           comPortAdapterList.append(new ComPortAdapter(line));
        }
        file.close();
    }
    //ComPortAdapter *comPortAdapter = new ComPortAdapter("ttyUSB5", "config.json");
    ComPortAdapter *comPortAdapter2 = new ComPortAdapter("ttyACM0");

//    qobject_cast<Controller*>(home->getControllerList().at(0))->saveJson();

    engine.rootContext()->setContextProperty("home", home);

    qmlRegisterUncreatableType<Regulator>("MyApp", 1, 0, "Regulator", "Available as property only");

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
