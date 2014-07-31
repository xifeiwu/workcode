#!/bin/bash
cd ./initrd.d 
find . | cpio -o -H newc | gzip -c > ../initrd.gz
if [ "$?" == "0" ]; then
    echo "modify initrd success."
fi
cd ..
mv initrd.gz /boot/initrd.img-3.8.13.13-cos-v0.9-i686
if [ "$?" == "0" ]; then
    echo "move initrd.gz success"
fi
read -p "reboot now[Y/n]?" $yn
echo "what your input is $yn"
if [ "$yn" == "y" ]; then
    reboot
fi
