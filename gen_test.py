import os
import string
rot13 = string.maketrans( 
            "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
                "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")

for i in range(10):
    fi = open("cities.txt",'r')
    content = []
    '''
    file_name = "res/case_"+i+".txt"
    fo = open(file_name,'w')
    for l in fi.readlines():
        x = l.split(',')[0].strip()
        if( len(x)>i ):
            x = x[:i]+'$'+x[i+1:]
        for j in l.split(',')[1:]:
            x += ','+j.strip()
        fo.write(x+'\n')
    '''

    #  fo = open("sorted_cities.txt",'w')
    #  for l in fi.readlines():
        #  content.append(l.strip())
    #  content.sort()
    #  for l in content:
        #  x = l.split(',')[0].strip()
        #  #  if( len(x)>i ):
            #  #  x = x[:i]+'$'+x[i+1:]
        #  for j in l.split(',')[1:]:
            #  x += ','+j.strip()
        #  fo.write(x+'\n')
    
    fo = open("case_final.txt",'w')
    for l in fi.readlines():
        content.append(l.strip())
    content.sort()
    for l in content:
        x = l.split(',')[0].strip()
        if( len(x)>i ):
            x = x[:-1]+'$'
        for j in l.split(',')[1:]:
            x += ','+j.strip()
        #print x    
        fo.write(x+'\n')
