#!/bin/bash

if [ $# -lt 1 ] ; then
        echo "Supply a sound mode as argument: 8bit or 16bit"
        exit 1
fi

if [ "$1" == "8bit" ] ; then
	src="Makefile_mo"
elif [ "$1" == "16bit" ] ; then
	src="Makefile_hr"
else
	echo "Unknown sound mode '$1'. Valid options: 8bit or 16bit"
	exit 1
fi

echo "Switching to sound mode $1. Makefile will link to $src"

rm Makefile

ln -s $src Makefile

exit 0
