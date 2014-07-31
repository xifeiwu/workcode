#!/bin/bash

_UNDERLINE_ON=`tput smul`                      # turn on underline
_UNDERLINE_OFF=`tput rmul`                    # turn off underline

get_window_size() {
  _WINDOW_X=`tput lines`
  _WINDOW_Y=`tput cols`
  
  declare -i _SPACES=${_WINDOW_Y}
  _FULL_SPACES=`awk '
  BEGIN{
    total="'$_SPACES'"
    while(total-- > 0)
    {
    printf " "
    }
  }'`
  _FULL_UNDERLINE=`echo "${_UNDERLINE_ON}${_FULL_SPACES}${_UNDERLINE_OFF}"`
  #echo "$_FULL_UNDERLINE"
  unset _FULL_SPACES
  show_menu
  return 0
  }
set_color() {
  tput clear
  PS3="Enter Selection[1-9]:"
  select _COLOR in "Black" "Blue" "Green" "Cyan" "Red" "Magenta" "Yellow" "White" "Exit"
  do
    case ${REPLY} in
       [1-8])  _X=`expr ${REPLY} - 1`;;
           9)  break;;
           *)  echo "Invalid Color"; continue;;
    esac

    if [[ ${1} = "b" ]]
    then
      tput setb ${_X}
    else
      tput setf ${_X}
    fi
  done
}

show_menu() {
  while [[ -z ${_ANS} ]]
  do
    #tput civis
    tput clear

cat << EOFF
窗口大小（宽*高）: ${_WINDOW_Y} * ${_WINDOW_X}
${_FULL_UNDERLINE}
B) Background Text Color
F) Foreground Text Color
X) Exit
EOFF

    tput smul
    #tput cnorm

    read _ANS
    tput rmul

    case ${_ANS} in
      [Bb])  set_color "b";;
      [Ff])  set_color "f";;
      [Xx])  tput clear; exit;;
         *)
             echo -e "Invalid Selection: ${_ANS}\c"
             sleep 2
             ;;
    esac
    unset _ANS
  done
}
tput cup 10 3
[[ -n ${_ANS} ]] && unset _ANS
get_window_size




