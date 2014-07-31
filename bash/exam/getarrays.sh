declare -i count
count=0
#echo "result:`find . -perm 664 | xargs echo`"
for loop in `find . -perm 664 | xargs echo`
do
count=`expr $count + 1`
echo "$count:$loop"
done
