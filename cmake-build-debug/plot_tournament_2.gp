set terminal png size 1000,600
set output 'plot_tournament_2.png'
set title 'Ewolucja fitness - tournament_2'
set xlabel 'Generacja'
set ylabel 'Fitness'
set logscale y
plot 'fitness_data_tournament_2.txt' using 1:2 with lines title 'Najlepszy', 'fitness_data_tournament_2.txt' using 1:3 with lines title 'Sredni'
