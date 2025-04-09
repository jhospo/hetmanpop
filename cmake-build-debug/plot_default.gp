set terminal png size 1000,600
set output 'plot_default.png'
set title 'Ewolucja fitness - default'
set xlabel 'Generacja'
set ylabel 'Fitness'
set logscale y
plot 'fitness_data_default.txt' using 1:2 with lines title 'Najlepszy', 'fitness_data_default.txt' using 1:3 with lines title 'Sredni'
