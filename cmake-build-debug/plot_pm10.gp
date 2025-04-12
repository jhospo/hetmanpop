set terminal pngcairo size 1000,600 enhanced font 'Arial,10'
set output 'plot_pm10.png'
set title 'Fitness Evolution - pm10'
set xlabel 'Generation'
set ylabel 'Fitness'
set grid
plot 'fitness_pm10.txt' using 1:2 with lines title 'Best', \
     'fitness_pm10.txt' using 1:3 with lines title 'Average'
