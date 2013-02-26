#!/bin/bash

/usr/bin/time -f "The time it takes to process one character at a time is %E" ./onechar < ../shakespearetradgedies.txt > /dev/null
/usr/bin/time -f "The time it takes to process one line at a time is %E" ./oneline < ../shakespearetradgedies.txt > /dev/null
