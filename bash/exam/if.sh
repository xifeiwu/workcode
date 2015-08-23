if test date; then
    echo date is useable.
else
    echo is not useable.
fi

echo
echo
echo

if 0; then
    echo 0
else
    echo 1
fi

echo
echo
echo

var=10
while [ $var -ge 0 ];
do
    echo $var
    var=$[$var-1]
done > test.out
