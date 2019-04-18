#!/bin/bash

#BEFORE RUNNING THIS SCRIPT, INSTALL TGFF 3.6
#https://robertdick.org/projects/tgff/index.html

ORIG_DIR=$(pwd)
TGFF=${ORIG_DIR}"/tgff-3.6/tgff" #path to compiled tgff
DESC="descriptions"
ADJL_DIR=${ORIG_DIR}/adj-lists
EPS_DIR=${ORIG_DIR}/eps

#stores all files: .tgffopt, .tgff, .eps, .vcd, and .adjl.
#The files are distributed in a directory tree accoring to
#attributes used in the for loop in the end of this file.
mkdir -p $DESC 
#stores only .adjl files in a single dir
mkdir -p $ADJL_DIR 
#stores only .eps files in a single dir
mkdir -p $EPS_DIR

# creates .tgffopt files
#mkdir -p $DESC/shableu

#random seed for generating graphs
ARR_SEED=(73 2112) 
#0 = task graphs with one start node
#1 = task graph with more than one start node
ARR_PROB_MULTI_START_NODES=(0 1)
#Number of nodes
#note: number of nodes is not restrictive.
#	tgff generates graphs with a number of
#	tasks given some restrictions, and
#	lower and upper bounds.
#	THIS SCRIPTS WILL ATTEMPT TO GENERATE
#	TASKS WITH ARR_TASK_CNT^2 NODES.
ARR_TASK_CNT=(4 5 6 7 8)

#Label of communication tables
COM_TAB_LABEL="COMMUN"
#Number of communication tables
COM_TAB_NUM=1

#Random factor for multiple start nodes:
#[a +- b)
START_NODE=(A B)

#weight range: [mean +- multiplier)
ARR_WEIGHT_MEAN=(51 251)
ARR_WEIGTH_MULT=(50 250)
ARR_WEIGHT_LEN=${#ARR_WEIGHT_MEAN[*]}

write_tgffopt ()
{
	FILENAME=in-out-degrees-$[ DEGREE ]-$[ DEGREE ]
	FILETYPE=.tgffopt

	echo seed $SEED > ${FILENAME}${FILETYPE}
	echo tg_cnt 1 >> ${FILENAME}${FILETYPE}
	echo trans_type_cnt $[ TASK_CNT ** 3 ] >> ${FILENAME}${FILETYPE}
	echo prob_multi_start_nodes ${MULTI_START_NODES} >> ${FILENAME}${FILETYPE}
	echo start_node $[ TASK_CNT / 2 ] $[ TASK_CNT/2 - 1 ] >> ${FILENAME}${FILETYPE}
	echo period_mul ${TASK_CNT} >> ${FILENAME}${FILETYPE}
	echo task_degree ${DEGREE} ${DEGREE} >> ${FILENAME}${FILETYPE} 
	echo task_cnt $[ TASK_CNT ] 1 >> ${FILENAME}${FILETYPE}
	echo table_cnt 1 >> ${FILENAME}${FILETYPE}
	echo table_label COMMUN >> ${FILENAME}${FILETYPE}
	echo type_attrib cost ${WEIGHT_MEAN} $WEIGHT_MULT} 0 1 >> ${FILENAME}${FILETYPE} 
	echo trans_write >> ${FILENAME}${FILETYPE} 
	echo tg_write >> ${FILENAME}${FILETYPE} 
	echo eps_write >> ${FILENAME}${FILETYPE} 
	echo vcg_write >> ${FILENAME}${FILETYPE} 

	#echo Wrote ${WORK_DIR}/${FILENAME}${FILETYPE}
}

copy_file ()
{
	#expects 3 arguments:
	#	#1 - FILENAME
	#	#2 - FILETYPE
	#	#3 - TARGET DIRECTORY
	FILENAME=$1
	FILETYPE=$2
	TARGET_DIR=$3
	
	#string manipulation to add original filepath to its name
	PREFIX=${WORK_DIR:${#ORIG_DIR}}
	PREFIX=${PREFIX/#\//}
	PREFIX=$(echo "${PREFIX}" | sed 's/\//_/g')

	TARGET_FILENAME=${PREFIX}_${FILENAME}${FILETYPE}

	cp -f ${FILENAME}${FILETYPE} ${TARGET_DIR}/${TARGET_FILENAME}${FILETYPE}
}

run_tgff ()
{
	$TGFF ${WORK_DIR}/${FILENAME}
	copy_file $FILENAME ".eps" ${EPS_DIR}
}

parse_tgff_to_adjacency_list ()
{
	python3 ${ORIG_DIR}/tgff-to-adj-list.py ${WORK_DIR}/${FILENAME}.tgff
	copy_file $FILENAME ".adjl" ${ADJL_DIR}
}

# creates .tgffopt files
TOTAL=$[ ${#ARR_SEED[*]} * ${#ARR_PROB_MULTI_START_NODES[*]} * ARR_WEIGHT_LEN * ${#ARR_TASK_CNT[*]} ]

for SEED in ${ARR_SEED[*]}; do
	WORK_DIR=${ORIG_DIR}/${DESC}
	cd $WORK_DIR

	WORK_DIR=$(pwd)/seed-${SEED}
	mkdir -p $WORK_DIR
	cd $WORK_DIR

	for MULTI_START_NODES in ${ARR_PROB_MULTI_START_NODES[*]}; do
		if [ "$MULTI_START_NODES" -eq 0 ]; then
			WORK_DIR=$(pwd)/single-start-node
		elif [ "$MULTI_START_NODES" -eq 1 ]; then
			WORK_DIR=$(pwd)/multi-start-node
		else
			echo "INVALID VALUE OF MULTI_START_NODES"
			continue
		fi
		
		mkdir -p $WORK_DIR
		cd $WORK_DIR
		
		for (( WEIGHT_I=0; WEIGHT_I<$ARR_WEIGHT_LEN; WEIGHT_I++ )); do
			WEIGHT_MEAN=${ARR_WEIGHT_MEAN[$WEIGHT_I]}
			WEIGHT_MULT=${ARR_WEIGTH_MULT[$WEIGHT_I]}

			MIN_WEIGHT=$[ WEIGHT_MEAN - WEIGHT_MULT ]
			MAX_WEIGHT=$[ WEIGHT_MEAN + WEIGHT_MULT - 1 ]

			WORK_DIR=$(pwd)/weight-range-${MIN_WEIGHT}-${MAX_WEIGHT}
			mkdir -p $WORK_DIR
			cd $WORK_DIR

 			for TASK_CNT in ${ARR_TASK_CNT[*]}; do
 				WORK_DIR=$(pwd)/dimension-${TASK_CNT}x${TASK_CNT}
 				
 				mkdir -p ${WORK_DIR}
 				cd ${WORK_DIR}

				for (( DEGREE=2; DEGREE<$[ TASK_CNT + 1 ]; DEGREE++ )); do
					write_tgffopt
					run_tgff
					parse_tgff_to_adjacency_list
				done
				COUNT=$[ COUNT + 1 ]
				PERC=$(echo "scale=2; 100*${COUNT}/${TOTAL}" | bc )
				echo ${PERC}%
				#echo ""
 				
 				cd ..
			done
			cd ..
		done
		cd ..
	done
done
