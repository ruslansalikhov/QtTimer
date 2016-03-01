QT       -= gui
CONFIG  += c++11

TARGET = QtTimer

DEFINES += QTTIMER_LIBRARY

QMAKE_TARGET_BUNDLE_PREFIX=com.example.qttimer

android {
    TEMPLATE = app
    QT += androidextras
    SOURCES += $$PWD/qttimer_android.cpp

    isEmpty(NDK_ROOT): NDK_ROOT = $$(ANDROID_NDK_ROOT)
    isEmpty(NDK_ROOT): NDK_ROOT = $$DEFAULT_ANDROID_NDK_ROOT
    NDK_TOOLCHAIN_VERSION = $$(ANDROID_NDK_TOOLCHAIN_VERSION)
    isEmpty(NDK_TOOLCHAIN_VERSION): NDK_TOOLCHAIN_VERSION = $$DEFAULT_ANDROID_NDK_TOOLCHAIN_VERSION

    # Qt libraries
    QTLIBS.path = /libs/$$ANDROID_TARGET_ARCH
    QTLIBS.files += $$[QT_INSTALL_PREFIX]/lib/libQt5Core.so

    # Qt Extra JAR
    QTJARS.path = /libs
    QTJARS.files += $$[QT_INSTALL_PREFIX]/jar/QtAndroid-bundled.jar

    # Android libraries
    ANDROID_LIBS.path = /libs/$$ANDROID_TARGET_ARCH
    ANDROID_LIBS.files += $$NDK_ROOT/sources/cxx-stl/gnu-libstdc++/$$NDK_TOOLCHAIN_VERSION/libs/$$ANDROID_TARGET_ARCH/libgnustl_shared.so

    # Java, resource, gradle
    QTTIMER_JAVASRC.path = /
    QTTIMER_JAVASRC.files += $$files($$PWD/android/*)
    INSTALLS += QTTIMER_JAVASRC QTLIBS QTJARS ANDROID_LIBS
} else:ios {
#    ## the objective sources should be put in this variable
#    OBJECTIVE_SOURCES += \
#            $$PWD/qttimer_ios.mm
} else {
    TEMPLATE = lib
}

SOURCES += qttimer.cpp

HEADERS += qttimer.h \
    qttimer_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    $$PWD/README.md \
    $$files($$PWD/android/gradle/wrapper/*) \
    $$files($$PWD/android/src/com/example/qttimer/*) \
    $$files($$PWD/android/*) \
    $$files($$PWD/*.sh)
