from bokeh.plotting import figure, show
from bokeh.io import output_notebook
import numpy as np

# Open the text file and read the data
with open('hisotry.txt', 'r') as f:
    data = f.readlines()

# Extract x and y values from the data
x = []
y = []
for line in data:
    values = line.split()
    x.append(float(values[0]))
    y.append(float(values[1]))

# Create a new plot with a title and axis labels
plot = figure(title='My Line Chart', x_axis_label='X Axis', y_axis_label='Y Axis')

# Add a line to the plot
plot.line(x, y, legend_label='Line 1', line_width=2)

# Show the plot in a notebook or browser
show(plot)
