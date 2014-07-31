_UNDERLINE_ON=`tput smul`                       # turn on underline
_UNDERLINE_OFF=`tput rmul`                    # turn off underline
  _WINDOW_X=`tput lines`
  _WINDOW_Y=`tput cols`
echo $_WINDOW_X
echo $_WINDOW_Y
_FULL_SPACES="11111111111111111111111111"
_FULL_UNDERLINE=`echo "${_UNDERLINE_ON}${_FULL_SPACES}${_UNDERLINE_OFF}"`
echo ${_FULL_UNDERLINE}


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

cat << EOFF
Window Size: ${_WINDOW_X} / ${_WINDOW_Y}
Select => ${_UNDERLINE_ON}     ${_UNDERLINE_OFF}
${_FULL_UNDERLINE}
B) Background Text Color
F) Foreground Text Color
X) Exit
EOFF
