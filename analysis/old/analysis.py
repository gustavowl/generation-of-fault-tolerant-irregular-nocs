import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

tabu_stats = pd.read_csv("./tabu-stats.csv", header=None, names=['input-file', 'output-file',
    'epsilon', 'tabu-list-size', 'stop-crit', 'performed-iteration', 'fitness',
    'disconnected', 'min-weight', 'max-weight', 'mean-weight', 'std-dev-weight'])

for graph in tabu_stats['input-file'].unique():
    ts = tabu_stats[tabu_stats['input-file'].str.match(graph)]

    title = graph[graph.rfind('/')+1:graph.rfind('.')]
    sns.boxplot(ts['epsilon'], ts['fitness'], color='white').set_title(title)

    #plt.show()
    plt.savefig(title)
    plt.clf()
    ts = ts[ts['fitness'] == ts['fitness'].min()]
    print(ts[['output-file', 'fitness']])
