#!/bin/sh
#
# sol10.10a.sh
# A version of watch.sh that takes a list of watchees from a file
# on the command line.  If no file name is provided, the program
# looks for $HOME/.watchees
#
# note: the list of users has to have one name per line
#
#
	# set up files

	FILE=$1
	if test -z "$FILE" 
	then
		if test ! -f $HOME/.watchees 
		then
			echo "usage: watch.sh userlist"
			exit 1
		fi
	fi
	NAMES=`wc -w < $FILE`
	LINES=`wc -l < $FILE`
	if test $NAMES -ne $LINES ; then
		echo "file $FILE must have one name per line"
		exit 2
	fi

	# file has to be sorted, because we plan to use join
	sort -o $FILE $FILE

	# main loop now
	who | sort | join - $FILE >prev	# get initial user list
	while true			# true is a program: exit(0);
	do
		sleep 10				# wait a while
		who | sort | join - $FILE > current	# get current user list
		echo "Logged out:"			# print header
		comm -23 prev current			# and results
		echo "Logged in:"			# header
		comm -13 prev current   		# and results
		mv current prev				# make now past
	done
