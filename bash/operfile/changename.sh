#/bin/sh
files=`find . -maxdepth 1 -type d`
FILES=(`echo "${files[@]}" | sed "s:\./::g"`)
LENGTH=${#FILES[@]}
echo "LENGTH:${LENGTH}"
#echo "FILES:${FILES[@]}"
for ((i=0;i<${LENGTH};i++))
do
    from=${FILES[$i]}
    to=`echo ${from} | sed "s:C:c:g"`
    echo "rename from ${from} to ${to}"
    mv ${from} ${to}
done
