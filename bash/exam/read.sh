echo -n "Enter Your Name:"
read name
echo "Hello $name, welcom to program."
#echo -n "input your name:"
#read first second
#echo "first:$first"
#echo "second:$second"
#echo END

while [[ $name != "quit" ]]
do
    read -p "what is your name?" name
    echo My Name is $name.
done
