set terminal pngcairo size 1000,600 enhanced font 'Arial,10'
set output 'plot_pop30.png'
set title 'Fitness Evolution - pop30'
set xlabel 'Generation'
set ylabel 'Fitness'
set grid
plot 'fitness_pop30.txt' using 1:2 with lines title 'Best', \
     'fitness_pop30.txt' using 1:3 with lines title 'Average'
