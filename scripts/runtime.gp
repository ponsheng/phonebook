reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:0.070]'output.txt'\
 using 2:xtic(1) with histogram title 'original', \
'' using ($0-0.06):($2+0.0015):2 with labels title ' ', \
'' using 3:xtic(1) with histogram title 'optimized1'  , \
'' using 4:xtic(1) with histogram title 'optimized2' ,\
'' using ($0+0.2):($3+0.0015):3 with labels title ' ', \
'' using ($0+0.4):($4+0.0015):4 with labels title ' '

