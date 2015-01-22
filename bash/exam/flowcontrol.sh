echo "Enter a list of names:"
read list
if echo $list | grep "xifei"; then
echo "xifei is here."
exit 0
else
echo "xifei is not here"
exit 5
fi
