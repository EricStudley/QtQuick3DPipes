#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PipeEnums.h"
#include "PipeModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    PipeModel* pipeModel = new PipeModel(&app);

    qmlRegisterUncreatableType<PipeEnums>("Pipe", 1, 0, "Direction", "Error: Direction is an uncreatable enum type.");

    QVector<QQmlContext::PropertyPair> contextProperties;
    contextProperties.append(QQmlContext::PropertyPair{"pipeModel", QVariant::fromValue(pipeModel)});
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
