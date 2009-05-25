#!/bin/bash

pidgin_src=/home/kgraefe/pidgin/pidgin-2.5.6-linux
prot_png=/tmp/generate-icons-tmp.png
theme=theme.xml

echo '<?xml version="1.0" encoding="UTF-8"?>' >$theme
echo '<theme name="Default Theme" version="1.0" author="Konrad Gräfe &lt;freakazoid@teamblind.de&gt;" website="http://freakazoid.teamblind.de/">' >>$theme



for protocol in ${pidgin_src}/pidgin/pixmaps/protocols/16/scalable/*.svg; do
	dir=$(basename $protocol .svg)
	mkdir -p $dir
	
	echo '	<protocol prpl-id="'$(basename $protocol .svg)'">' >>$theme
	
	# ImageMagick liefert schlechte Qualität bei SVG->PNG
	rsvg -w 12 $protocol $prot_png
	
	for status in ${pidgin_src}/pidgin/pixmaps/status/22/*.png; do
		if [ $(basename $status .png) != "log-in" -a $(basename $status .png) != "log-out" -a $(basename $status .png) != "person" ]; then
			echo Generating ${dir}/$(basename $status)...
			composite -gravity SouthEast $prot_png $status ${dir}/$(basename $status)
			
			echo '		<status name="'$(basename $status .png)'" file="'${dir}/$(basename $status)'" />' >>$theme
		fi
	done
	
	echo '	</protocol>' >>$theme
done

echo '</theme>' >>$theme

rm $prot_png