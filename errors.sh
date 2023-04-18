#!/bin/bash
if ! test $# -eq 2
then
	echo "wrong number of arguments; try bash script.sh file.c redirectErrors.txt"
	exit 1
fi

if ! test -f $1 || test -L $1
then
	echo "not a regular file"
	exit 2
fi

if ! [[ $1 == *.c ]]
then
	echo "not a C file"
	exit 3
fi

gcc -Wall "$1" 2> "$2" 
error_count=$(grep -c "error" "$2")
warning_count=$(grep -c "warning" "$2")
 
echo "number of errors: $error_count"
echo "number of warnings: $warning_count"

