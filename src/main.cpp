#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QSortFilterProxyModel>
#ifdef Q_OS_ANDROID
#include "smsclient.h"
#include "notificationclient.h"
#endif
#include "uicontrollor.h"
#include "listmodel.h"
#include "conversionlistitem.h"
#include "conversiondatalistitem.h"
#include "contactlistitem.h"
#include "imageview.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ImageView>("ImageViews", 1, 0, "ImageView");
    qRegisterMetaType< QList<ListItem *> >("QList<ListItem *>");
    qmlRegisterUncreatableType<UIControllor> ("UIControllor", 1, 0, "PageView", "indentify Page");

    QQmlApplicationEngine engine;

    UIControllor *uictrl = new UIControllor(&engine);
    engine.rootContext()->setContextProperty(QLatin1String("UIControllor"), uictrl);


#ifdef Q_OS_ANDROID
    // sms client
    SMSClient *smsClient = new SMSClient(&app);

    // notification client
    NotificationClient *notificationClient = new NotificationClient(&app);

    QObject::connect(smsClient, SIGNAL(newMsgReceived(int, const QString &, const QString &)),
                     notificationClient, SLOT(showNotification(int, const QString &, const QString &)));

    // list model of conversion
    ListModel *conversionListModel = new ListModel(new ConversionListItem, &app);
    conversionListModel->setObjectName("conversionListModel");
    QObject::connect(smsClient, SIGNAL(conversionListAppended(QList<ListItem *>)),
                     conversionListModel, SLOT(appendRows(QList<ListItem *>)));
    QObject::connect(smsClient, SIGNAL(conversionListUpdated(QList<ListItem *>)),
                     conversionListModel, SLOT(updateRows(QList<ListItem *>)));

    // list model of conversion data
    ListModel *conversionDataListModel = new ListModel(new ConversionDataListItem, &app);
    conversionDataListModel->setObjectName("conversionDataListModel");
    QObject::connect(smsClient, SIGNAL(conversionDataPrepended(QList<ListItem *>)),
                     conversionDataListModel, SLOT(prependRows(QList<ListItem *>)));
    QObject::connect(smsClient, SIGNAL(conversionDataUpdated(QList<ListItem *>)),
                     conversionDataListModel, SLOT(updateRows(QList<ListItem *>)));
    QObject::connect(smsClient, SIGNAL(appendMsgToConversion(ListItem *)),
                     conversionDataListModel, SLOT(appendRow(ListItem *)));

    // list model of contacts
    ListModel *contactListModel = new ListModel(new ContactListItem, &app);
    contactListModel->setObjectName("contactListModel");
    QObject::connect(smsClient, SIGNAL(contactListUpdate(QList<ListItem *>)),
                     contactListModel, SLOT(updateRows(QList<ListItem *>)));

    QSortFilterProxyModel *contactListProxyModel = new QSortFilterProxyModel(&app);
    contactListProxyModel->setSourceModel(contactListModel);
    contactListProxyModel->setFilterKeyColumn(0);
    contactListProxyModel->setFilterRole(ContactListItem::UserDisplayNameRole);

    QObject::connect(uictrl, SIGNAL(contactListFilterChanged(const QString &)),
                     contactListProxyModel, SLOT(setFilterFixedString(const QString &)));

    engine.rootContext()->setContextProperty(QLatin1String("notificationClient"), notificationClient);
    engine.rootContext()->setContextProperty(QLatin1String("SMSClient"), smsClient);
    engine.rootContext()->setContextProperty(QLatin1String("ConversionListModel"), conversionListModel);
    engine.rootContext()->setContextProperty(QLatin1String("ConversionDataListModel"), conversionDataListModel);
    engine.rootContext()->setContextProperty(QLatin1String("ContactListModel"), contactListModel);
    engine.rootContext()->setContextProperty(QLatin1String("ContactListProxyModel"), contactListProxyModel);

#endif

    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    return app.exec();
}
