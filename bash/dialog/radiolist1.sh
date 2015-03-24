#!/bin/bash

TMP="/tmp/radiolist.$$"
M1="请选择您偏爱的文件系统"
NUMFS=3
FSLIST="EXT2 第2版的扩展文件系统 off ext3 第3版的扩展文件系统 on ext4 第4版的扩展文件系统 off"

/usr/bin/dialog --radiolist "$M1" 10 40 $NUMFS $FSLIST 2> $TMP

FSTYPE=$(cat $TMP)
[ -z "#FSTYPE" ] && FSTYPE="ext3"

rm -f $TMP

echo "您的选择是：$FSTYPE"
