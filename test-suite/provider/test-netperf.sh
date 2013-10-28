#!/bin/bash

NUMIT=${1-10}
SAVE_DIR=${2-results}

TSTAMP=$(date +"%y%m%d.%H%M%S")
FOLDER="$SAVE_DIR/$TSTAMP"
FILENAME="temp_results.out"
mkdir -p $FOLDER

for i in $(seq 1 $NUMIT); do
	echo "*** ITERATION $i OF $NUMIT ***"
	sleep 1
	result="$(netperf -H local1 | tail -n 1)"
	echo $result >> $FOLDER/$FILENAME
	echo $result
done

read AVG VAR STD MAX MIN <<<$(awk '
        {
		if (min == "") {
			min = max = $5
		}
		if ($5 > max) {
			max = $5
		}
		if ($5 < min) {
			min = $5
		}
                sum += $5
                sum_square += $5*$5
        }
        END {
                print sum/(NR),
                ((sum_square-((sum*sum)/(NR)))/(NR-1)),
                sqrt((sum_square-((sum*sum)/(NR)))/(NR-1)),
		max,
		min
        }' $FOLDER/$FILENAME)

echo "Avg: $AVG - Var: $VAR - StdDev: $STD"
echo "Peak: $MAX - Min: $MIN"
