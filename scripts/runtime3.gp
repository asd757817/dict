reset
set xlabel 'experiment index'
set ylabel 'time(sec)'
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime3.png'
set format x "%10.0f"
set xtics rotate by 45 right

plot [:][:] 'bench_cpy.txt' using 1:2 with points title 'cpy',\
				 'bench_ref.txt' using 1:2 with points title 'ref',\







