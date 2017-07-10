#!/bin/bash

while :
do
    echo "DIR is $PWD"
    server = $(ps -ef |grep "$PWD/build/server" |grep -v "grep")
    if ["$server"] ; then
        echo "server was already started!"
    else
        echo "server was not started!"
        echo "starting server..."
        $PWD/build/server
        echo "server was already exited!"
    fi

    sleep 5
done
