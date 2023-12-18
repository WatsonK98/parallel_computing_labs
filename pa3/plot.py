import matplotlib.pyplot as plt

# Number of threads
threads = [2, 4, 8, 16]

# Parallel execution times (seconds)
execution_times = [5.951055, 3.184265, 1.665317, 1.056535]

# Sequential execution time (seconds)
sequential_time = 11.8207247

# Calculate speedup
speedup = [sequential_time / t for t in execution_times]

# Calculate linear speedup
linear_speedup = [t / threads[0] for t in threads]

# Plot the results
plt.plot(threads, speedup, marker='o', label='Actual Speedup')
plt.plot(threads, linear_speedup, linestyle='--', marker='o', label='Linear Speedup')

# Add labels and title
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('Parallel Speedup vs Linear Speedup')
plt.legend()

# Show the plot
plt.savefig('speedup_plot.png')
