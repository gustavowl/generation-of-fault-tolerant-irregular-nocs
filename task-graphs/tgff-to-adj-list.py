import sys
import re

def read_task_graph(f):
    links = []
    num_nodes = 0
    while True:
        #reads lines and removes unnecessary whitespaces
        line = f.readline().strip()
        line = re.sub('\t+',' ', line)
        line = re.sub(' +', ' ', line) 
        
        if line == '': #empty line
            continue

        if '{' in line:
            continue

        if line[0] == '#': #commentary
            continue

        #end of table
        if '}' in line:
            return num_nodes, links

        #found a node description
        if 'TASK' in line:
            num_nodes += 1
            continue

        #found an edge description
        if 'ARC' in line:
            #saves link info. Format: ARC x FROM y TO z TYPE k
            #x and y format: ta_b, where
            #   a = task id
            #   b = node id

            #saves link origin
            line = line[line.index('FROM') + 4:].strip()
            orig = int(line.split(' ')[0].split('_')[1])

            #saves link destiny
            line = line[line.index('TO') + 2:].strip()
            dest = int(line.split(' ')[0].split('_')[1])

            #saves weight_id
            line = line[line.index('TYPE') + 4:].strip()
            weight_id = int(line.split(' ')[0])

            links.append([orig, dest, weight_id])

def read_edges_weigth_table(f):
    weight_hash = []

    while True:
        line = f.readline().strip()

        if '{' in line:
            continue

        if line == '': #empty line
            continue

        if line[0] == '#': #commentary
            continue

        #end of table
        if '}' in line:
            return weight_hash

        line = line.split(' ')
        if (len(line) > 1):
            #valid trable entry (edge)
            #index    weight
            weight_hash.append(int(line[-1]))


def parse_tgff(f):
    # n-nodes, adj-list
    num_nodes = 0
    adj_list = []
    weight_hash = []

    while True:
        line = f.readline()

        #finishes if EOF
        if not line:
            break

        #reads until finding "@COMMUN", which is the weight hash table
        if "@COMMUN" in line:
            weight_hash = read_edges_weigth_table(f)
            continue

        #reads until finding "@TASK_COMMUN", which is the table that
        #contains the graph topology: nodes, edges and weight_keys
        #(weight_keys are used as key to the hash table in @COMMUN)
        if "@TASK_GRAPH" in line:
            num_nodes, adj_list = read_task_graph(f) 
            continue
        
    #Adds edges weights according to weight_hash
    for edge in adj_list:
        edge[2] = weight_hash[edge[2]]

    return num_nodes, adj_list

def write_adj_list(output_file, num_nodes, adj_list):
    output_file.write("# number_of_nodes,number_of_links\n")
    output_file.write(str(num_nodes) + ',' + str(len(adj_list)) + '\n')
    output_file.write("# orig_node,dest_node,edge_weight\n")

    adjl = ""
    for link in adj_list:
        adjl += str(link[0]) + ',' + str(link[1]) + ',' + str(
                link[2]) + '\n'

    output_file.write(adjl)

if len(sys.argv) != 2:
    print("Expected exactly 1 argument:")
    print("\t1 - Path of .tgff file")
    exit()

tgff = open(sys.argv[1], 'r')
if (tgff.closed):
    print("Failed to open file " + str(sys.argv[1]) +
            ".\n\t Aborting...")
    exit()

n_nodes, adj_list = parse_tgff(tgff)
tgff.close()

output_filename = sys.argv[1].split('.')[0] + ".adjl"
output_file = open(output_filename, 'w')
if (output_file.closed):
    print("Failed to open file " + output_filename +
            ".\n\t Aborting...")
    exit()

write_adj_list(output_file, n_nodes, adj_list)

output_file.close()
