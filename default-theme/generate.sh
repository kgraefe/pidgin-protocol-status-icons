#!/bin/bash

pidgin_src=/home/kgraefe/pidgin/pidgin-2.6.3-linux
prot_png=/tmp/generate-icons-prot-tmp.png
dir=protocol-status-theme/purple/status-icon/16
theme=protocol-status-theme/purple/status-icon/theme.xml

mkdir -p ${dir}
rm ${dir}/*.png

echo '<theme 	type="pidgin status icon" 
		name="Pidgin Protocol Status Icon Theme"
		author="freakazoid"
		image="">
	<description>
		Pidgin Protocol Status Icon Theme
	</description>' >$theme

for protocol in ${pidgin_src}/pidgin/pixmaps/protocols/16/scalable/*.svg; do
	prot=$(basename $protocol .svg)
	
	# ImageMagick liefert schlechte Qualität bei SVG->PNG
	rsvg -w 22 $protocol $prot_png
	
	for status in ${pidgin_src}/pidgin/pixmaps/status/11/*.png; do
		if [ $(basename $status .png) != "log-in" -a $(basename $status .png) != "log-out" -a $(basename $status .png) != "person" -a $(basename $status .png) != "chat" ]; then
			echo Generating ${prot}-$(basename $status)...
			
			if [ $(basename $status .png) = "offline" ]; then
				convert $prot_png -colorspace Gray ${dir}/${prot}-$(basename $status)
			else
				composite -gravity SouthEast $status $prot_png ${dir}/${prot}-$(basename $status)
			fi
			
			echo '	<icon id="prpl-'$(basename $protocol .svg)'-'$(basename $status .png)'" file="'${prot}-$(basename $status)'" />' >>$theme
		fi
	done
done

echo '</theme>' >>$theme

rm $prot_png
