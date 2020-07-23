#!/bin/bash

logfile="test.log"
trap "rm $logfile" EXIT

for test in bin/c/test_*; do
    echo "Running $(basename $test)"
    valgrind --leak-check=full $test > $logfile 2>&1
    output=$?
    grep -q 'ERROR SUMMARY: 0' $logfile
    errors=$?
    ! grep -q 'Assertion' $logfile
    assertion=$?
    if [ "$output" -ne 0 ] || [ "$errors" -ne 0 ] || [ "$assertion" -ne 0 ]; then
        echo "FAILED"
        cat test.log
        exit 1
    else
        echo "PASS"
    fi
done

exit 0
