loop=0
while [ $# -ne 0 ]
do
loop=`expr $loop + 1`
echo "$loop:$#:$1"
shift
done
