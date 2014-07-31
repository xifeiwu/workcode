#!/bin/bash
read -p "Input:" num
echo ${num}
num=`echo ${num} | sed "s/[^1-9]//g"`
echo "after change:${num}"
exit 0

ch=5
echo ${ch}
echo $((ch+1))
exit 0

allsteps=7
steps=1
declare -a ALLSTEPS
ALLSTEPS[0]="$((steps++))/${allsteps}. Checking current version of COS Desktop."
ALLSTEPS[1]="$((steps++))/${allsteps}. Update repositoies for Package installation."
ALLSTEPS[2]="$((steps++))/${allsteps}. Replace Packages in Component Main."
ALLSTEPS[3]="$((steps++))/${allsteps}. Upgrade Packages in Component Main."
ALLSTEPS[4]="$((steps++))/${allsteps}. Purge Packages in Component Universe."
ALLSTEPS[5]="$((steps++))/${allsteps}. Install Packages in Component Universe."
ALLSTEPS[6]="$((steps++))/${allsteps}. Other fix."
for((i=0;i<${#ALLSTEPS[@]};i++))
do
    echo ${ALLSTEPS[${i}]}
done

exit 0

declare -a menulists
menulists[0]="0. Add new repo, and update source lists."
menulists[1]="1. Install frequently used packages."
menulists[2]="2. Import gpg, Custom ssh key, bash, vim, and git config."
menulists[3]="3. git clone ccustom, cstudy from sil-15, repo sync from ibp-142."
menulists[4]="4. Install chrome, wps and sogou input method."
for((i=0;i<${#menulists[@]};i++))
do
echo ${menulists[${i}]}
done
exit 9
echo ${menulists[*]}
for list in ${menulists[*]}
do
    echo "${list}"
done
#read -p "input: " input
#echo $input
#for in in ${input}
#do
#    echo $in
#done
