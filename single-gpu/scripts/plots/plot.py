from types import SimpleNamespace
import pandas as pd
import glob 
import numpy as np
import sys
import os
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
import matplotlib

matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams['ps.fonttype'] = 42

sns.set_theme()

BAR_WIDTH = 0.4
HLINE_STYLE = {"color": "red", "ls": "dashed", "lw": "0.5", "zorder": 1}
COLOR1='C0'
COLOR2='C1'
COLOR3='C2'

legend = [
  Line2D([0], [0], color="red", label="Coarse-Grained", linestyle="dashed", linewidth=0.7),     
  Patch(facecolor=COLOR2, label='Fine-Grained'),
  Patch(facecolor=COLOR3, label='Phase-Based'), 
]

hardware = {
  'AMD': 'MI100',
  'Intel': 'Max 1100',
  'NVIDIA': "V100S",
}   

def subplot(ax, vals1, vals2, names, *, fontsize, y_lims):
    x = np.arange(len(names))
    ax.axhline(1, **HLINE_STYLE)

    bars1 = ax.bar(x + (BAR_WIDTH / 2), vals1, width=BAR_WIDTH, color=COLOR3)
    bars2 = ax.bar(x - (BAR_WIDTH / 2), vals2, width=BAR_WIDTH, color=COLOR2)
    
    for bar in bars1 + bars2:
      yval = bar.get_height()
      if yval == 0:
        pass
        # ax.text(bar.get_x() + bar.get_width()/2, max(yval, y_lims[0]), 'NA', ha='center', va='bottom', rotation=90, color='black', fontsize=fontsize['numbers'])
      elif bar.get_height() - 0.4 < y_lims[0]:
        ax.text(bar.get_x() + bar.get_width()/2, max(yval, y_lims[0]), round(yval, 2), ha='center', va='bottom', rotation=90, color='k', fontsize=fontsize['numbers'])
      else:
        ax.text(bar.get_x() + bar.get_width()/2, min(y_lims[1], yval) - 0.05, round(yval, 2), ha='center', rotation=90, color='k', va='top', fontsize=fontsize['numbers'])
    x_labels = [str(d) for d in names]
    ax.set_xticks(x, labels=x_labels)


def plot(df: pd.DataFrame, val, *, 
         y_label, 
         y_lims, 
         calc_val, 
         figsize,
         x_rotation,
         wspace,
         show_legend,
         fontsize):    
  
  fig, axs = plt.subplots(1, 3, sharex=True, sharey=True, figsize=figsize)
  axs[0].set_ylabel(y_label, fontsize=fontsize['text'])
  for i, (n, g) in enumerate(df.groupby('arch')):
    df_app = g[g['approach'] == 'app']
    df_phase = g[g['approach'] == 'phase']
    df_kernel = g[g['approach'] == 'kernel']
    vals1 = np.nan_to_num(calc_val(df_app[f'{val}'].values, df_phase[f'{val}'].values))
    vals2 = np.nan_to_num(calc_val(df_app[f'{val}'].values, df_kernel[f'{val}'].values))
    subplot(axs[i], vals1, vals2, df_app['name'].unique(), fontsize=fontsize, y_lims=y_lims)
    axs[i].tick_params(axis='x', labelsize=fontsize['text'], rotation=x_rotation)
    axs[i].tick_params(axis='y', labelsize=fontsize['ticks'])
    axs[i].set_ylim(*y_lims)
    axs[i].set_xlabel(f"{n} {hardware[n]}", labelpad=10, fontsize=fontsize['text'])
    axs[i].tick_params(axis='x', pad=0.0)
  
  if show_legend:
    
    fig.legend(handles=legend, 
               fontsize=fontsize['legend'], 
               loc='upper left', 
               bbox_transform=axs[0].transAxes, 
               bbox_to_anchor=(0, 1), 
               ncol=3, 
               columnspacing=1, 
               handlelength=1.8)
  plt.tight_layout()
  plt.subplots_adjust(wspace=wspace)
    
def plot_stacked(df: pd.DataFrame, val1, val2, *, 
         y_label, 
         y_lims, 
         calc_val, 
         figsize,
         x_rotation,
         wspace,
         show_legend,
         fontsize):    
  
  fig, axs = plt.subplots(2, 3, sharex=True, sharey=True, figsize=figsize)

  axs[0, 0].set_ylabel(y_label[0], fontsize=fontsize['text'])
  for i, (n, g) in enumerate(df.groupby('arch')):
    df_app = g[g['approach'] == 'app']
    df_phase = g[g['approach'] == 'phase']
    df_kernel = g[g['approach'] == 'kernel']
    vals1 = np.nan_to_num(calc_val(df_app[f'{val1}'].values, df_phase[f'{val1}'].values))
    vals2 = np.nan_to_num(calc_val(df_app[f'{val1}'].values, df_kernel[f'{val1}'].values))
    subplot(axs[0, i], vals1, vals2, df_app['name'].unique(), fontsize=fontsize, y_lims=y_lims)
    axs[0, i].xaxis.set_tick_params(labelbottom=False)
    axs[0, i].tick_params(axis='y', labelsize=fontsize['ticks'])
    axs[0, i].set_ylim(*y_lims)
    axs[0, i].tick_params(axis='x', pad=0.0)
    
  axs[1, 0].set_ylabel(y_label[1], fontsize=fontsize['text'])
  for i, (n, g) in enumerate(df.groupby('arch')):
    df_app = g[g['approach'] == 'app']
    df_phase = g[g['approach'] == 'phase']
    df_kernel = g[g['approach'] == 'kernel']
    vals1 = np.nan_to_num(calc_val(df_app[f'{val2}'].values, df_phase[f'{val2}'].values))
    vals2 = np.nan_to_num(calc_val(df_app[f'{val2}'].values, df_kernel[f'{val2}'].values))
    subplot(axs[1, i], vals1, vals2, df_app['name'].unique(), fontsize=fontsize, y_lims=y_lims)
    axs[1, i].tick_params(axis='x', labelsize=fontsize['text'], rotation=x_rotation)
    axs[1, i].tick_params(axis='y', labelsize=fontsize['ticks'])
    axs[1, i].set_ylim(*y_lims)
    axs[1, i].set_xlabel(f"{n} {hardware[n]}", labelpad=10, fontsize=fontsize['text'])
    axs[1, i].tick_params(axis='x', pad=0.0)
  
  if show_legend:
    fig.legend(handles=legend, 
               fontsize=fontsize['legend'], 
               ncol=3, 
               loc='upper left', 
               bbox_to_anchor=(0, 1), 
               bbox_transform=axs[0, 0].transAxes, 
               columnspacing=1,
               handlelength=1.8)
  plt.tight_layout()
  plt.subplots_adjust(wspace=wspace, hspace=0.1)
  

def parse_dataset(df: pd.DataFrame):
  # print rename each column
  df = df.rename(columns={
    "time[ms]_Average": "time",
    "device_energy[j]_Average": "device_energy",
    "host_energy[j]_Average": "host_energy",
    "time[ms]_Stdev": "time_err",
    "device_energy[j]_Stdev": "device_energy_err",
    "host_energy[j]_Stdev": "host_energy_err"
  })
  df['name'] = df['name'].replace('metropolis', 'mt.polis')
  df['stacked_energy'] = df['device_energy'] + df['host_energy']
  return df

if __name__ == '__main__':
  if len(sys.argv) != 3:
      print(f"Usage: {sys.argv[0]} <dataset> <out_dir>")
      exit(1)
      
  fname = sys.argv[1]
  out_dir = sys.argv[2]

  data = pd.read_csv(fname)
  data = parse_dataset(data)
  
  speedup_func = lambda norm, val: norm/val
  normalized_energy_func = lambda norm, val: val/norm

  time_args = {
    'y_label': 'Speedup',
    'y_lims': (0, 2),
    'calc_val': speedup_func,
    'figsize': (14, 3),
    'show_legend': True,
    'x_rotation': 0,
    'wspace': 0.005,
    'fontsize': {
      'text': 12,
      'numbers': 12,
      'legend': 10,
      'ticks': 10,
    }
  }
  host_device_energy_args = {
    'y_label': ('Device', 'Host'),
    'y_lims': (0, 1.5),
    'calc_val': normalized_energy_func,
    'figsize': (14, 6),
    'show_legend': True,
    'x_rotation': 20,
    'wspace': 0.005,
    'fontsize': {
      'text': 24,
      'numbers': 20,
      'legend': 18,
      'ticks': 16,
    }
  }
  device_energy_args = {
    'y_label': 'Normalized Energy',
    'y_lims': (0, 1.5),
    'calc_val': normalized_energy_func,
    'figsize': (14, 5),
    'show_legend': False,
    'x_rotation': 20,
    'wspace': 0.01,
    'fontsize': {
      'text': 24,
      'numbers': 20,
      'legend': 18,
      'ticks': 20,
    }
  }
  host_energy_args = device_energy_args

  plot(data, 'time', **time_args)
  # plt.show()
  plt.savefig(os.path.join(out_dir, f"two_col_time.pdf"))
  plt.clf()
  one_col_time = device_energy_args
  one_col_time['calc_val'], one_col_time['y_label'], one_col_time['y_lims'] = time_args['calc_val'], time_args['y_label'], time_args['y_lims']
  one_col_time['show_legend'] = True
  plot(data, 'time', **device_energy_args)
  plt.savefig(os.path.join(out_dir, f"one_col_time.pdf"))
  plt.clf()
  # plot(data, "device_energy", **device_energy_args)
  # plt.savefig(os.path.join(out_dir, f"device_energy.pdf"))
  # plt.clf()
  # plot(data, "host_energy", **host_energy_args)
  # plt.savefig(os.path.join(out_dir, f"host_energy.pdf"))
  # plt.clf()
  plot_stacked(data, "device_energy", "host_energy", **host_device_energy_args)
  plt.savefig(os.path.join(out_dir, f"stacked_energy.pdf"))
  plt.clf()
