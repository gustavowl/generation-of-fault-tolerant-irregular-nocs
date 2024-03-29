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
#STOP_CRITERION=(100 250 500 1000 2000)
#NUM_NODE_MULT=(0.5 1 2 3 4 5)
STOP_CRITERION=(100 250)
NUM_NODE_MULT=(0.5 1)

make

sc_count=1
for j in {0..2}; do
	for sc in ${STOP_CRITERION[*]}; do
		graph_count=1
		for graph in ${GRAPHS[*]}; do
			ghead=$(head -1 $graph)
			nodeindex=$(expr index "$ghead" "$SEPARATOR")
			numnodes=${ghead:0:$nodeindex-1}
			numedges=${ghead:nodeindex}

			TABU_LIST_SIZES=(${numnodes})
			for i in ${NUM_NODE_MULT[*]}; do
				TABU_LIST_SIZES+=($(echo "scale=0; (${numnodes}*${i})/1" | bc))
			done

			tlsize_count=1
			for tlsize in ${TABU_LIST_SIZES[*]}; do
				epsilon_count=1
				for (( epsilon=$numnodes; epsilon<=$numnodes*2; epsilon++ )); do
					exec_count=1
					for i in {1..10}; do
						echo "=============================================================================================="
						echo "Stop Criterion:" $sc '('$sc_count'/'${#STOP_CRITERION[*]}')'
						echo "Graph: "$graph '('$graph_count'/'${#GRAPHS[*]}')'
						echo "Tabu List Size:" $tlsize '('$tlsize_count'/'${#TABU_LIST_SIZES[*]}')'
						echo "Epsilon:" $epsilon '('$epsilon_count'/'$((numnodes+1))')'
						echo "Execution:" $exec_count '('$(($j*10+$exec_count))'/'$(($j*10+10))')'
						otpt=$(echo `expr "$graph" : '\(\(\([^/]\)*/\)*\)'`)
						pos=${#otpt}
						otpt=${graph:pos}
						otpt=$(echo `expr "$otpt" : '\(\([^\.]\)*\)'`)
						otpt=$otpt"-sc"$sc"-tls"$tlsize"-eps"$epsilon"-exec"
						mult=$(($j*10+$i))
						if [ $mult -lt 10 ]; then
							otpt=$otpt'0'$mult
						else
							otpt=$otpt$mult
						fi
						otpt=$otpt".adjl"
						echo -e  "args: "$graph $SEPARATOR $epsilon $tlsize $sc $otpt
						echo "----------------------------------------------------------------------------------------------"
						./gftinoc $graph $SEPARATOR $epsilon $tlsize $sc $otpt
						echo "=============================================================================================="
						echo -e "\n"
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
done
