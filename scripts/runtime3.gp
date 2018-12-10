reset
set term png enhanced font 'Verdana,12'
set output 'plot/runtime3.png'
set terminal png size 1280,1024

set multiplot layout 3, 1 title "Total search-time" font ",14"
###
set xlabel 'experiment'
set ylabel 'time(sec)'
set title 'Prefix search, repeat 600 times'
set format x "%5.0f"
set xtics rotate by 45 right
plot [:][:] 'res/bench_cpy.txt' using 1:2 with points lc rgb "blue" title 'cpy 
###
set xlabel 'experiment'
set ylabel 'time(sec)'
set title 'Prefix search with bloom filter,with different parameters'
set format x "%5.0f"
set xtics rotate by 45 right
plot [:][:] 'res/bench_ref.txt' using 1 with points lc rgb "red" title 'ref'
				 
###
set xlabel 'experiment'
set ylabel 'time(sec)'
set title 'comparison'
set format x "%5.0f"
set xtics rotate by 45 right
stats 'bench_cpy.txt' using 2 name 'cpy'

plot [:][:] 'res/bench_cpy.txt' using 1:2 with points lc rgb "blue" title 'cpy',\
				 'res/bench_ref.txt' using 1 with points lc rgb "red" title 'ref',\
				 cpy_mean lw 3 lc rgb "green" title 'prefix search mean time'


