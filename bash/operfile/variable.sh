#/bin/sh
function varfunc()
{
    echo "dirs in function:"
    echo "$1"
}
DIRPATH=~
dirs=`cd ${DIRPATH}; find . -maxdepth 1 -type d`
echo "dirs before function:${dirs}"
varfunc "${dirs}"
