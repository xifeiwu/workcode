echo "number of parameters:$#"
argc=$#
argv=($@)
echo $argc ${argv[@]}
for((i=0;i<=argc;i++))
do
    echo $i:${argv[$i]}
done

echo ===============================
echo $@
echo $*
echo "zero:$0"
echo "one:$1"
echo "two:$2"
echo "three:$3"
echo "four:$4"
echo "five:$5"
