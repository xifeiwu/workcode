#!/bin/sh
pkglists=("mint-artwork-cinnamon" "mint-backgrounds-lisa" "mintdrivers"   "mint-mdm-themes"       "mintsources" \
"mint-artwork-common"   "mint-backgrounds-lisa-extra"   "mint-flashplugin"      "mintmenu"      "mintstick" \
"mint-artwork-gnome"    "mint-backgrounds-maya" "mint-flashplugin-10.2" "mint-meta-cinnamon"    "mint-stylish-addon" \
"mint-artwork-kde"      "mint-backgrounds-maya-extra"   "mint-flashplugin-10.3" "mint-meta-codecs"      "mintsystem" \
"mint-artwork-kde-icons"        "mint-backgrounds-nadia"        "mint-flashplugin-11"   "mint-meta-codecs-kde"  "mint-themes" \
"mint-artwork-mate"     "mint-backgrounds-nadia-extra"  "mint-info-cinnamon"    "mint-meta-core"        "mint-translations" \
"mint-artwork-xfce"     "mint-backgrounds-olivia"       "mint-info-kde"         "mint-meta-kde"         "mintupdate" \
"mint-backgrounds-helena"       "mint-backgrounds-xfce" "mint-info-mate"        "mint-meta-mate"        "mintupload" \
"mint-backgrounds-isadora"      "mintbackup"    "mint-info-xfce"        "mint-meta-xfce"        "mintwelcome" \
"mint-backgrounds-julia"        "mint-common"   "mintinstall"   "mint-mirrors"  "mintwifi" \
"mint-backgrounds-katya"        "mint-configuration-kde"        "mintinstall-icons"     "mintnanny"     "mint-x-icons" \
"mint-backgrounds-katya-extra"  "mintdesktop"   "mint-local-repository" "mint-search-addon")
lists=(mint-artwork-cinnamon         mint-backgrounds-lisa         mintdrivers                   mint-mdm-themes               mintsources
mint-artwork-common           mint-backgrounds-lisa-extra   mint-flashplugin              mintmenu                      mintstick
mint-artwork-gnome            mint-backgrounds-maya         mint-flashplugin-10.2         mint-meta-cinnamon            mint-stylish-addon
mint-artwork-kde              mint-backgrounds-maya-extra   mint-flashplugin-10.3         mint-meta-codecs              mintsystem
mint-artwork-kde-icons        mint-backgrounds-nadia        mint-flashplugin-11           mint-meta-codecs-kde          mint-themes
mint-artwork-mate             mint-backgrounds-nadia-extra  mint-info-cinnamon            mint-meta-core                mint-translations
mint-artwork-xfce             mint-backgrounds-olivia       mint-info-kde                 mint-meta-kde                 mintupdate
mint-backgrounds-helena       mint-backgrounds-xfce         mint-info-mate                mint-meta-mate                mintupload
mint-backgrounds-isadora      mintbackup                    mint-info-xfce                mint-meta-xfce                mintwelcome
mint-backgrounds-julia        mint-common                   mintinstall                   mint-mirrors                  mintwifi
mint-backgrounds-katya        mint-configuration-kde        mintinstall-icons             mintnanny                     mint-x-icons
mint-backgrounds-katya-extra  mintdesktop                   mint-local-repository         mint-search-addo)
for pkg in ${lists[*]}; do
    dpkg -s ${pkg} &> /dev/null && result=0 || result=1
    if [ ${result} == 0 ] ; then
        echo ${pkg}
    fi
done

