import pandas as pd
import argparse

TIME_LABEL = 'time[ms]_Average'
ENERGY_LABEL = 'device_energy[j]_Average'

def get_val(data, approach, label):
  return data[data['approach'] == approach][label].values[0]

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Compute speedup')
  parser.add_argument('csv', type=str, help='CSV file')
  
  args = parser.parse_args()
  
  data = pd.read_csv(args.csv)
  
  print("Speedup")
  for arch, ag, in data.groupby('arch'):
    print("Arch: ", arch)
    for n, g in ag.groupby('name'):
      val = get_val(g, 'kernel', TIME_LABEL) / get_val(g, 'phase', TIME_LABEL)
      print(f" - {n}: {val:.2f}")
  print("Normalized Energy")
  for arch, ag, in data.groupby('arch'):
    print("Arch: ", arch)
    for n, g in ag.groupby('name'):
      val = get_val(g, 'phase', ENERGY_LABEL) / get_val(g, 'kernel', ENERGY_LABEL)
      print(f" - {n}: {val:.2f}")
  
  print("Energy consumption")
  print(f"{'arch':<10}, {'name':<10}, {'kernel_energy':<13}, {'phase_energy':<13}")
  for arch, ag, in data.groupby('arch'):
    for n, g in ag.groupby('name'):
      print(f"{arch:<10}, {n:<10}, {get_val(g, 'kernel', ENERGY_LABEL):<13.2f}, {get_val(g, 'phase', ENERGY_LABEL):<13.2f}")
  