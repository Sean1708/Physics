#!/usr/bin/env bash

cmd="rsync -u"

for file in $@
do
    if [[ -e $file ]]
    then
        cmd="$cmd $file"
    fi
done

$cmd
