#!/bin/bash

SEPARATOR=','
GRAPHS=("task-graphs/jonathan-graphs-adap/ap1.adjl"\
	"task-graphs/jonathan-graphs-adap/ap2.adjl"\
	"task-graphs/jonathan-graphs-adap/ap3.adjl"\
	"task-graphs/jonathan-graphs-adap/ap4.adjl"\
	"task-graphs/jonathan-graphs-adap/integral.adjl"\
	"task-graphs/jonathan-graphs-adap/mpeg.adjl"\
	"task-graphs/jonathan-graphs-adap/mwd.adjl"\
	"task-graphs/jonathan-graphs-adap/vopd.adjl")
STOP_CRITERION=(100 250 500 1000 2000)
NUM_NODE_MULT=(0.5 1 2 3 4 5)

#task-graphs/adj-lists/descriptions_seed-73_single-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl
#descriptions_seed-73_single-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-3-3.adjl

sc_count=1
for sc in ${STOP_CRITERION[*]}; do
	graph_count=1
	for graph in ${GRAPHS[*]}; do
		ghead=$(head -1 $graph)
		nodeindex=$(expr index "$ghead" "$SEPARATOR")
		numnodes=${ghead:0:$nodeindex-1}
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
					echo "===================================================================================="
					echo "Stop Criterion:" $sc '('$sc_count'/'${#STOP_CRITERION[*]}')'
					echo "Graph: "$graph '('$graph_count'/'${#GRAPHS[*]}')'
					echo "Tabu List Size:" $tlsize '('$tlsize_count'/'${#TABU_LIST_SIZES[*]}')'
					echo "Epsilon:" $epsilon '('$epsilon_count'/'$((numnodes+1))')'
					echo "Execution:" $exec_count '('$exec_count'/'30')'
					echo "------------------------------------------------------------------------------------"
					otpt=$(echo `expr "$graph" : '\(\(\([^/]\)*/\)*\)'`)
					pos=${#otpt}
					otpt=${graph:pos}

					./gftinoc $graph $SEPARATOR $epsilon $tlsize $sc $otpt

					echo "===================================================================================="
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
