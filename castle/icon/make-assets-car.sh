#!/bin/bash

buildIconDir=$1

if [ "$buildIconDir" == "" ]; then
	echo "must provide a build icon dir"
	exit 1
fi


xcrun actool Assets.xcassets --compile $buildIconDir --platform macosx --minimum-deployment-target 10.14 --app-icon AppIcon --output-partial-info-plist $buildIconDir/partial.plist

