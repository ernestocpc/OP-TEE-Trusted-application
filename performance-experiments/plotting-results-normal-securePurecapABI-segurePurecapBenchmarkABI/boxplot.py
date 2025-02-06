import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.ticker import ScalarFormatter, LogLocator

# Load data from CSV files
df_in = pd.read_csv('aggregated-memory-in-experiment-results.csv', thousands='.')
df_out = pd.read_csv('aggregated-memory-out-experiment-results.csv', thousands='.')
df_benchmark = pd.read_csv('aggregated-memory-in-experiment-benchmarkABI-results.csv', thousands='.')

# Rename columns to include means and standard deviations
columns = [
    'Block Size (MB)', 'Allocation_Time_Mean', 'Allocation_Time_Std', 
    'Write_Time_Mean', 'Write_Time_Std', 'Read_Time_Mean', 
    'Read_Time_Std', 'Free_Time_Mean', 'Free_Time_Std'
]
df_in.columns = columns
df_out.columns = columns
df_benchmark.columns = columns

# Prepare data for the Box Plot
data_to_plot = {
    "Allocation Time": [df_in['Allocation_Time_Mean'], df_benchmark['Allocation_Time_Mean'], df_out['Allocation_Time_Mean']],
    "Write Time": [df_in['Write_Time_Mean'], df_benchmark['Write_Time_Mean'], df_out['Write_Time_Mean']],
    "Read Time": [df_in['Read_Time_Mean'], df_benchmark['Read_Time_Mean'], df_out['Read_Time_Mean']],
    "Free Time": [df_in['Free_Time_Mean'], df_benchmark['Free_Time_Mean'], df_out['Free_Time_Mean']]
}

# Define titles and limits for the plots
titles = ['Allocation Time', 'Write Time', 'Read Time', 'Free Time']

# Create the figure with subplots
fig, axs = plt.subplots(2, 2, figsize=(14, 8))

# Iterate through the time types and adjust scales
for ax, (key, data) in zip(axs.ravel(), data_to_plot.items()):
    # Updated labels with line breaks to avoid visual clutter
    ax.boxplot(data, labels=[
        'Secure -\nPurecap ABI', 
        'Secure -\nPurecap Benchmark', 
        'Normal'
    ])
    ax.set_title(key)
    ax.set_yscale('log')  # Apply logarithmic scale
    ax.set_ylabel('Time (ms)')
    ax.yaxis.set_major_locator(LogLocator(base=10.0, numticks=10))
    ax.yaxis.set_minor_locator(LogLocator(base=10.0, subs='auto', numticks=10))

# Adjust layout for better readability
plt.tight_layout()

# Save the adjusted graph
plt.savefig('boxplot_allocate_rd_wr_free_mem_benchmarkABI_comparasion.png')

# Display the plots
plt.show()

