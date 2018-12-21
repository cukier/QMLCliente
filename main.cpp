#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "clistmodel.h"
#include "clientsocket.h"
#include "filterproxymodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    CListModel listmodel;
    ClientSocket socket(&listmodel, "mauricio", "mauricio1");
    FilterProxyModel filterModel;

    filterModel.setSourceModel(&listmodel);
    filterModel.setFilterRole(ItemNameRole);
    filterModel.setSortRole(ItemNameRole);

    context->setContextProperty("filterModel", &filterModel);
    context->setContextProperty("listmodel", &listmodel);
    context->setContextProperty("socket", &socket);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
