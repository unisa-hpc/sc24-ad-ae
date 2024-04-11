import matplotlib
from matplotlib.lines import Line2D
from matplotlib.patches import Patch
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import numpy as np

matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams['ps.fonttype'] = 42


TIME_ROUND = 1000
ENERGY_ROUND = 1000
STD_NORM_FACTOR = 1
FONTSIZE = 14

Y_LIMS = (0, 3.5)

TIME_METRIC = 'mean'
ENERGY_METRIC = 'mean'
NORM_TIME_ERR = 7
NORN_ENERGY_ERR = 1

LEGEND_FONTSIZE = 10
BAR_WIDTH = 0.4
PROPS = dict(facecolor='lightgray', alpha=1)

COLOR_FINE = 'C1'
COLOR_PHASE = 'C2'
COLOR_MPI = 'C8'
HATCH_CLOVER = '.'
HATCH_MINI = 'O.'

FONT = {'family' : 'normal',
        'weight' : 'normal',
        'size'   : 32}

plt.rc('font', **FONT)

legend = [
  Line2D([0], [0], color="red", label="Coarse-Grained", linestyle="dashed", linewidth=0.7),     
  Patch(facecolor='w', edgecolor='k', label='CloverLeaf', hatch=(HATCH_CLOVER * 3)), 
  Patch(facecolor='w', edgecolor='k', label='MiniWeather', hatch=(HATCH_MINI * 2)),
]

def compute_values(appname: str, data1: pd.DataFrame, time_metric: str = 'mean', energy_metric: str = 'mean'):  
  clover_coarse_time = data1[data1['name'] == f'{appname}_per_app'][f'{time_metric}_time'].values[0]
  # clover_coarse_time_err = data1[data1['name'] == f'{appname}_per_app'][f'std_time'].values[0] / TIME_ROUND
  clover_fine_time = data1[data1['name'] == f'{appname}_per_kernel'][f'{time_metric}_time'].values[0]
  # clover_fine_time_err = data1[data1['name'] == f'{appname}_per_kernel'][f'std_time'].values[0] / TIME_ROUND
  clover_no_hiding_time = data1[data1['name'] == f'{appname}_no_hiding'][f'{time_metric}_time'].values[0]
  # clover_no_hiding_time_err = data1[data1['name'] == f'{appname}_no_hiding'][f'std_time'].values[0] / TIME_ROUND
  clover_hiding_time = data1[data1['name'] == f'{appname}_hiding'][f'{time_metric}_time'].values[0] 
  # clover_hiding_time_err = data1[data1['name'] == f'{appname}_hiding'][f'std_time'].values[0] / TIME_ROUND
   
  # clover_coarse_energy_err = data1[data1['name'] == f'{appname}_per_app'][f'std_energy'].values[0] 
  # clover_fine_energy_err = data1[data1['name'] == f'{appname}_per_kernel'][f'std_energy'].values[0] 
  # clover_no_hiding_energy_err = data1[data1['name'] == f'{appname}_no_hiding'][f'std_energy'].values[0] 
  # clover_hiding_energy_err = data1[data1['name'] == f'{appname}_hiding'][f'std_energy'].values[0] 
  clover_coarse_energy = data1[data1['name'] == f'{appname}_per_app'][f'{energy_metric}_energy'].values[0] 
  clover_fine_energy = data1[data1['name'] == f'{appname}_per_kernel'][f'{energy_metric}_energy'].values[0]
  clover_no_hiding_energy = data1[data1['name'] == f'{appname}_no_hiding'][f'{energy_metric}_energy'].values[0]
  clover_hiding_energy = data1[data1['name'] == f'{appname}_hiding'][f'{energy_metric}_energy'].values[0]
  
  print("************", appname.capitalize(), "************")
  print(f"{'Approach':<20} {'Speedup':<20} {'Normalized Energy':<20}")
  print("------------------------------------------------------------")
  print(f"{'Fine-Grained':<20} {clover_coarse_time / clover_fine_time:<20} {clover_fine_energy / clover_coarse_energy:<20}")
  print(f"{'Phase-TDAG':<20} {clover_coarse_time / clover_no_hiding_time:<20} {clover_no_hiding_energy / clover_coarse_energy:<20}")
  print(f"{'Phase-Based + Hiding':<20} {clover_coarse_time / clover_hiding_time:<20} {clover_hiding_energy / clover_coarse_energy:<20}")
    
  df = pd.DataFrame(columns=['approach', 'time', 'speedup', 'energy', 'norm-energy'])
  # df.loc[len(df)] = ('Coarse-Grained', clover_coarse_time, clover_coarse_time_err, clover_coarse_energy, clover_coarse_energy_err)
  df.loc[len(df)] = ('Fine-Grained', clover_fine_time, (clover_coarse_time / clover_fine_time), clover_fine_energy, (clover_fine_energy / clover_coarse_energy))
  df.loc[len(df)] = ('Phase-E', clover_no_hiding_time, (clover_coarse_time / clover_no_hiding_time), clover_no_hiding_energy, (clover_no_hiding_energy / clover_coarse_energy))
  df.loc[len(df)] = ('Phase-MPI', clover_hiding_time, (clover_coarse_time / clover_hiding_time), clover_hiding_energy, (clover_hiding_energy / clover_coarse_energy))

  return df

def plot(ax, mini, clover, val):
  x = np.arange(len(mini['approach']))
  ax.axhline(y=1, color='red', linestyle='--', linewidth=0.7)
  ax.bar(x - BAR_WIDTH / 2, clover[val], width=BAR_WIDTH, color=[COLOR_FINE, COLOR_PHASE, COLOR_MPI], hatch=HATCH_CLOVER)
  ax.bar(x + BAR_WIDTH / 2, mini[val], width=BAR_WIDTH, color=[COLOR_FINE, COLOR_PHASE, COLOR_MPI], hatch=HATCH_MINI)
  ax.set_xticks(x, mini['approach'], rotation=20)
  

if __name__ == '__main__':
  sns.set_theme()
  parser = argparse.ArgumentParser(description='Plot time and energy')
  parser.add_argument('clover_csv', type=str, help='CSV file')
  parser.add_argument('mini_csv', type=str, help='CSV file')
  parser.add_argument('output', type=str, help='Output file')
  
  args = parser.parse_args()
  
  clover = pd.read_csv(args.clover_csv)
  mini = pd.read_csv(args.mini_csv)
  
  df_clover = compute_values("cloverleaf", clover, time_metric='median', energy_metric='max')
  df_mini = compute_values("miniweather", mini, time_metric='median', energy_metric='median')

  _, axs = plt.subplots(1, 2, sharex=True, sharey=False, figsize=(8, 4))
  plot(axs[0], df_clover, df_mini, 'speedup')
  axs[0].set_ylabel('Speedup', fontsize=FONTSIZE)
  axs[0].tick_params(axis='both', labelsize=FONTSIZE)
  plot(axs[1], df_clover, df_mini, 'norm-energy')
  axs[1].set_ylabel('Normalized Energy', fontsize=FONTSIZE)
  axs[1].tick_params(axis='both', labelsize=FONTSIZE)
  
  axs[1].legend(handles=legend, loc='best', fontsize=LEGEND_FONTSIZE)
  plt.tight_layout()
  
  plt.savefig(args.output)
  plt.clf()