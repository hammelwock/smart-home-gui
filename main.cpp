#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Com.h"
#include "SmartItems.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    ComPortAdapter *comPortAdapter = new ComPortAdapter();
    Home *home = new Home(comPortAdapter->receptionConfig());

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("home", home);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
