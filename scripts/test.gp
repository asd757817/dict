clear
reset
set xlabel 'search time(nsec)'
set ylabel 'count'
set auto x
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'plot/test.png'

stats 'res/bench_cpy.txt' using 2 name 'cpy'
stats 'res/bench_ref.txt' using 1 name 'ref'

set arrow 1 from cpy_mean,graph -0.1 to cpy_mean,graph 0 filled back lw 3 lc rgb "blue"
set label "mean cpy" at 0,graph -0.10 center offset -1,0 textcolor "blue" 
set arrow 2 from ref_mean,graph -0.1 to ref_mean,graph 0 filled back lw 3 lc rgb "red"
set label "mean ref" at 0,graph -0.10 center offset -1,1 textcolor "red" 

set boxwidth 0.01 absolute
#set style fill solid 1.0 noborder
bin_width = 0.01;
bin_number(x) = floor(x/bin_width)
rounded(x) = bin_width * ( bin_number(x) + 0 )

plot [0:][0:]'res/bench_cpy.txt' using (rounded($2)):(1) smooth freq with boxes title 'cpy',\
'res/bench_ref.txt' using (rounded($3)):(1) smooth freq with boxes title 'ref',\

