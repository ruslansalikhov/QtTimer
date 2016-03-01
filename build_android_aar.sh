#! /bin/bash

set -e

if [ -z $QT_BASE_DIR ]; then
    echo "Eror: QT_BASE_DIR is no set"
    exit 1
fi

if [ -z $ANDROID_NDK_ROOT ]; then
    echo "Error: ANDROID_NDK_ROOT is no set"
    exit 1
fi

BASEDIR=$(dirname $0)
echo $BASEDIR
pushd $BASEDIR
BASEDIR=`pwd`

#rm -rf _build
[ ! -d _build ] && mkdir _build
pushd _build

for qt_dir in $(ls -d $QT_BASE_DIR/android_*); do
 type=${qt_dir##*/}; 
 # arm
 [ ! -d $type ] && mkdir $type
 pushd $type

 $qt_dir/bin/qmake -spec android-g++ $BASEDIR
 make -j4
 INSTALL_ROOT=$BASEDIR/_build/aar make install

# Extracting symbols for minidump
#dump_syms libHomeBridge.so > libHomeBridge.so.sym

# Extracting debug symbols
#mv libHomeBridge.so libHomeBridge.so.debug
#${ANDROID_NDK_ROOT}/toolchains/${ANDROID_TOOLCHAIN}/prebuilt/${ANDROID_NDK_HOST}/arm-linux-androideabi/bin/strip --strip-debug --strip-unneeded libHomeBridge.so.debug -o  libHomeBridge.so
#${ANDROID_NDK_ROOT}/toolchains/${ANDROID_TOOLCHAIN}/prebuilt/${ANDROID_NDK_HOST}/arm-linux-androideabi/bin/objcopy --add-gnu-debuglink=libHomeBridge.so.debug libHomeBridge.so

#echo "${ANDROID_NDK_ROOT}/toolchains/${ANDROID_TOOLCHAIN}/prebuilt/${ANDROID_NDK_HOST}/arm-linux-androideabi/bin/strip"
popd # type
done # for

# Compiling AAR
pushd aar
./gradlew assembleDefault
cp -v build/outputs/aar/aar-release.aar $BASEDIR/_build/qttimer.aar
popd

popd # _build

popd # basedir
