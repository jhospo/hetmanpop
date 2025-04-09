set terminal png size 1000,600
set output 'plot_pm_0.1.png'
set title 'Ewolucja fitness - pm_0.1'
set xlabel 'Generacja'
set ylabel 'Fitness'
set logscale y
plot 'fitness_data_pm_0.1.txt' using 1:2 with lines title 'Najlepszy', 'fitness_data_pm_0.1.txt' using 1:3 with lines title 'Sredni'
