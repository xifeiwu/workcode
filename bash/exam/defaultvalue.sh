#!/bin/bash

MSG=${1:?"必须输入一个参数"}
#MSG=${1:-"testmsg.txt"}

echo MSG:$MSG

cat > $MSG <<HERE
from: im@example.edu.cn
to: yr@example.edu.cn
Subject:Test
这是一封测试信，请勿回复。
HERE
