import matplotlib.pyplot as plt
import pandas as pd

# Load the three CSV files, treating periods as thousand separators
try:
    df_in = pd.read_csv('aggregated-memory-in-experiment-results.csv', thousands='.')
    df_out = pd.read_csv('aggregated-memory-out-experiment-results.csv', thousands='.')
    df_benchmark = pd.read_csv('aggregated-memory-in-experiment-benchmarkABI-results.csv', thousands='.')
except FileNotFoundError as e:
    print(f"Error: {e}")
    exit()

# Keep only the columns relevant to the means
expected_columns = ['Block Size (MB)', 'Allocation_Time_Mean', 'Write_Time_Mean', 'Read_Time_Mean', 'Free_Time_Mean']
df_in = df_in[expected_columns]
df_out = df_out[expected_columns]
df_benchmark = df_benchmark[expected_columns]

# Rename columns for clarity
df_in.columns = ['Block Size (MB)', 'Allocation Time (ms)', 'Write Time (ms)', 'Read Time (ms)', 'Free Time (ms)']
df_out.columns = ['Block Size (MB)', 'Allocation Time (ms)', 'Write Time (ms)', 'Read Time (ms)', 'Free Time (ms)']
df_benchmark.columns = ['Block Size (MB)', 'Allocation Time (ms)', 'Write Time (ms)', 'Read Time (ms)', 'Free Time (ms)']

# Set the index to Block Size (MB)
df_in_means = df_in.set_index('Block Size (MB)')
df_out_means = df_out.set_index('Block Size (MB)')
df_benchmark_means = df_benchmark.set_index('Block Size (MB)')

# Create a figure with 4 subplots (2x2) organized for each time type
fig, axs = plt.subplots(2, 2, figsize=(16, 12))  # Increased height

# Plot Allocation Time
axs[0, 0].plot(df_in_means.index, df_in_means['Allocation Time (ms)'], label='Secure - Purecap ABI', marker='o', linestyle='-')
axs[0, 0].plot(df_out_means.index, df_out_means['Allocation Time (ms)'], label='Normal', marker='s', linestyle='--')
axs[0, 0].plot(df_benchmark_means.index, df_benchmark_means['Allocation Time (ms)'], label='Secure - Purecap Benchmark ABI', marker='^', linestyle=':')
axs[0, 0].set_title('Allocation Time')
axs[0, 0].set_xlabel('Block Size (MB)')
axs[0, 0].set_ylabel('Time (ms)')
axs[0, 0].legend(loc='upper left')

# Plot Write Time
axs[0, 1].plot(df_in_means.index, df_in_means['Write Time (ms)'], label='Secure - Purecap ABI', marker='o', linestyle='-')
axs[0, 1].plot(df_out_means.index, df_out_means['Write Time (ms)'], label='Normal', marker='s', linestyle='--')
axs[0, 1].plot(df_benchmark_means.index, df_benchmark_means['Write Time (ms)'], label='Secure - Purecap Benchmark ABI', marker='^', linestyle=':')
axs[0, 1].set_title('Write Time')
axs[0, 1].set_xlabel('Block Size (MB)')
axs[0, 1].set_ylabel('Time (ms)')
axs[0, 1].legend(loc='upper left')

# Plot Read Time
axs[1, 0].plot(df_in_means.index, df_in_means['Read Time (ms)'], label='Secure - Purecap ABI', marker='o', linestyle='-')
axs[1, 0].plot(df_out_means.index, df_out_means['Read Time (ms)'], label='Normal', marker='s', linestyle='--')
axs[1, 0].plot(df_benchmark_means.index, df_benchmark_means['Read Time (ms)'], label='Secure - Purecap Benchmark ABI', marker='^', linestyle=':')
axs[1, 0].set_title('Read Time')
axs[1, 0].set_xlabel('Block Size (MB)')
axs[1, 0].set_ylabel('Time (ms)')
axs[1, 0].legend(loc='upper left')

# Plot Free Time
axs[1, 1].plot(df_in_means.index, df_in_means['Free Time (ms)'], label='Secure - Purecap ABI', marker='o', linestyle='-')
axs[1, 1].plot(df_out_means.index, df_out_means['Free Time (ms)'], label='Normal', marker='s', linestyle='--')
axs[1, 1].plot(df_benchmark_means.index, df_benchmark_means['Free Time (ms)'], label='Secure - Purecap Benchmark ABI', marker='^', linestyle=':')
axs[1, 1].set_title('Free Time')
axs[1, 1].set_xlabel('Block Size (MB)')
axs[1, 1].set_ylabel('Time (ms)')
axs[1, 1].legend(loc='upper left')

# Adjust layout for better readability
plt.tight_layout(pad=4.0)  # Adjusted padding for better spacing

# Save the adjusted graph
plt.savefig('performancememOperations_benchmarkABI_comparasion.png')

# Display the plot
plt.show()

