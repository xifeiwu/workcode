#!/bin/sh
#get content of .deb file
function get_content()
{
    content=`dpkg -c $1 | awk 'BEGIN {s=6} 
{
for(i=s; i <=NF; i++)
{
    if(match($i, /\./))
    {
        sub(/\./, "", $i) ; print $i ; break 
    }    
}
}'`
    echo $content
}
package=
MAX=100
if [ -z $1 ]; then
    echo "Input the package you want to find after command."
    exit ${MAX}
else
    package=$1
    dpkg -s ${package} > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "package ${package} is not found in you system."
        exit ${MAX}
    fi
fi
#find all \.[u]*deb file in current directory
deb_fullname=(`ls | grep '\.[u]*deb'`)
echo "The deb package found in current dir as below:"
ls | grep '\.[u]*deb'
deb_length=${#deb_fullname[*]}
if [ ${deb_length} -eq "0" ]; then
    echo "The deb package founded is zero, exit."
    exit ${MAX}
fi
if [ ${deb_length} -gt ${MAX} ]; then
    echo "The length of deb array is greater than ${MAX}, resize variable MAX in script."
    exit ${MAX}
fi
echo
echo "Look through files in backend......"
#change deb_fullname to deb_array_name
#all files in deb are contained in its correspondding array, whose name is contained in deb_array_name
declare -a deb_array_name
for((i=0; i<${deb_length}; i++))
do
    name=${deb_fullname[$i]}
    name=`echo ${name%%_*} | sed 's/\-/_/g'`
    deb_array_name[$i]=${name}
done
#echo "deb_array_name[${#deb_array_name[*]}] : ${deb_array_name[*]}" && exit 0 || exit 1

for((i=0; i<${deb_length}; i++))
do
    full_name=${deb_fullname[$i]}
    array_name=${deb_array_name[$i]}
    eval ${array_name}=\(`get_content ${full_name}`\)
#    eval echo ${array_name} : \${#${array_name}[@]}
done
#look through all .deb files to find the one contain file $1
function look_through()
{
    local file
    for((i=0; i<${deb_length}; i++))
    do
        array_name=${deb_array_name[$i]}
        eval files=\${${array_name}[@]}
        for file in $files
        do
            if [ ${file} == $1 ]; then
                return $i
            fi
        done
    done
    return ${MAX}
}
if [ ! -d ./results ]; then
    mkdir ./results
fi
echo "">./results/${package}-find.all
echo "">./results/${package}-find.notfound
for file in `dpkg -L ${package}`
do
    if [ -d ${file} ]; then
        printf "%-80s\t%s\n" "$file" "is a directory" >> ./results/${package}-find.all
        continue
    fi
    look_through ${file}
    pos=$?
    if [ ${pos} == ${MAX} ]; then
        printf "%-80s\t%s\n" "$file" "file not found" >> ./results/${package}-find.all
        printf "%-80s\t%s\n" "$file" "file not found" >> ./results/${package}-find.notfound
    else        
        printf "%-80s\t%s\n" "$file" "${deb_fullname[${pos}]}" >> ./results/${package}-find.all
    fi
done

echo "Result is stored in ./results/${package}-find.all and ./results/${package}-find.notfound"
