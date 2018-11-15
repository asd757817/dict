set terminal png enhanced font "arial,10" fontscale 1.0 size 600, 400 
set output 'heatmaps.png'
unset key
set view map scale 1
set style data lines
set xtics border in scale 0,0 mirror norotate  autojustify
set ytics border in scale 0,0 mirror norotate  autojustify
set ztics border in scale 0,0 nomirror norotate  autojustify
unset cbtics
set rtics axis in scale 0,0 nomirror norotate  autojustify
set title "Error Rate of bloom filter" 
set xlabel 'size of bloom filter'
set ylabel 'number of hash funciton'
set xrange [ -0.500000 : 1350000.50000 ] noreverse nowriteback
set yrange [ -0.500000 : 15.50000 ] noreverse nowriteback
set cblabel "Error rate" 
set cbrange [ 0.00000 : 5.00000 ] noreverse nowriteback
set palette rgbformulae 2, -7, -7
DEBUG_TERM_HTIC = 119
DEBUG_TERM_VTIC = 119

plot '../bloom_error_rate.txt' using 2:1:3 with image
