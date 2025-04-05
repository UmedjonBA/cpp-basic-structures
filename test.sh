
#! /usr/bin/env bash
if [[ $1 == "master" ]]
then
    echo "No testing on master branch"
else
    cp -R cpp-basic-structures/$1/. $1/
    cd $1
    bash test.sh $1
fi
