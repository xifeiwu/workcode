MYDATE=`date +%d%m%Y`
HOST=`hostname -s`
USER=`whoami`

while :
do
cat << MAYDAY
--------------------------------------------------------------
User: $USER		Host:$HOST		Date:$MYDATE
--------------------------------------------------------------
		1: List files in current Directory
		2: Use the vi editor
		3: See who is on the system
		H: Help screen
		Q: Exit menu
--------------------------------------------------------------
MAYDAY
echo -e -n "\tYour Choice [1,2,3,H,Q] >"
read ANS
case $ANS in
1)
ls
;;
2)
vi
;;
3)
who
;;
H|h)
cat << MAYDAY
This is the help screen, nothing here yet to help you!
MAYDAY
;;
Q|q)
exit 0
;;
*)
echo -e "\t\007Unknown user response."
;;
esac
done
