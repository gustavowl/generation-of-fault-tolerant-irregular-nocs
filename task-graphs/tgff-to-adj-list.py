import sys

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

        #reads until finding "@COMMUN", which is the hash table
        if "@COMMUN" in line:
            weight_hash = read_edges_weigth_table(f)
            print(weight_hash)
        
    return num_nodes, adj_list

def write_adj_list(output_file, num_nodes, adj_list):
    output_file.write("# number of nodes,number of links")
    output_file.write(str(num_nodes) + ',' + str(len(adj_list)))
    adjl = ""
    for link in adj_list:
            print(link)

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
