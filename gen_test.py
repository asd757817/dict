import os
import string
rot13 = string.maketrans( 
            "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
                "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")

# for i in range(10):
if '__main__' == __name__ :
    repeat_time = 5

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

    fo = open("repeat_%d.txt" % repeat_time,'w')
    for l in fi.readlines():
        content.append(l.strip())
    content.sort()
    content = content[:repeat_time]
    for i in range(100000//repeat_time):
       for l in content:
           x = l.split(',')[0].strip()
           if( len(x)>i ):
               x = x[:i]+'$'+x[i+1:]
           for j in l.split(',')[1:]:
               x += ','+j.strip()
           fo.write(x+'\n')  
    '''
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
    '''
