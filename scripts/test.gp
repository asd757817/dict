clear
reset
set xlabel 'search time(nsec)'
set ylabel 'count'
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'test.png'

stats 'bench_cpy.txt' using 2 name 'cpy'
stats 'bench_ref.txt' using 2 name 'ref'

set arrow 1 from cpy_mean,graph 0.5 to cpy_mean,graph 0
set label 1 at cpy_mean, graph 0.5 "mean cpy" center offset 3,1
set arrow 2 from ref_mean,graph 0.5 to ref_mean,graph 0 
set label 2 at ref_mean, graph 0.5 "mean ref" center offset 4,-1 

set boxwidth 1.0 absolute
#set style fill solid 1.0 noborder
bin_width = 1;
bin_number(x) = floor(x/bin_width)
rounded(x) = bin_width * ( bin_number(x) + 0.5  )
plot [0:2][0:]'bench_cpy.txt' using (rounded($2)):(1) smooth freq with boxes title 'cpy',\
'bench_ref.txt' using (rounded($2)):(1) smooth freq with boxes title 'ref',\
