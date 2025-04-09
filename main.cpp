#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

int calculate_attacks(const vector<pair<int, int>> &individual) {
    int attacks = 0;
    int n = individual.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (individual[i].first == individual[j].first ||
                individual[i].second == individual[j].second ||
                abs(individual[i].first - individual[j].first) == abs(individual[i].second - individual[j].second)) {
                attacks++;
            }
        }
    }
    return attacks;
}

vector<int> evaluate_population(const vector<vector<pair<int, int>>> &population) {
    vector<int> fitness;
    for (const auto &individual : population) {
        fitness.push_back(calculate_attacks(individual));
    }
    return fitness;
}

vector<vector<pair<int, int>>> selection(const vector<vector<pair<int, int>>> &population,
                                         const vector<int> &fitness, int tournament_pool, int pop_size) {
    vector<vector<pair<int, int>>> new_population;
    for (int i = 0; i < pop_size; i++) {
        int best_index = -1;
        int best_fit = INT_MAX;
        for (int j = 0; j < tournament_pool; j++) {
            int idx = rand() % pop_size;
            if (fitness[idx] < best_fit) {
                best_fit = fitness[idx];
                best_index = idx;
            }
        }
        new_population.push_back(population[best_index]);
    }
    return new_population;
}

void crossover(vector<vector<pair<int, int>>> &population, double pc) {
    int pop_size = population.size();
    int n = population[0].size();
    for (int i = 0; i < pop_size - 1; i += 2) {
        double r = (double)rand() / RAND_MAX;
        if (r <= pc) {
            int k = 1 + rand() % (n - 1);
            vector<pair<int, int>> child1, child2;
            for (int j = 0; j < k; j++) {
                child1.push_back(population[i][j]);
                child2.push_back(population[i + 1][j]);
            }
            for (int j = k; j < n; j++) {
                child1.push_back(population[i + 1][j]);
                child2.push_back(population[i][j]);
            }
            population[i] = child1;
            population[i + 1] = child2;
        }
    }
}

void mutation(vector<vector<pair<int, int>>> &population, double pm, int n) {
    int pop_size = population.size();
    for (int i = 0; i < pop_size; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r <= pm) {
            int gene_index = rand() % n;
            int coord = rand() % 2;
            int new_val = 1 + rand() % n;
            if (coord == 0)
                population[i][gene_index].first = new_val;
            else
                population[i][gene_index].second = new_val;
        }
    }
}

string board_to_string(const vector<pair<int, int>> &queens, int n) {
    vector<vector<char>> board(n, vector<char>(n, '.'));
    for (const auto &q : queens) {
        board[q.first - 1][q.second - 1] = 'Q';
    }
    stringstream ss;
    for (const auto &row : board) {
        for (char c : row) ss << c << " ";
        ss << "\n";
    }
    return ss.str();
}

void search(int n, int pop_size, int tournament_pool, int genmax, double pc, double pm, const string& tag) {
    srand(time(0));

    vector<vector<pair<int, int>>> population;
    for (int i = 0; i < pop_size; i++) {
        vector<pair<int, int>> individual;
        for (int j = 0; j < n; j++) {
            int x = 1 + rand() % n;
            int y = 1 + rand() % n;
            individual.push_back(make_pair(x, y));
        }
        population.push_back(individual);
    }

    vector<int> fitness = evaluate_population(population);
    int best_index = 0;
    int best_fitness = fitness[0];
    for (int i = 1; i < pop_size; i++) {
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            best_index = i;
        }
    }

    vector<int> bestHistory;
    vector<double> avgHistory;
    bestHistory.push_back(best_fitness);

    double avg = 0.0;
    for (int f : fitness) avg += f;
    avg /= pop_size;
    avgHistory.push_back(avg);

    int generation = 0;

    while (generation < genmax && best_fitness > 0) {
        auto new_population = selection(population, fitness, tournament_pool, pop_size);
        crossover(new_population, pc);
        mutation(new_population, pm, n);
        fitness = evaluate_population(new_population);

        for (int i = 0; i < pop_size; i++) {
            if (fitness[i] < best_fitness) {
                best_fitness = fitness[i];
                best_index = i;
            }
        }

        avg = 0.0;
        for (int f : fitness) avg += f;
        avg /= pop_size;

        generation++;
        bestHistory.push_back(best_fitness);
        avgHistory.push_back(avg);

        population = new_population;
    }

    const auto &bestIndividual = population[best_index];

    ofstream result("results_" + tag + ".txt");
    result << "Najlepszy osobnik (fitness = " << best_fitness << "):\n";
    for (const auto &q : bestIndividual)
        result << "(" << q.first << ", " << q.second << ") ";
    result << "\n\nReprezentacja szachownicy:\n";
    result << board_to_string(bestIndividual, n);
    result.close();

    // Also print to console
    cout << "\nWyniki dla konfiguracji: " << tag << endl;
    cout << "Najlepszy osobnik (fitness = " << best_fitness << "):" << endl;
    for (const auto &q : bestIndividual)
        cout << "(" << q.first << ", " << q.second << ") ";
    cout << "\n\nReprezentacja szachownicy:\n";
    cout << board_to_string(bestIndividual, n) << endl;

    string data_file = "fitness_data_" + tag + ".txt";
    ofstream file(data_file);
    for (int i = 0; i < bestHistory.size(); i++) {
        file << i << " " << bestHistory[i] << " " << avgHistory[i] << "\n";
    }
    file.close();

    string plot_file = "plot_" + tag + ".gp";
    string img_file = "plot_" + tag + ".png";

    ofstream gp(plot_file);
    gp << "set terminal png size 1000,600\n";
    gp << "set output '" << img_file << "'\n";
    gp << "set title 'Ewolucja fitness - " << tag << "'\n";
    gp << "set xlabel 'Generacja'\n";
    gp << "set ylabel 'Fitness'\n";
    gp << "set logscale y\n";
    gp << "plot '" << data_file << "' using 1:2 with lines title 'Najlepszy', '"
       << data_file << "' using 1:3 with lines title 'Sredni'\n";
    gp.close();

    system(("gnuplot " + plot_file).c_str());
}

int main() {
    /*
     Wnioski z eksperymentów
    Rozmiar turnieju wpływa na tempo ewolucji. Zbyt mały powoduje zbyt dużą losowość,
     a zbyt duży powoduje utrate różnorodności. Najlepsze wyniki przy wartości 5.

    Liczność populacji ma znaczenie dla jakości rozwiązań. Mała populacja często prowadzi do
     utknięcia w lokalnym optimum. Populacja 30 zapewnia dobry kompromis między jakością a czasem.

    Prawdopodobieństwo mutacji (pm) powinno być umiarkowane. Niskie wartości spowalniają,
     a zbyt wysokie wprowadzają chaos. Najlepsze efekty dawało pm = 0.3.

    Prawdopodobieństwo krzyżowania (pc) powinno być wysokie, ale nie maksymalne.
     pc = 0.8 umożliwia skuteczną wymianę informacji między osobnikami bez rozwalania populacji.
     */
    search(8, 30, 5, 2000, 0.8, 0.3, "default");
    search(8, 30, 2, 2000, 0.8, 0.3, "tournament_2");
    search(8, 30, 10, 2000, 0.8, 0.3, "tournament_10");
    search(8, 10, 5, 2000, 0.8, 0.3, "pop_10");
    search(8, 50, 5, 2000, 0.8, 0.3, "pop_50");
    search(8, 30, 5, 2000, 0.8, 0.1, "pm_0.1");
    search(8, 30, 5, 2000, 0.8, 0.6, "pm_0.6");
    search(8, 30, 5, 2000, 0.5, 0.3, "pc_0.5");
    search(8, 30, 5, 2000, 0.95, 0.3, "pc_0.95");
    return 0;
}