#!/bin/bash

ROOT=~/Workspace/Semantikin/

echo $ROOT

exp_succ=`find $ROOT/tests/regression/yes-compile/ -name '*.lxn'`
exp_errs=`find $ROOT/tests/regression/no-compile/ -name '*.lxn'`

echo  ""
echo "Should compile: "

	for src in ${exp_succ[@]} ; do

		$ROOT/Semantikin -i $src -dumps -dumpIRTree > /dev/null

		if [ $? -eq 0 ] ; then
			printf "[%50s] [ OK] compiled successfully.\n" `basename $src`
		else 
			printf "[%50s] [ERR] did not compile.\n" `basename $src`
		fi

	done


echo  ""
echo "Should NOT compile: "

	for src in ${exp_errs[@]} ; do

		$ROOT/Semantikin -i $src -dumps -dumpIRTree > /dev/null

		if [ $? -eq 0 ] ; then
			printf "[%50s] [ERR] compiled successfully.\n" `basename $src`
		else 
			printf "[%50s] [ OK] did not compile.\n" `basename $src`
		fi

	done

