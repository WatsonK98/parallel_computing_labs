#! /usr/bin/env python3

import matplotlib.pyplot as plt 


x_values = [1000,	2000,	4000,	8000,	16000,	32000]
y_values = [.001227,	.006269,	.026665,	.106852,	.429503,	1.757168]
x2_values = [1, 2, 4, 8]
y2_values = [3.764191,1.901525,0.964139,0.502156]

fig, ax1 = plt.subplots()

# Plotting the linear data on the left y-axis
ax1.plot(x_values, y_values, label='Linear', color='b')
ax1.set_xlabel('Input Size')
ax1.set_ylabel('Linear Time (s)', color='b')
ax1.tick_params('y', colors='b')

# Creating a second y-axis for the parallel data
ax2 = ax1.twinx()
ax2.plot(x2_values, y2_values, label='Parallel', color='r')
ax2.set_ylabel('Parallel Time (s)', color='r')
ax2.tick_params('y', colors='r')

# Adding legends
ax1.legend(loc='upper left')
ax2.legend(loc='upper right')

# Saving the plot
plt.savefig('plot.png')