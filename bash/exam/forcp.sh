declare -i count
count=0
for loop in $*
do
count=`expr ${count} + 1`
echo "$count:$loop."
done
