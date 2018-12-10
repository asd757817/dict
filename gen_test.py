import os
import string
rot13 = string.maketrans( 
            "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
                "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")

for i in range(10):
    fi = open("res/cities.txt",'r')
    file_name = "res/case_"+str(i+1)+".txt"
    fo = open(file_name,'w')
    for l in fi.readlines():
        x = l.split(',')[0].strip()
        if( len(x)>i ):
            x = x[:i]+'$'+x[i+1:]
        for j in l.split(',')[1:]:
            x += ','+j.strip()
        fo.write(x+'\n')

