import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import os
import math
import re

def filter_graph_name(graph):
    return graph[graph.rfind('/')+1 : graph.rfind('.')]

args = "args"
args_eps = os.path.join(args, "epsilon")
args_tls = os.path.join(args, "tabu-list-size")
args_sc = os.path.join(args, "stop-crit")
fault_dir = "fault"
fault_fit = os.path.join(fault_dir, "fitness")
fault_prop = os.path.join(fault_dir, "proportion")
fault_boxplt = os.path.join(fault_dir, "boxplot")
fault_boxplt_fit = os.path.join(fault_boxplt, "fitness")
fault_boxplt_prop = os.path.join(fault_boxplt, "proportion")

if not os.path.exists(args):
    os.mkdir(args)
if not os.path.exists(args_eps):
    os.mkdir(args_eps)
if not os.path.exists(args_tls):
    os.mkdir(args_tls)
if not os.path.exists(args_sc):
    os.mkdir(args_sc)
if not os.path.exists(fault_dir):
    os.mkdir(fault_dir)
if not os.path.exists(fault_fit):
    os.mkdir(fault_fit)
if not os.path.exists(fault_prop):
    os.mkdir(fault_prop)
if not os.path.exists(fault_boxplt):
    os.mkdir(fault_boxplt)
if not os.path.exists(fault_boxplt_fit):
    os.mkdir(fault_boxplt_fit)
if not os.path.exists(fault_boxplt_prop):
    os.mkdir(fault_boxplt_prop)

ts = pd.read_csv("tabu-stats.csv")

#args boxplot
#for graph in ts['input-file'].unique():
#    gstats = ts[ts['input-file'].str.match(graph)] 
#    title = filter_graph_name(graph)
#    sns.boxplot(gstats['epsilon'], gstats['fitness'],
#            color='white')
#    plt.ylabel("Latency Estimation (Fitness)", fontsize='xx-large')
#    plt.xlabel("Number of Links", fontsize='xx-large')
#    plt.xticks(fontsize='xx-large')
#    plt.yticks(fontsize='xx-large')
#    plt.tight_layout()
#    plt.savefig("args/epsilon/" + title)
#    plt.clf()
#
#    sns.boxplot(gstats['tabu-list-size'], gstats['fitness'],
#            color='white')
#    plt.ylabel("Latency Estimation (Fitness)", fontsize='xx-large')
#    plt.xlabel("Tabu List Size", fontsize='xx-large')
#    plt.xticks(fontsize='xx-large')
#    plt.yticks(fontsize='xx-large')
#    plt.tight_layout()
#    plt.savefig("args/tabu-list-size/" + title)
#    plt.clf()
#
#    sns.boxplot(gstats['stop-crit'], gstats['fitness'],
#            color='white')
#    plt.ylabel("Latency Estimation (Fitness)", fontsize='xx-large')
#    plt.xlabel("Termination Criterion", fontsize='xx-large')
#    plt.xticks(fontsize='xx-large')
#    plt.yticks(fontsize='xx-large')
#    plt.tight_layout()
#    plt.savefig("args/stop-crit/" + title)
#    plt.clf()

#selects "floor" median graphs
#median(30) = (15+16)/2. Select index 15
selected = pd.DataFrame(columns=ts.columns)
for graph in ts['input-file'].unique():
    df = ts[ts['input-file'].str.match(graph)]
    for eps in df['epsilon'].unique():
        t = df[df['epsilon'] == eps]
        t = t.sort_values(by=['fitness'])
        rows, _ = t.shape
        index = math.floor(np.median(np.arange(0, rows)))

        selected = selected.append(t.iloc[index], ignore_index=True)

def get_graph_dir(graph):
    #expected directory hierarchy:
    #generation/res/graph/graph.adjl
    #generation/res/graph/fault-stats.csv
    #generation/res/graph/fault-injection/
    #
    #generation/res/graph[1-10]
    #generation2/res/graph[11-20]
    #generation3/res/graph[21-30]
    dir = "../../generation"

    if (re.match(r".*exec(1[1-9])|(20)", graph) != None):
        dir += "2"
    elif (re.match(r".*exec(2[1-9])|(30)", graph) != None):
        dir += "3"

    return dir + "/res"

#median fault boxplots for ap2 and mpeg

def fault_tolerance_box_plot(df):
    for inputgraph in df['input-file'].unique():
        igdf = df[df['input-file'].str.match(inputgraph)]
        rows, _ = igdf.shape
        index = math.floor(np.median(np.arange(0, rows)))
        tgraph = igdf.iloc[index]

        dir = get_graph_dir(tgraph['output-file'])
        gname = filter_graph_name(tgraph['output-file'])

        fault_stats = os.path.join(dir, gname, "fault-stats.csv")
        fault_stats = pd.read_csv(fault_stats, header=None, names=[
            'output-graph', 'perc', 'fitness', 'disconnected',
            'min-weight', 'max-weight', 'mean-weight', 'weight-std-dev'])

        sns.boxplot(fault_stats['perc'], fault_stats['fitness'],
                color='white')
        outputfig = os.path.join(fault_boxplt_fit, gname)

        plt.ylabel("Latency Estimation (Fitness)", fontsize='xx-large')
        plt.xlabel("Fault Percentage", fontsize='xx-large')
        plt.xticks(fontsize='xx-large')
        plt.yticks(fontsize='xx-large')
        plt.tight_layout()

        plt.savefig(outputfig)
        plt.clf()

        ax = sns.boxplot(fault_stats['perc'],
                fault_stats['fitness'] / tgraph['fitness'],
                color='white')
        ax.axhline(1, ls='--', color='r', alpha=0.7)
        outputfig = os.path.join(fault_boxplt_prop, gname)

        plt.ylabel("Latency Overhead", fontsize='xx-large')
        plt.xlabel("Fault Percentage", fontsize='xx-large')
        plt.xticks(fontsize='xx-large')
        plt.yticks(fontsize='xx-large')
        plt.tight_layout()

        plt.savefig(outputfig)
        plt.clf()

#fault_tolerance_box_plot(selected)

def median_faults(graph):
    #returns a dict with the respective fitnesses for
    #10, 15, 20, 25, and 30% fault injection
    dir = get_graph_dir(graph) 
    gname = filter_graph_name(graph)
    fault_stats = os.path.join(dir, gname, "fault-stats.csv")

    fault_stats = pd.read_csv(fault_stats, header=None, names=[
        'output-graph', 'perc', 'fitness', 'disconnected',
        'min-weight', 'max-weight', 'mean-weight', 'weight-std-dev'])

    ret = []
    for perc in fault_stats['perc'].unique():
        fs = fault_stats[fault_stats["perc"] == perc]
        fs = fs.sort_values(by=['fitness'])

        rows, _ = fs.shape
        index = math.floor(np.median(np.arange(0, rows)))

        ret.append(fs.iloc[index])
    return ret

def fault_fitness_median_graph(selected, proportional, outdir):
    for graph in selected['input-file'].unique():
        fit_prop = pd.DataFrame(columns=['x', 'y', 'perc'])
    
        df = selected[selected['input-file'].str.match(graph)]
        num_rows, _ = df.shape

        lines = [] #10, 15, 20, 25, and 30% per epsilon

        for i in range(num_rows):
            row = df.iloc[i]
            inj_fit = median_faults(row['output-file'])

            eps = row['epsilon']

            fit = row['fitness']
            for i in range(len(inj_fit)):
                y = np.nan
                if not inj_fit[i]['disconnected']:
                    if proportional:
                        y = inj_fit[i]['fitness'] / fit
                    else:
                        y = inj_fit[i]['fitness']

                fit_prop = fit_prop.append({'x': eps, 'y': y,
                    'perc': inj_fit[i]['perc']},
                    ignore_index=True)

        xmax = max(fit_prop['x'])
        xmin = min(fit_prop['x'])
        ymax = max(fit_prop['y'])
        ymin = min(fit_prop['y'])
        inf = ymax * 1.1
        fit_prop = fit_prop.fillna(inf)
        #fit_prop = fit_prop[fit_prop['perc'] != 15]
        #fit_prop = fit_prop[fit_prop['perc'] != 25]
        fit_prop['perc'] = fit_prop['perc'].astype(int)
        fit_prop['perc'] = fit_prop['perc'].astype(str)
        fit_prop['perc'] += '%'

        print(fit_prop)
        epsilons = np.round(np.arange(xmin, xmax + 1, (xmax - xmin)/4))
        print(epsilons)
        print(fit_prop[fit_prop['x'].isin(epsilons)])
        fit_prop = fit_prop[fit_prop['x'].isin(epsilons)]

        #sns.lineplot(x='perc', y='y', data=fit_prop, hue='num_links')
        fig, ax = plt.subplots(figsize=(9,6))
        count = 0
        colours = ['b', 'm', 'r', 'g', 'k']
        markers = ['d', 'o', 'v', 's', 'P']
        for eps in fit_prop['x'].unique():
            sel_num_links = fit_prop[fit_prop['x'] == eps]
            ax.plot(sel_num_links['perc'], sel_num_links['y'],
                    color=colours[count], alpha=0.6,
                    marker=markers[count],
                    label=str(int(eps)) + " links")
            count += 1
        ax.legend(shadow=True, fontsize='x-large',
                loc='center left', bbox_to_anchor=(1, 0.5))

        #sns.lineplot(x='perc', y='y', data=fit_prop, hue='x')
        plt.xlabel("Fault Percentage", fontsize="x-large")
        if (proportional):
            plt.ylabel("Latency Overhead", fontsize="x-large")
        else:
            plt.ylabel("Latency Estimation (Fitness)",
                    fontsize="x-large")

        #plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
        #plt.xticks(np.arange(xmin, xmax + 1))
        plt.xticks(fit_prop['perc'].unique(), fontsize="x-large")

        step = (inf - ymin) / 10
        ytcks = np.arange(ymin, inf + 0.0001*step, step)
        if (proportional):
            ytcks = [round(yt, 2) for yt in ytcks]
        else:
            ytcks = [int(yt) for yt in ytcks]
        ytcks[-1] = 'Inf'
        plt.yticks(np.arange(ymin, inf + step, step),
                ytcks, fontsize="x-large")
        plt.grid()

        outfname = os.path.join(outdir, filter_graph_name(graph))
        plt.tight_layout()
        plt.savefig(outfname)
        plt.clf()

#sns.set(rc={'figure.figsize':(9, 6)})
fault_fitness_median_graph(selected, True, fault_prop)
#fault_fitness_median_graph(selected, False, fault_fit)

#all median
def fitness_median_graph(flip_cond, threshold, step):
    xmin = 732112
    xmax = 0
    ymin = 732112
    ymax = 0
    df = pd.DataFrame(columns=['x', 'y', 'Graph'])
    for graph in ts['input-file'].unique():
        gstats = ts[ts['input-file'].str.match(graph)]
        x = gstats['epsilon'].unique()
        y = []
        for eps in x:
            median_eps = gstats[gstats['epsilon'] == eps]
            y.append(median_eps['fitness'].median())
        
        condition = any(fitness > threshold for fitness in y)
        if (flip_cond):
            condition = not condition
        if (condition):
            for i in range(len(x)):
                df = df.append({'x': x[i], 'y': y[i],
                    'Graph': filter_graph_name(graph)},
                    ignore_index=True)
            #sns.lineplot(x, y, hue="graph")
            if (min(x) < xmin):
                xmin = min(x)
            if (max(x) > xmax):
                xmax = max(x)
            if (min(y) < ymin):
                ymin = min(y)
            if (max(y) > ymax):
                ymax = max(y)
    sns.lineplot(x='x', y='y', data=df, hue='Graph')
    plt.xlabel("Number of links", fontsize='large')
    plt.ylabel("Latency Estimation (Fitness)", fontsize='large')
    plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,
            fontsize='large')
    plt.xticks(np.arange(xmin, xmax + 1), fontsize='large')
    plt.yticks(np.arange(ymin, ymax + step, step), fontsize='large')
    title = ""
    for g in df['Graph'].unique():
        title += g + '-'
    title = title[0 : len(title) - 1]
    plt.tight_layout()
    plt.savefig(title)
    plt.clf()

#sns.set(rc={'figure.figsize':(9, 6)})
#fitness_median_graph(False, 6000, 750)
#fitness_median_graph(True, 6000, 250)
