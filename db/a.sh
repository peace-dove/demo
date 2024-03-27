#!/bin/bash

solution=/home/bxj/db/solution

points=(10 15 15 20 20)

function check {
    result=$(timeout 30 diff <(psql -d dblab -f $1 -t --csv 2>> $4) <(psql -d dblab -f $2 -t --csv))
    status=$?
    if ! [[ -s $4 ]]; then
        rm $4
    fi
    if [[ $status -ne 0 ]]; then
        echo "$result" >> $3
        return 1
    fi
    return 0
}

function grade() {
    id=$(basename $1 .zip)
    report="$2/$id.txt"
    submission="submissions-$id"
    # remove old results
    rm $report 2> /dev/null
    rm -r $submission 2> /dev/null
    rm -r __MACOSX 2> /dev/null
    # skip junks
    unzip -j $1 -d $submission -x "*_*" > /dev/null 2>&1
    fail_count=0
    score=0
    for i in {1..5}; do
        sub="$submission/$i.sql"
        ref="$solution/$i.sql"
        if [[ -e $sub ]]; then
                check $sub $ref "$submission/diff-$i.log" "$submission/error-$i.log"
            if [[ $? -ne 0 ]]; then
                echo "[$id] $i.sql failed" >> $report
                fail_count=$((fail_count+1))
            else
                score=$((score+points[i]))
                rm $sub
            fi
        else
            echo "[$id] $i.sql not found!" | tee -a $report
            fail_count=$((fail_count+1))
        fi
    done
    # score=$((100-10*fail_count))
    echo "[$id] score: $score" | tee -a $report
    rmdir $submission 2> /dev/null
    rm -r __MACOSX 2> /dev/null
}

rm -r grading_report
mkdir grading_report
for file in *.zip; do
    grade $file "grading_report"
done