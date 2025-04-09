set terminal png size 1000,600
set output 'plot_pc_0.5.png'
set title 'Ewolucja fitness - pc_0.5'
set xlabel 'Generacja'
set ylabel 'Fitness'
set logscale y
plot 'fitness_data_pc_0.5.txt' using 1:2 with lines title 'Najlepszy', 'fitness_data_pc_0.5.txt' using 1:3 with lines title 'Sredni'
