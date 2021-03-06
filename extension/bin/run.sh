#!/bin/bash
cd bin
echo
echo "Hello"

cd ..
echo
echo "Now compiling"
make


while true
do

echo
echo

echo "Please input a number(1~6) to run a test, Ctrl-d to exit:"
echo " 		1 for test1.c -- test struct type"
echo " 		2 for test2.c -- test pointer type"
echo " 		3 for test3.c -- test function pointer"
echo " 		4 for sort.c  -- use different compare function to sort an array of struct pointers"
echo " 		5 for type.c  -- print types"

read choice

case $choice in
	1)
		bin/compiler test/test1.c 
		llc -filetype=obj test1.ll -o bin/test1.o
		clang bin/test1.o bin/libexternfunc.so -o test1
		./test1
		;;
	2)
		bin/compiler test/test2.c 
		llc -filetype=obj test2.ll -o bin/test2.o
		clang bin/test2.o bin/libexternfunc.so -o test2
		./test2
		;;
	3)
		bin/compiler test/test3.c 
		llc -filetype=obj test3.ll -o bin/test3.o
		clang bin/test3.o bin/libexternfunc.so -o test3
		./test3
		;;
	4)
		bin/compiler test/sort.c 
		llc -filetype=obj sort.ll -o bin/sort.o
		clang bin/sort.o bin/libexternfunc.so -o sort
		./sort
		;;
	5)
		bin/compiler  -t test/type.c 
		;;

	*)
		echo $choice: unknown option
		exit 1
		;;
esac


done

