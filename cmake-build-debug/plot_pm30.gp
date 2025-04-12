set terminal pngcairo size 1000,600 enhanced font 'Arial,10'
set output 'plot_pm30.png'
set title 'Fitness Evolution - pm30'
set xlabel 'Generation'
set ylabel 'Fitness'
set grid
plot 'fitness_pm30.txt' using 1:2 with lines title 'Best', \
     'fitness_pm30.txt' using 1:3 with lines title 'Average'
