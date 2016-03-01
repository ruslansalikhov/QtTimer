#include "qttimer.h"

#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <QtAndroidExtras>

static const char *javaClassName = "com/example/qttimer/QtTimerBinding";

static void android_message_handler(QtMsgType type,
                                    const QMessageLogContext &/*context*/,
                                    const QString &message)
{
    android_LogPriority priority = ANDROID_LOG_DEBUG;
    switch (type)
    {
        case QtDebugMsg:    priority = ANDROID_LOG_DEBUG;   break;
        case QtWarningMsg:  priority = ANDROID_LOG_WARN;    break;
        case QtInfoMsg:     priority = ANDROID_LOG_INFO;    break;
        case QtCriticalMsg: priority = ANDROID_LOG_ERROR;   break;
        case QtFatalMsg:    priority = ANDROID_LOG_FATAL;   break;
    };

    __android_log_print(priority, "Qt", "%s", qPrintable(message));
}

/*
 * --------------------------------------
 * To Java callsbacks
 * --------------------------------------
*/
static void emitTimeout()
{
    qDebug() << Q_FUNC_INFO;
    // TODO: check why it doesn't work
    //QAndroidJniObject::callStaticMethod<void>(javaClassName, "emitTimeout", "()V");

    QAndroidJniEnvironment env;
    jclass clazz = env->FindClass(javaClassName);
    if (env->ExceptionCheck())
    {
        qWarning() << "Cannot find clazz";
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();

        return;
    }
    QAndroidJniObject::callStaticMethod<void>(clazz, "emitTimeout");
    env->DeleteLocalRef(clazz);
    if (env->ExceptionCheck())
    {
        qWarning() << "Cannot call static method";
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();

        return;
    }
}

/*
 * --------------------------------------
 * From Java calls
 * --------------------------------------
*/
static void fromJavaInitialize(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    int argc = 0;
    char *argv[] = {};
    new QCoreApplication(argc, argv);

    QObject::connect(QtTimer::instance(), &QtTimer::timeout, emitTimeout);
}

static void fromJavaStart(JNIEnv *env, jobject thiz, jint msec)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    QtTimer::instance()->start(msec);
}

static void fromJavaStop(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    return  QtTimer::instance()->stop();
}

static void fromJavaProcessEvents(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    QCoreApplication::processEvents();
}

/*
 * --------------------------------------
 * On library loading procedures
 * --------------------------------------
*/
static JNINativeMethod methods[]
{
    {"initialize",      "()V",      reinterpret_cast<void *>(fromJavaInitialize)},
    {"processEvents",   "()V",      reinterpret_cast<void *>(fromJavaProcessEvents)},
    {"start",           "(I)V",     reinterpret_cast<void *>(fromJavaStart)},
    {"stop",            "()V",      reinterpret_cast<void *>(fromJavaStop)},
};

extern "C" jint JNI_OnLoad(JavaVM* vm, void * /*reserved*/)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
        return JNI_FALSE;

    // installing jni methods
    jclass clazz = env->FindClass(javaClassName);
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
    env->DeleteLocalRef(clazz);

    qInstallMessageHandler(android_message_handler);
    qDebug() << "QtTimer loaded";

    return JNI_VERSION_1_4;
}
