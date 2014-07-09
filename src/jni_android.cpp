/****************************************************************************
**
** Copyright (C) 2014 Chen Jian <Jerry.C@foxmail.com>
** All rights reserved.
** Contact: Chen Jian <Jerry.C@foxmail.com>
**
** If you want use this file in the commercial usage, please MUST contact to me.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Chen Jian, email: Jerry.C@foxmail.com
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras>
#include <QtAndroid>
#include <jni.h>
#include <android/log.h>
#include "smsclient.h"

void QtSmsClient_jniOnMsgReceived(JNIEnv */*env*/, jobject /*javaObject*/, jlong qtObject, jlong threadid)
{
    reinterpret_cast<SMSClient *>(qtObject)->onMsgReceived(threadid);
}

void QtSmsClient_jniOnMsgSendOut(JNIEnv */*env*/, jobject /*javaObject*/, jlong qtObject, jlong msgid, jboolean isSuccess)
{
    reinterpret_cast<SMSClient *>(qtObject)->onMsgSendOut(msgid, isSuccess);
}

void QtSmsClient_jniOnMsgDelivered(JNIEnv */*env*/, jobject /*javaObject*/, jlong qtObject, jlong msgid)
{
    reinterpret_cast<SMSClient *>(qtObject)->onMsgDelivered(msgid);
}

void QtSmsClient_jniOnMsgConversionCreated(JNIEnv */*env*/, jobject /*javaObject*/, jlong qtObject, jlong threadid)
{
    reinterpret_cast<SMSClient *>(qtObject)->onMsgConversionCreated(threadid);
}


static JNINativeMethod methods_sms_client[] =
{
    {"onMsgReceived", "(JJ)V", (void *) QtSmsClient_jniOnMsgReceived},
    {"onMsgSendOut", "(JJZ)V", (void *) QtSmsClient_jniOnMsgSendOut},
    {"onMsgDelivered", "(JJ)V", (void *) QtSmsClient_jniOnMsgDelivered},
    {"onMsgConversionCreated", "(JJ)V", (void *) QtSmsClient_jniOnMsgConversionCreated}
};

static const char logTag[] = "QtSMSClient";
static const char classErrorMsg[] = "Can't find class \"%s\"";

#define FIND_AND_CHECK_CLASS(CLASS_NAME) \
clazz = env->FindClass(CLASS_NAME); \
if (!clazz) { \
    __android_log_print(ANDROID_LOG_FATAL, logTag, classErrorMsg, CLASS_NAME); \
    return JNI_FALSE; \
}

static bool registerNatives(JNIEnv *env)
{
    jclass clazz;

    FIND_AND_CHECK_CLASS("org/qtproject/example/QSMS/sms/SMSClient");
    if (env->RegisterNatives(clazz, methods_sms_client, sizeof(methods_sms_client) / sizeof(methods_sms_client[0])) < 0) {
        __android_log_print(ANDROID_LOG_FATAL, logTag, "RegisterNatives for SMSClient failed");
        return false;
    }

    return true;
}

jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    typedef union {
        JNIEnv *nativeEnvironment;
        void *venv;
    } UnionJNIEnvToVoid;

    UnionJNIEnvToVoid uenv;
    uenv.venv = 0;

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_FATAL, logTag, "GetEnv failed");
        return -1;
    }

    JNIEnv *env = uenv.nativeEnvironment;
    if (!registerNatives(env)) {
        __android_log_print(ANDROID_LOG_FATAL, logTag, "registerNatives failed");
        return -1;
    }

    return JNI_VERSION_1_4;
}
