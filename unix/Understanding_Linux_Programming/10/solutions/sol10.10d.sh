#!/bin/sh
#
# sol10.10d.sh
#
# Version d: Uses scratch files in /tmp made by mktemp and removes
#	     them on exit.
# Version c: Only report on if a user is logged in regardless of
#	     the number of times.
#            Solution: use cut, sort -u
# Version b: Only produce output if there is something to show in
#            that category
#	     Solution: write output to a temp file.  Then use test -s
#	     to check if temp file has non-zero size
#
# Version a: Accept a list of users to watch.  The name of the
#            file with that list is a command line arg.
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

	# create a scratch file
	SCRATCH=`mktemp /tmp/watch.XXXXXX`
	CURR=`mktemp /tmp/watch.XXXXXX`
	PREV=`mktemp /tmp/watch.XXXXXX`
	trap "rm -f $SCRATCH $CURR $PREV; exit 0" 0 2 3 15

	# main loop now
	# first get initial list
	who | cut -d" " -f1 | sort -u | join - $FILE >$PREV
	while true			# true is a program: exit(0);
	do
		sleep 10				# wait a while
		who | cut -d" " -f1 | sort -u | join - $FILE > $CURR	
		# compute results into scratch fle
		comm -23 $PREV $CURR> $SCRATCH

		# and only print the file if non-empty
		if test -s $SCRATCH ; then
			echo "Logged out:"		# print header
			cat $SCRATCH			# and results
		fi

		# same logic for other one
		comm -13 $PREV $CURR > $SCRATCH	# and results
		if test -s $SCRATCH ; then
			echo "Logged in:"		# header
			cat $SCRATCH			# and results
		fi
		mv $CURR $PREV				# make now past
	done
