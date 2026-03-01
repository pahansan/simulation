#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <tree/graph> <N Vertexes> <Max Degree>"
    exit 1
fi

./app "$1" "$2" "$3" > draw_tree.sh
./draw_tree.sh
