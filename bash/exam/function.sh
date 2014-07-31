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

showfile / /home function.sh
