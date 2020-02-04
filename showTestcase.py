import matplotlib.pyplot as plt
import os
file = open('C:\\Users\\kevin\\Documents\\GitHub\\Corner-Stitch\\test5','r')
###################################################################################################################
fig = plt.subplot(1,1,1)
fig.set_title("original_same_color")
line = file.readline()
color_list=['b','g','r','y','k']
color_index=0
num_line=0
while line:
    if line[0] == '#':
        print("ignore")
        line = file.readline()
        continue
    print("line ",line)
    line_split=line.split(" ")
    line_split_f=[float(x) for x in line_split]
    line_split_f.append(line_split_f[0])
    line_split_f.append(line_split_f[1])
    for i in range(0,len(line_split_f)-3,2):
        point = [(line_split_f[i], line_split_f[i+1]), (line_split_f[i+2], line_split_f[i+3])]
        print("p: ", point)
        (xpoint, ypoint) = zip(*point)
        fig.plot(xpoint, ypoint,color_list[color_index])
    line = file.readline()
plt.show() 
file.close()
