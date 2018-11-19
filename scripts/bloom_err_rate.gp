reset
set terminal png enhanced font "arial,10" fontscale 1.0 size 600, 400 
set output 'bloom_err_rate.png'
unset key
set view map scale 1
set style data lines
set xtics border in scale 0,0 mirror norotate  autojustify
set ytics border in scale 0,0 mirror norotate  autojustify
set ztics border in scale 0,0 nomirror norotate  autojustify
unset cbtics
set rtics axis in scale 0,0 nomirror norotate  autojustify
set title "Error Rate of bloom filter" 
set xlabel 'size of bloom filter(x50kbits)'
set ylabel 'number of hash funciton'
set yrange [ 0.500000 : 15.50000 ] noreverse nowriteback
set xrange [ 0.500000 : 40.50000 ] noreverse nowriteback
set cblabel "Error rate(The forth root)" 
set cbrange [ 0.00000 : 0.60000 ] noreverse nowriteback
set palette rgbformulae 2, -7, -7
DEBUG_TERM_HTIC = 119
DEBUG_TERM_VTIC = 119

plot 'ref_accuracy.txt' using 1:2:3 with image ,\
log(2)*50000/93827*x
