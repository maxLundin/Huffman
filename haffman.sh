#!/usr/bin/env bash
file=true
outputfile="output.txt"
inputfile="input.txt"
needhelp=false
mode=code
stat=false
rev=false

while [ $# -gt 0 ]
do
    case "$1" in
        -mode)
            shift
            mode="$1"
            shift
            ;;
        -o)
            shift
            file=true
            outputfile="$1"
            shift
            ;;
        -stat)
            stat=true
            shift
            ;;
        -order=REVERSE)
            rev=true
            shift
            ;;
        -test)
            bash testHaff.sh
            exit 0
            ;;
        --help|-h|-?)
            needhelp=true
            shift
            ;;
        *)
            if "$file"; then
                inputfile="$1"
                file=false
                shift
            else
                echo "wrong argument"
                echo "$1"
                exit 1
            fi
            ;;
	esac
done



if "$needhelp" ; then
	./Help
else
    ./Haffman "$outputfile" "$mode" "$inputfile" "$stat" "$rev"
fi
