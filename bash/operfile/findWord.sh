argc=$#
argv=($@)
if [ $argc -lt 2 ]; then
    echo Parameter Less Than Two.
    echo First is file type, Others is key word.
    exit 1
fi

count=2
grepkey=""
for((i=1;i<argc;i++))
do
    grepkey="${grepkey} grep ${argv[$i]}"
done

echo find . -name \"${argv[0]}\"
echo $grepkey

for file in  `find . -name ${argv[0]}`
do
    cat $file | $grepkey
    if [ $? -eq 0 ]; then
        echo -e "\033[2;31mFound in file: $file\033[0m"
    fi
done
