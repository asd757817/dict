import os
import string
rot13 = string.maketrans( 
            "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
                "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")

'''
fo = open("t.txt",'w')
for l in fi.readlines():
    w_line = ""
    for x in l.split(','):
        x = x.strip()
        i=x[-1:]
        x=x[:-1]+string.translate(i,rot13)
        w_line += ','+x
    fo.write(w_line.strip(',')+'\n')
'''

for i in range(10):
    fi = open("cities.txt",'r')
    file_name = "case_"+str(i+1)+".txt"
    fo = open(file_name,'w')
    for l in fi.readlines():
        x = l.split(',')[0].strip()
        if( len(x)>i ):
            x = x[:i]+'$'+x[i+1:]
        for j in l.split(',')[1:]:
            x += ','+j.strip()
        fo.write(x+'\n')

