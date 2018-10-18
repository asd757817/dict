import os
import string
rot13 = string.maketrans( 
            "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
                "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")

fi = open("cities.txt",'r')
fo = open("t.txt",'w')
for l in fi.readlines():
    w_line = ""
    for x in l.split(','):
        x = x.strip()
        i=x[-1:]
        x=x[:-1]+string.translate(i,rot13)
        w_line += ','+x
    fo.write(w_line.strip(',')+'\n')


