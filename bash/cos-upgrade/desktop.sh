#!/bin/sh
files2replace=(baobab
bluetooth-sendto
bluetooth-wizard
caribou
cinnamon-control-center
cinnamon-network-panel
cinnamon-settings
cinnamon-sound-nua-panel
cinnamon-user-accounts-panel
evince
fcitx
firefox
gcalctool
gcr-prompter
gedit
gnome-disk-image-mounter
gnome-disks
gnome-screenshot
gnome-system-log
gnome-system-monitor
gnome-system-monitor-kde
gnome-terminal
gparted
gthumb
gufw
mintBackup
mintBackup_mime
mintdrivers
mintInstall
mintInstall_kde
mintInstall_mime
mintsources
mintUpdate
mono-runtime
mono-runtime-terminal
ndisgtk-kde
nemo-autorun-software
nemo
nm-applet
nm-connection-editor
session-properties
synaptic
system-config-printer
tomboy
totem
ubiquity-gtkui
vino-preferences
yelp)
files2remove=(aptoncd
cinnamon-universal-access-panel
fcitx-config-gtk3
fcitx-configtool
file-roller
gdebi
gnome-font-viewer
gnome-power-statistics
gnome-user-share-properties
gucharmap
im-config
itweb-settings
mintNanny
mintstick
mintstick-kde
mintWelcome
ndisgtk
openjdk-7-policytool
seahorse
simple-scan
synaptic-kde
upload-manager
xchat
)
mkdir -p ~/applications
echo $?
for file in ${files2replace[@]}; do
    if [ -f applications/${file}.desktop ]; then
        cp applications/${file}.desktop ~/applications
        echo "cp applications/${file}.desktop to ~/applications"
    else
        echo "applicatios/${file}.desktop does not exist."
    fi
done
#cat ../change_start_menu.sh | sed -n "s/.*\/tmpfiles\/applications\/\(.*\)\.desktop \./\1/p" | wc -l
#cat ../change_start_menu.sh | sed -n "s/rm -f \(.*\)\.desktop.*/\1/p"
