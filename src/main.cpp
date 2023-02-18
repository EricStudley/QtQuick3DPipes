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

    PipeModel* pipeModel = new PipeModel(&app);
    PipeController *pipeController = new PipeController(pipeModel, &app);

    qRegisterMetaType<Side>("Side");
    qRegisterMetaType<Color>("Color");
    qmlRegisterUncreatableType<PipeEnums>("Pipe", 1, 0, "Side", "Error: Side is an enum type.");
    qmlRegisterUncreatableType<PipeEnums>("Pipe", 1, 0, "Color", "Error: Color is an enum type.");

    QVector<QQmlContext::PropertyPair> contextProperties;
    contextProperties.append(QQmlContext::PropertyPair{"pipeModel", QVariant::fromValue(pipeModel)});
    contextProperties.append(QQmlContext::PropertyPair{"pipeController", QVariant::fromValue(pipeController)});
    engine.rootContext()->setContextProperties(contextProperties);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
