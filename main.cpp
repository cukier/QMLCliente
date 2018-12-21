#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "clistmodel.h"
#include "clientsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    CListModel listmodel;
    ClientSocket socket(&listmodel, "mauricio", "mauricio1");

    context->setContextProperty("listmodel", &listmodel);
    context->setContextProperty("socket", &socket);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
