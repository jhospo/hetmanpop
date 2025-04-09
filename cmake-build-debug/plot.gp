set title 'Ewolucja fitness'
set xlabel 'Generacja'
set ylabel 'Fitness'
plot 'fitness_data.txt' using 1:2 with lines title 'Najlepszy', 'fitness_data.txt' using 1:3 with lines title 'Sredni'
pause -1