#!/usr/bin/env bash

countPassed=0
count=0

RED='\033[0;31m'
GREEN='\033[0;92m'
BLUE='\033[0;96m'
PURP='\033[0;95m'
NC='\033[0m' # No Color

function compare()
{
    if cmp "$1" "$2"; then
    	echo "Equal"
    else
    	echo "Not Equal"
    fi
}

function test()
{
    FILESIZE1=$(stat -c%s Data/"$1".txt)
    printf "${GREEN}TESTING:${NC}"
    printf "${PURP}$1 test: ${NC} \n"
    let "count+=1"
    bash haffman.sh Data/"$1".txt -o Data/"$1".haf -mode code
    FILESIZE=$(stat -c%s Data/"$1".haf)
    bash haffman.sh Data/"$1".haf -o Data/"$1"-decode.txt -mode decode
    res=$(compare Data/"$1".txt Data/"$1"-decode.txt)
    if [ "$res" == "Equal" ] ; then
        printf "Initially size:  $FILESIZE1 \nCompressed size: $FILESIZE \n"
        printf "${GREEN}[ OK ] ${BLUE}$1 test${NC} \n"
        let "countPassed+=1"
    else
        printf "${RED}[ Fail ]${NC} \n"
    fi
}

function test1()
{
    FILESIZE1=$(stat -c%s Data/"$1".txt)
    printf "${GREEN}TESTING:${NC}"
    printf "${PURP}$1 triple compression test: ${NC} \n"
    let "count+=1"
    bash haffman.sh Data/"$1".txt -o Data/"$1".haf -mode code
    bash haffman.sh Data/"$1".haf -o Data/"$1"1.haf -mode code
    bash haffman.sh Data/"$1"1.haf -o Data/"$1"2.haf -mode code
    FILESIZE=$(stat -c%s Data/"$1"2.haf)
    bash haffman.sh Data/"$1"2.haf -o Data/"$1"1-decode.haf -mode decode
    bash haffman.sh Data/"$1"1.haf -o Data/"$1"-decode.haf -mode decode
    bash haffman.sh Data/"$1".haf -o Data/"$1"-decode.txt -mode decode
    res=$(compare Data/"$1".txt Data/"$1"-decode.txt)
    if [ "$res" == "Equal" ] ; then
        printf "Initially size:  $FILESIZE1 \nCompressed size: $FILESIZE \n"
        printf "${GREEN}[ OK ] ${BLUE}$1 test${NC} \n"
        let "countPassed+=1"
    else
        printf "${RED}[ Fail ]${NC} \n"
    fi
}

echo "==========Testing==========="

time=$(date +%s%N | cut -b1-13)

test a
#test war-and-peace
test orwell
test empty
test abrakadabra
test aaaaaa
test ababab
#test Haffman1
#test Wavebox_3_10_0_linux_x86_64

test1 aaaaaa
test1 ababab

echo "============================"
echo "Tests passed in $(($(date +%s%N | cut -b1-13) - $time)) miliseconds"
echo "Test passed: ""$countPassed"" from: ""$count"
