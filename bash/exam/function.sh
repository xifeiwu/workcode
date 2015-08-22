function showfile()
{
for loop in $@
do
if [ -f $loop ]; then
echo -n -e "$loop is a file, the contents of the file is listed below:\n"
echo -n -e "`cat ${loop}`\n"
fi
if [ -d $loop ]; then
echo -n -e "$loop is a directory, the contents of directory is listed below:\n"
echo -n -e "`ls ${loop}`\n"
fi
done
}
#showfile / /home functiuon.sh

function db1(){
    read -p "Enter a value: " value
    echo "doubling the value."
    return $[$value*2]
}
#db1
#echo "The new Value is $?"

function testit {
    local newarray
    newarray=($@)
    echo "The new array value is: ${newarray[*]}"
    echo "new array size: ${#newarray[@]}"
}
#myarray=(1 2 3 4 5)
#echo "The original array is ${myarray[*]}"
#testit ${myarray[*]}

function arraydblr {
    echo my
    echo result 
}
result=`arraydblr`
echo $result
