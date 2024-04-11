import re
import statistics
import sys
import numpy as np

# Function to calculate Z-scores
def calculate_z_score(data):
    mean = np.mean(data)
    std_dev = np.std(data)
    z_scores = (data - mean) / std_dev
    return z_scores


# Regular expression patterns to extract Total time and Total energy
time_pattern = re.compile(r'Total time chrono\[ms\]: (\d+\.?\d*)')
energy_pattern = re.compile(r'Total energy: (\d+\.?\d*)')

# Lists to store extracted values
total_times = []
total_energies = []

# Read the file
with open(sys.argv[1], 'r') as file:
    for line in file:
        # Extract Total time
        time_match = time_pattern.search(line)
        if time_match:
            total_time = float(time_match.group(1))
            total_times.append(total_time)

        # Extract Total energy
        energy_match = energy_pattern.search(line)
        if energy_match:
            total_energy = float(energy_match.group(1))
            total_energies.append(total_energy)


# Convert lists to NumPy arrays
total_times = np.array(total_times)
total_energies = np.array(total_energies)
# Calculate Z-scores for time and energy
z_scores_time = calculate_z_score(total_times)
z_scores_energy = calculate_z_score(total_energies)

# Define threshold for outliers (e.g., Z-score greater than 3 or less than -3)
outlier_threshold = 3

# Identify outliers
outliers_time = total_times[np.abs(z_scores_time) > outlier_threshold]
outliers_energy = total_energies[np.abs(z_scores_energy) > outlier_threshold]
# Remove outliers from the data
total_times = total_times[np.abs(z_scores_time) <= outlier_threshold]
total_energies = total_energies[np.abs(z_scores_energy) <= outlier_threshold]

# Calculate mean and median
mean_time = statistics.mean(total_times)
median_time = statistics.median(total_times)
std_time = statistics.stdev(total_times)

mean_energy = statistics.mean(total_energies)
median_energy = statistics.median(total_energies)
std_energy = statistics.stdev(total_energies)


# Output results
print("Mean Total time: {:.2f} ms".format(mean_time))
print("Median Total time: {:.2f} ms".format(median_time))
print("Stddev time: {:.2f} ms".format(std_time))


print("Mean Total energy: {:.2f} j".format(mean_energy))
print("Median Total energy: {:.2f} j".format(median_energy))
print("Stddev energy: {:.2f} j".format(std_energy))
