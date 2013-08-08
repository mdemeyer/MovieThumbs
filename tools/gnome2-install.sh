#!/bin/bash

# Original script by Jesus Alvarez
# https://github.com/demizer/binfiles

VIDEO_EXTENSIONS="video@flv video@webm video@mkv video@mp4 video@mpeg \
video@avi video@ogg video@quicktime video@x-avi video@x-flv video@x-mp4 \
video@x-mpeg video@x-webm video@x-mkv application@x-extension-webm \
video@x-matroska video@x-ms-wmv video@x-msvideo video@x-msvideo@avi \
video@x-theora@ogg video@x-theora@ogv video@x-ms-asf video@x-m4v \
video@vivo video@vnd-divx video@vnd.rn-realvideo video@x-anim video@x-flc \
video@fli video@flic video@asx video@x-ms-wm video@x-ms-wmx video@x-ms-wvx \
video@x-nsv video@x-ogm@ogg video@vnd-vivo video@msvideo video@mp4v-es \
video@fli video@dv video@3gpp"

THUMBNAIL_COMMAND="moviethumbs -i %i -o %o"

for i in $VIDEO_EXTENSIONS; do
    gconftool-2 -s "/desktop/gnome/thumbnailers/$i/command" -t string "$THUMBNAIL_COMMAND"
    gconftool-2 -s "/desktop/gnome/thumbnailers/$i/enable" -t boolean 'true'
done
