import pandas as pd
import re
import argparse
import os
import glob
from collections import defaultdict
import statistics
from types import SimpleNamespace

names = {
  "hiding.log": "Phase-Aware + Hiding",
  "no_hiding.log": "Phase-Aware",
  "per_app.log": "Coarse-Grained",
  "per_kernel.log": "Fine-Grained"
}

time_pattern = re.compile(r'hydro_time \[ms\]: (\d+)')
energy_pattern = re.compile(r'Total energy: (\d+.?\d*)')

def parse_file(fname):
  with open(fname) as f:
    data = f.read()
    
  times = list(map(int, time_pattern.findall(data)))
  energies = list(map(float, energy_pattern.findall(data)))
  
  # z_score_times = [(t - statistics.mean(times)) / statistics.stdev(times) for t in times]
  # z_score_energies = [(e - statistics.mean(energies)) / statistics.stdev(energies) for e in energies]  
  
  # times = [t for t, z in zip(times, z_score_times) if -3 <= z <= 3]
  # energies = [e for e, z in zip(energies, z_score_energies) if -3 <= z <= 3]
  
  res = {
    'mean_time': statistics.mean(times),
    'median_time': statistics.median(times),
    'min_time': min(times),
    'max_time': max(times),
    'std_time': statistics.stdev(times),
    'mean_energy': statistics.mean(energies),
    'median_energy': statistics.median(energies),
    'min_energy': min(energies),
    'max_energy': max(energies),
    'std_energy': statistics.stdev(energies)
  }
  
  return res
  

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Extract frequency from CloverLeaf logs')
  parser.add_argument('log_dir', type=str, help='Log directory to parse')
  parser.add_argument('output', type=str, help='Output CSV file')
  
  args = parser.parse_args()
  
  log_dir = os.path.abspath(args.log_dir)
  output_file = os.path.abspath(args.output)
  
  df = None
  
  for file in glob.glob(f'{log_dir}/*.log'):
    print(file)
    parsed_file = parse_file(file)
    fname = os.path.basename(file).split('.')[0]
    parsed_file['name'] = fname

    if df is None:
      df = pd.DataFrame([parsed_file])
    else:
      df = pd.concat([df, pd.DataFrame([parsed_file])])

  df.to_csv(output_file, index=False)
    