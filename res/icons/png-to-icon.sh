#!/bin/bash
# Converts the png source file to a windows .ico file. 
# Requires png2ico, imagemagick and optipng.
# http://www.winterdrache.de/freeware/png2ico/data/png2ico-src-2002-12-08.tar.gz

# Name of the icon name, excl. ext.
ICONNAME=engineIcon

for cmd in png2ico convert optipng
do
	command -v $cmd >/dev/null 2>&1 || { 
		echo >&2 "I require $cmd but it's not installed. Aborting."; 
		exit 1;
	}
done

OUTPUT=""
for i in 128 64 48 32 24 16
do
   convert $ICONNAME.png -resize $ix$i "$ICONNAME$i.png"
   optipng -o5 --strip all "$ICONNAME$i.png"
   OUTPUT+=" $ICONNAME$i.png"
done
echo "Converting files to ICO";
png2ico $ICONNAME.ico $OUTPUT
echo "Cleaning up...";
rm $OUTPUT
echo "Done!";