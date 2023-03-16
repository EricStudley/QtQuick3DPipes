#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PipeEnums.h"
#include "PipeModel.h"
#include "PipeController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    PipeModel *pipeModel = new PipeModel(&app);
    PipeController *pipeController = new PipeController(&app, pipeModel);

    qmlRegisterUncreatableType<PipeEnums>("Pipe", 1, 0, "Direction", "Error: Direction is an uncreatable enum type.");
    engine.rootContext()->setContextProperty("pipeModel", QVariant::fromValue(pipeModel));
    engine.rootContext()->setContextProperty("pipeController", QVariant::fromValue(pipeController));

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
