#!/bin/sh

B_REALPATH=`readlink "$0"`

if [ "$B_REALPATH" = "" ]; then
B_REALPATH="$0"
fi

cd $(dirname "$B_REALPATH")


if [ "$1" == "-v" ] || [ "$1" == "--version" ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
        ./castle "$@"
        exit 0
fi


cd ../../..
open -a $PWD/Castle.app --args "$@"



