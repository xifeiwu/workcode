#!/bin/bash
#myname="Sheller man"
#substr=${myname:4}
#echo $substr

#echo $0
#para=${@:1}
#echo ${para[@]}
#para_arr=($para)
#echo ${para_arr}
#echo ${#para_arr[@]}


#files=$(ls)
#files_arr=($files)
#for file in $files
#do
#    echo $file
#done
#echo ${#files_arr[@]}

files=$(ls)
files_arr=($files)
select file in $files
do
    echo $file
    break
done
echo the file you selected is $file
echo ${#files_arr[@]}
