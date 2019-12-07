#!/bin/bash

buildDir=$1

if [ "$buildDir" == "" ]; then
    echo "must provide a build dir"
    exit 1
fi

echo "buildDir = $buildDir"

bundle=$2

if [ "$bundle" == "" ]; then
    echo "must provide a bundle dir"
    exit 1
fi

echo "bundle = $bundle"

ver=$3

if [ "$ver" == "" ]; then
    echo "must provide a version"
    exit 1
fi

echo "bundle = $ver"


qtwidgets=Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -id @executable_path/../$qtwidgets $bundle/Contents/$qtwidgets

qtcore=Frameworks/QtCore.framework/Versions/5/QtCore
install_name_tool -id @executable_path/../$qtcore $bundle/Contents/$qtcore

qtgui=Frameworks/QtGui.framework/Versions/5/QtGui
install_name_tool -id @executable_path/../$qtgui $bundle/Contents/$qtgui

qtdbus=Frameworks/QtDBus.framework/Versions/5/QtDBus
install_name_tool -id @executable_path/../$qtdbus $bundle/Contents/$qtdbus

qtprintsupport=Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport
install_name_tool -id @executable_path/../$qtprintsupport $bundle/Contents/$qtprintsupport

qtsvg=Frameworks/QtSvg.framework/Versions/5/QtSvg
install_name_tool -id @executable_path/../$qtsvg $bundle/Contents/$qtsvg


install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $bundle/Contents/MacOS/castle
install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $bundle/Contents/MacOS/castle
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $bundle/Contents/MacOS/castle


install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $bundle/Contents/$qtwidgets
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $bundle/Contents/$qtwidgets

install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $bundle/Contents/$qtgui


install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $bundle/Contents/$qtprintsupport
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $bundle/Contents/$qtprintsupport
install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $bundle/Contents/$qtprintsupport

install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $bundle/Contents/$qtdbus

#echo "final dir = "
install_name_tool -change $buildDir/dragonrecipes/libdragonrecipes$ver.dylib @executable_path/../Resources/libdragonrecipes$ver.dylib $bundle/Contents/MacOS/castle

install_name_tool -id @executable_path/../Resources/libdragonrecipes$ver.dylib $bundle/Contents/Resources/libdragonrecipes$ver.dylib

cocoa=$bundle/Contents/plugins/platforms/libqcocoa.dylib
install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $cocoa
install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $cocoa
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $cocoa
install_name_tool -change @rpath/QtDBus.framework/Versions/5/QtDBus @executable_path/../$qtdbus $cocoa
install_name_tool -change @rpath/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../$qtprintsupport $cocoa

cocoa=$bundle/Contents/plugins/platforms/libqcocoa_debug.dylib
install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $cocoa
install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $cocoa
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $cocoa
install_name_tool -change @rpath/QtDBus.framework/Versions/5/QtDBus @executable_path/../$qtdbus $cocoa
install_name_tool -change @rpath/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../$qtprintsupport $cocoa


for f in $bundle/Contents/plugins/imageformats/*.dylib
do
    echo "f = $f"
    echo "f short = ${f##*/}"
    install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $f
    install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $f
    install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $f
    install_name_tool -change @rpath/QtDBus.framework/Versions/5/QtDBus @executable_path/../$qtdbus $f
    install_name_tool -change @rpath/QtSvg.framework/Versions/5/QtSvg @executable_path/../$qtsvg $f
    install_name_tool -change @rpath/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../$qtprintsupport $f
done

for f in $bundle/Contents/plugins/styles/*.dylib
do
    echo "f = $f"
    echo "f short = ${f##*/}"
    install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../$qtwidgets $f
    install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/../$qtgui $f
    install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../$qtcore $f
    install_name_tool -change @rpath/QtDBus.framework/Versions/5/QtDBus @executable_path/../$qtdbus $f
    install_name_tool -change @rpath/QtSvg.framework/Versions/5/QtSvg @executable_path/../$qtsvg $f
    install_name_tool -change @rpath/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../$qtprintsupport $f
done

