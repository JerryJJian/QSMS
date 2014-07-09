TEMPLATE = app

QT += qml quick

CONFIG += qtquickcompiler

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

android: {

    QT += androidextras
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    HEADERS += \
        smsclient.h \
        notificationclient.h

    SOURCES += \
        jni_android.cpp \
        smsclient.cpp \
        notificationclient.cpp

    OTHER_FILES += \
        android/AndroidManifest.xml \
        android/src/org/qtproject/example/QSMS/notification/NotificationClient.java \
        android/src/org/qtproject/example/QSMS/sms/SMSClient.java \
        android/src/org/qtproject/example/QSMS/sms/Base64Util.java
}

SOURCES += main.cpp \
    uicontrollor.cpp \
    listmodel.cpp \
    conversionlistitem.cpp \
    conversiondatalistitem.cpp \
    contactlistitem.cpp \
    imageview.cpp

RESOURCES += qml.qrc \
    image.qrc

HEADERS += \
    uicontrollor.h \
    listmodel.h \
    conversionlistitem.h \
    conversiondatalistitem.h \
    contactlistitem.h \
    imageview.h
