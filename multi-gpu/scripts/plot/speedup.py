import pandas as pd
import argparse
import numpy as np

def parse(df):
  for val in df['name']:
    original_val = val
    val = val.replace('miniweather_', '').replace('cloverleaf_', '')
    df['name'] = df['name'].replace(original_val, val)
  return df
      

def get_val(df, approach, value, metric='mean'):
  return df[df['name'] == approach][f'{metric}_{value}'].values[0]

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Compute speedup')
  parser.add_argument('mini_csv', type=str, help='CSV file')
  parser.add_argument('clover_csv', type=str, help='CSV file')
  parser.add_argument('-mt', type=str, default='median', choices=['mean', 'max', 'median'], help='Time Metric for MiniWeather')
  parser.add_argument('-me', type=str, default='median', choices=['mean', 'max', 'median'], help='Energy Metric for MiniWeather')
  parser.add_argument('-ct', type=str, default='median', choices=['mean', 'max', 'median'], help='Time Metric for CloverLeaf')
  parser.add_argument('-ce', type=str, default='max', choices=['mean', 'max', 'median'], help='Energy Metric for CloverLeaf')
  
  args = parser.parse_args()
  
  mini_df = parse(pd.read_csv(args.mini_csv))
  clover_df = parse(pd.read_csv(args.clover_csv))
  
  mini_time_metric = args.mt
  mini_energy_metric = args.me
  clover_time_metric = args.ct
  clover_energy_metric = args.ce
  
  print('--------------------' * 3)
  print('MiniWeather')
  print('--------------------' * 3)
  print('Speedup | Fine-Grained/Phase-Based: ', get_val(mini_df, 'per_kernel', 'time', mini_time_metric) / get_val(mini_df, 'no_hiding', 'time', mini_time_metric))
  print('Speedup | Fine-Grained/Phase-MPI: ', get_val(mini_df, 'per_kernel', 'time', mini_time_metric) / get_val(mini_df, 'hiding', 'time', mini_time_metric))
  print('Speedup | Phase-Based/Phase-MPI: ', get_val(mini_df, 'no_hiding', 'time', mini_time_metric) / get_val(mini_df, 'hiding', 'time', mini_time_metric))
  print('Norm. Energy | Phase-Based/Fine-Grained: ', get_val(mini_df, 'no_hiding', 'energy', mini_energy_metric) / get_val(mini_df, 'per_kernel', 'energy', mini_energy_metric))
  print('Norm. Energy | Phase-MPI/Fine-Grained: ', get_val(mini_df, 'hiding', 'energy', mini_energy_metric) / get_val(mini_df, 'per_kernel', 'energy', mini_energy_metric))
  print('Norm. Energy | Phase-MPI/Phase-Based: ', get_val(mini_df, 'hiding', 'energy', mini_energy_metric) / get_val(mini_df, 'no_hiding', 'energy', mini_energy_metric))
  print('--------------------' * 3)
  print('CloverLeaf')
  print('--------------------' * 3)
  print('Speedup | Fine-Grained/Phase-Based: ', get_val(clover_df, 'per_kernel', 'time', clover_time_metric) / get_val(clover_df, 'no_hiding', 'time', clover_time_metric))
  print('Speedup | Fine-Grained/Phase-MPI: ', get_val(clover_df, 'per_kernel', 'time', clover_time_metric) / get_val(clover_df, 'hiding', 'time', clover_time_metric))
  print('Speedup | Phase-Based/Phase-MPI: ', get_val(clover_df, 'no_hiding', 'time', clover_time_metric) / get_val(clover_df, 'hiding', 'time', clover_time_metric))
  print('Norm. Energy | Phase-Based/Fine-Grained: ', get_val(clover_df, 'no_hiding', 'energy', clover_energy_metric) / get_val(clover_df, 'per_kernel', 'energy', clover_energy_metric))
  print('Norm. Energy | Phase-MPI/Fine-Grained: ', get_val(clover_df, 'hiding', 'energy', clover_energy_metric) / get_val(clover_df, 'per_kernel', 'energy', clover_energy_metric))
  print('Norm. Energy | Phase-MPI/Phase-Based: ', get_val(clover_df, 'hiding', 'energy', clover_energy_metric) / get_val(clover_df, 'no_hiding', 'energy', clover_energy_metric))
  
  
