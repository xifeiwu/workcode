#!/bin/bash

# This script provides wisdom
# You can now exit in a decent way.

FORTUNE=/usr/games/fortune

while true; do
echo "On which topic do you want advice?"
echo "1.  politics"
echo "2.  startrek"
echo "3.  kernelnewbies"
echo "4.  sports"
echo "5.  bofh-excuses"
echo "6.  magic"
echo "7.  love"
echo "8.  literature"
echo "9.  drugs"
echo "10. education"
echo

echo -n "Enter your choice, or 0 for exit: "
read choice
echo

case $choice in
     1)
     $FORTUNE politics
     ;;
     2)
     $FORTUNE startrek
     ;;
     3)
     $FORTUNE kernelnewbies
     ;;
     4)
     echo "Sports are a waste of time, energy and money."
     echo "Go back to your keyboard."
     echo -e "\t\t\t\t -- \"Unhealthy is my middle name\" Soggie."
     ;;
     5)
     $FORTUNE bofh-excuses
     ;;
     6)
     $FORTUNE magic
     ;;
     7)
     $FORTUNE love
     ;;
     8)
     $FORTUNE literature
     ;;
     9)
     $FORTUNE drugs
     ;;
     10)
     $FORTUNE education
     ;;
     0)
     echo "OK, see you!"
     break
     ;;
     *)
     echo "That is not a valid choice, try a number from 0 to 10."
     ;;
esac  
done
