#!/bin/bash
echo get $# parameters.
echo $@
#for para in $@
#do
#    echo $para
#done

#parameters=($@)
#echo 
#echo command: $0
#echo parameters:
#for((i=0;i<$#;i++))
#do
#    echo $i: ${parameters[$i]}
#done


while [ -n "$1" ]
do
	case "$1" in
	-a) echo Found the -a option;;
	-b) 
	param="$2"
	echo Found the -b option, with parameter value $param
	shift
	;;
	--)
	shift;
	break
	;;
	*)
	echo $1 is not an option.
	;;
	esac
	shift
done
count=1
echo $@
for param in "$@"
do
	echo "Parameter #$count: $param"
	count=$[$count+1]
done


#while getopts u:ah opt
#do
#    echo $opt
#    case $opt in
#    u]
#        echo "提供了选项u和参数: $OPTARG"
#    ;;
#    a]
#        echo "提供了选项a"
#    ;;
#    h]
#        echo "提供了选项h"
#    ;;
#    *]
#        echo "others"
#    esac
#done
