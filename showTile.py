import matplotlib.pyplot as plt
import os
file = open('C:\\Users\\kevin\\Documents\\GitHub\\Corner-Stitch\\output.txt','r')
###################################################################################################################
fig = plt.subplot(1,1,1)
fig.set_title("original_same_color")
line = file.readline()
color_list=['b','g','r','y','k']
color_index=0
num_line=0
print("line, ", line)
while line:
    num_line=num_line+1
    print("process line:",num_line)
    line_split=line.split(" ")
    line_split.pop(-1)
    line_split.pop(0)
    line_split_f=[float(x) for x in line_split]
    for i in range(0,len(line_split)-3,2):
        point = [(   line_split_f[i], line_split_f[i+1]), (line_split_f[i+2], line_split_f[i+3])]
        (xpoint, ypoint) = zip(*point)
        ##color_index=int(num_line/300);
        print("print")
        fig.plot(xpoint, ypoint,color_list[color_index])
    color_index=color_index+1
    if(color_index==5):
        color_index=0
    print('sleep')
    line = file.readline()
plt.show() 
file.close()
