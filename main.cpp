#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <numeric>
#include <string>
#include <algorithm>

using namespace std;

int calculate_attacks(const vector<pair<int, int>> &state) {
    int attacks = 0, n = state.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (state[i].first == state[j].first ||
                state[i].second == state[j].second ||
                abs(state[i].first - state[j].first) == abs(state[i].second - state[j].second))
                attacks++;
    return attacks;
}

vector<vector<pair<int, int>>> initialize_population(int popSize, int n) {
    vector<vector<pair<int, int>>> population(popSize, vector<pair<int, int>>(n));
    for (auto &state : population)
        for (auto &q : state)
            q = {1 + rand() % n, 1 + rand() % n};
    return population;
}

vector<int> evaluate(const vector<vector<pair<int, int>>> &P) {
    vector<int> fitness;
    for (auto &state : P)
        fitness.push_back(calculate_attacks(state));
    return fitness;
}

vector<vector<pair<int, int>>> selection(const vector<vector<pair<int, int>>> &P, const vector<int> &fitness, int tournamentSize) {
    vector<vector<pair<int, int>>> Pn;
    int n = P.size();
    for (int i = 0; i < n; i++) {
        int best = rand() % n;
        for (int j = 1; j < tournamentSize; j++) {
            int idx = rand() % n;
            if (fitness[idx] < fitness[best]) best = idx;
        }
        Pn.push_back(P[best]);
    }
    return Pn;
}

void crossover(vector<vector<pair<int, int>>> &Pn, double pc, int n) {
    for (int i = 0; i < Pn.size() - 1; i += 2) {
        if ((double)rand() / RAND_MAX > pc) continue;
        int point = 1 + rand() % (n - 1);
        for (int j = point; j < n; j++) {
            swap(Pn[i][j], Pn[i + 1][j]);
        }
    }
}

void mutation(vector<vector<pair<int, int>>> &Pn, double pm, int n) {
    for (auto &state : Pn) {
        if ((double)rand() / RAND_MAX <= pm) {
            int gene = rand() % n;
            if (rand() % 2)
                state[gene].first = 1 + rand() % n;
            else
                state[gene].second = 1 + rand() % n;
        }
    }
}

void generate_plot(const string &dataFile, const string &tag) {
    string gpfile = "plot_" + tag + ".gp";
    string img = "plot_" + tag + ".png";
    ofstream gp(gpfile);
    gp << "set terminal pngcairo size 1000,600 enhanced font 'Arial,10'\n";
    gp << "set output '" << img << "'\n";
    gp << "set title 'Fitness Evolution - " << tag << "'\n";
    gp << "set xlabel 'Generation'\n";
    gp << "set ylabel 'Fitness'\n";
    gp << "set grid\n";
    gp << "plot '" << dataFile << "' using 1:2 with lines title 'Best', \\\n";
    gp << "     '" << dataFile << "' using 1:3 with lines title 'Average'\n";
    gp.close();
    system(("gnuplot " + gpfile).c_str());
}

void evolutionary_algorithm(int n, int popSize, int tournamentSize, int genmax, double pc, double pm, const string &tag) {
    srand(time(0));
    auto P = initialize_population(popSize, n);
    auto fitness = evaluate(P);

    int bestFitness = *min_element(fitness.begin(), fitness.end());
    int bestIndex = min_element(fitness.begin(), fitness.end()) - fitness.begin();
    vector<pair<int, int>> beststate = P[bestIndex];

    vector<double> bestHistory = {(double)bestFitness};
    vector<double> avgHistory = {accumulate(fitness.begin(), fitness.end(), 0.0) / popSize};

    for (int gen = 1; gen <= genmax && bestFitness > 0; gen++) {
        auto Pn = selection(P, fitness, tournamentSize);
        Pn[0] = beststate; // Elitism
        crossover(Pn, pc, n);
        mutation(Pn, pm, n);
        fitness = evaluate(Pn);

        int generationBest = *min_element(fitness.begin(), fitness.end());
        if (generationBest < bestFitness) {
            bestFitness = generationBest;
            bestIndex = min_element(fitness.begin(), fitness.end()) - fitness.begin();
            beststate = Pn[bestIndex];
        }

        bestHistory.push_back(bestFitness);
        avgHistory.push_back(accumulate(fitness.begin(), fitness.end(), 0.0) / popSize);
        P = Pn;
    }

    cout << "\n=== Wyniki: " << tag << " ===\n";
    cout << "Najlepszy fitness: " << bestFitness << "\nPozycje hetmanów:\n";
    for (auto &q : beststate) cout << "(" << q.first << "," << q.second << ") ";
    cout << "\n";

    string dataFile = "fitness_" + tag + ".txt";
    ofstream out(dataFile);
    for (int i = 0; i < bestHistory.size(); i++)
        out << i << " " << bestHistory[i] << " " << avgHistory[i] << "\n";
    out.close();

    generate_plot(dataFile, tag);
}

int main() {
    int n = 8;
    int genmax = 1000;
    double pc = 0.8;
    double pm = 0.3;

    vector<int> tournamentSizes = {2, 5, 10};
    vector<int> popSizes = {10, 30, 50};
    vector<double> mutationRates = {0.1, 0.3, 0.6};

    for (int t : tournamentSizes)
        evolutionary_algorithm(n, 30, t, genmax, pc, pm, "ts" + to_string(t));

    for (int p : popSizes)
        evolutionary_algorithm(n, p, 5, genmax, pc, pm, "pop" + to_string(p));

    for (double m : mutationRates)
        evolutionary_algorithm(n, 30, 5, genmax, pc, m, "pm" + to_string(int(m * 100)));

    return 0;
}
