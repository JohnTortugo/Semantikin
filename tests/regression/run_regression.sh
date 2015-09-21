#!/bin/bash

ROOT=../..

exp_succ=`find $ROOT/tests/regression/yes-compile/ -name *.lxn`
exp_errs=`find $ROOT/tests/regression/no-compile/ -name *.lxn`

echo  ""
echo "Should compile: "

	for src in ${exp_succ[@]} ; do

		$ROOT/Semantikin -i $src > /dev/null

		if [ $? -eq 0 ] ; then
			printf "[%70s] [OK ] compiled successfully\n" $src
		else 
			printf "[%70s] [ERR] did not compile.\n" $src
		fi

	done


echo  ""
echo "Should NOT compile: "

	for src in ${exp_errs[@]} ; do

		$ROOT/Semantikin -i $src > /dev/null

		if [ $? -eq 0 ] ; then
			printf "[%70s] [ERR] compiled successfully\n" $src
		else 
			printf "[%70s] [OK ] did not compile.\n" $src
		fi

	done

