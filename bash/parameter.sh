#!/bin/sh
echo $0
while [ $# -gt 0 ]
do
    case $1 in
    "--set-step")
        shift
        args=($@)
        break
        ;;
    *)
        shift
        echo "wrong parameter."
        ;;
    esac
done
echo ${args[@]}
for arg in ${args[@]}
do
    echo ${arg}
done
