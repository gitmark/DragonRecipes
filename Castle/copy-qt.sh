#!/bin/bash

bundle=$1

if [ "$bundle" == "" ]; then
    echo "must provide a bundle dir"
    exit 1
fi

qtlib=$HOME/Qt/5.12.6/clang_64/lib
qtplugins=$HOME/Qt/5.12.6/clang_64/plugins
dest=$bundle/Contents/Frameworks

if [ -e $dest ]; then
    rm -rf $dest
fi

mkdir -p $dest

cp -R $qtlib/QtWidgets.framework $dest/
cp -R $qtlib/QtCore.framework $dest/
cp -R $qtlib/QtGui.framework $dest/

cp -R $qtlib/QtPrintSupport.framework $dest/
cp -R $qtlib/QtDBus.framework $dest/
cp -R $qtlib/QtSvg.framework $dest/

mkdir -p $bundle/Contents/plugins/platforms
cp -R $qtplugins/platforms/libqcocoa* $bundle/Contents/plugins/platforms/

cp -R $qtplugins/imageformats $bundle/Contents/plugins/
cp -R $qtplugins/styles $bundle/Contents/plugins/

