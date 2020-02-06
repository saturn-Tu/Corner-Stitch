import matplotlib.pyplot as plt
import os
file = open('C:\\Users\\kevin\\Documents\\GitHub\\Corner-Stitch\\output.txt','r')
###################################################################################################################
fig = plt.subplot(1,1,1)
fig.set_title("original_same_color")
line = file.readline()
color_list=['b','r']
color_index=0
num_line=0
print("line, ", line)
x_list=[]
y_list=[]
while line:
    line_split=line.split(" ")
    if line_split[0] == 'TILE':
        color_index = 1
    else:
        color_index = 0
    line_split.pop(0)
    line_split_f=[float(x) for x in line_split]
    for i in range(0,len(line_split)-3,2):
        point = [(line_split_f[i], line_split_f[i+1]), (line_split_f[i+2], line_split_f[i+3])]
        (xpoint, ypoint) = zip(*point)
        if color_index == 1:
            x_list.append(xpoint)
            y_list.append(ypoint)
        else:
            fig.plot(xpoint, ypoint,color_list[color_index])
    line = file.readline()

for i in range( len(x_list) ):
    fig.plot(x_list[i], y_list[i], color_list[1])
plt.show() 
file.close()
