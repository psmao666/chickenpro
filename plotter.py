from bokeh.plotting import figure, show
from bokeh.io import output_notebook
from datetime import datetime
import numpy as np

# Open the text file and read the data
with open('report.txt', 'r') as f:
    data = f.readlines()

# Extract x and y values from the data
x = []
y = []
for line in data:
    values = line.split(',')
    x.append(datetime.strptime(values[0], '%Y-%m-%d %H:%M:%S'))
    y.append(float(values[1]))

# Create a new plot with a title and axis labels
plot = figure(title='Equity Graph', x_axis_label='time', x_axis_type='datetime', y_axis_label='Equity')

# Add a line to the plot
plot.line(x, y, legend_label='Mean Reverse', line_width=2)

# Show the plot in a notebook or browser
show(plot)
