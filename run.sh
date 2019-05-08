#!/bin/bash

SEPARATOR=' '
GRAPHS=("test.adjl" "dijkstra.adjl" "task-graphs/jonathan-graphs/AP1/AP1_prog.txt")
STOP_CRITERION=(100 250 500 1000 2000)
NUM_NODE_MULT=(0.5 1 2 3 4 5)

sc_count=1
for sc in ${STOP_CRITERION[*]}; do
	graph_count=1
	for graph in ${GRAPHS[*]}; do
		ghead=$(head -1 $graph)
		nodeindex=$(expr index "$ghead" ' ')
		numnodes=${ghead:0:$nodeindex}
		numedges=${ghead:nodeindex}

		TABU_LIST_SIZES=(1)
		for i in ${NUM_NODE_MULT[*]}; do
			TABU_LIST_SIZES+=($(echo "scale=0; (${numnodes}*${i})/1" | bc))
		done

		tlsize_count=1
		for tlsize in ${TABU_LIST_SIZES[*]}; do
			epsilon_count=1
			for (( epsilon=$numnodes; epsilon<=$numnodes*2; epsilon++ )); do
				exec_count=1
				for i in {1..30}; do
					echo "=============================================================================="
					echo "Stop Criterion:" $sc '('$sc_count'/'${#STOP_CRITERION[*]}')'
					echo "Graph: "$graph '('$graph_count'/'${#GRAPHS[*]}')'
					echo "Tabu List Size:" $tlsize '('$tlsize_count'/'${#TABU_LIST_SIZES[*]}')'
					echo "Epsilon:" $epsilon '('$epsilon_count'/'$((numnodes+1))')'
					echo "Execution:" $exec_count '('$exec_count'/'30')'
					echo "------------------------------------------------------------------------------"
					otpt=$(echo `expr "$graph" : '\(\(\([^/]\)*/\)*\)'`)
					pos=${#otpt}
					otpt=${graph:pos}

					./gftinoc $graph ' ' $epsilon $tlsize $sc $otpt

					echo "=============================================================================="
					echo -e "\n\n\n"
					((exec_count++))
				done
				((epsilon_count++))
			done
			((tlsize_count++))
		done
		((graph_count++))
	done
	((sc_count++))
done
