declare -i count
count=0
while [ $count -lt "10" ]
do
echo "count:${count}"
sleep 3
count=`expr $count + 1`
done

