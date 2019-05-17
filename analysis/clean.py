import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

def read(fname):
    return pd.read_csv(fname, header=None, names=[
        'input-file', 'output-file', 'epsilon',
        'tabu-list-size', 'stop-crit', 'performed-iterations',
        'fitness', 'disconnected', 'min-weight', 'max-weight',
        'mean-weight', 'std-dev-weight'])

ts1 = read("tabu-stats1.csv")
ts2 = read("tabu-stats2.csv")
ts3 = read("tabu-stats3.csv")

ts = pd.concat([ts1, ts2, ts3], ignore_index=True)
ts.to_csv("tabu-stats.csv", index=False)
