#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

class Creature {
public:
    int fitness;
    Creature(int fitness) : fitness(fitness) {}
};

bool survives(const Creature& creature) {
    return creature.fitness > 0;
}

Creature birthOffspring(const Creature& creature) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);

    if (dis(gen) < 0.5) {
        return Creature(creature.fitness + 1);
    } else {
        return Creature(creature.fitness - 1);
    }
}

std::vector<Creature> reproduce(const Creature& creature) {
    std::vector<Creature> children;
    children.push_back(birthOffspring(creature));
    return children;
}

double calculateAverage(const std::vector<Creature>& creatures) {
    double sum = 0;
    for (const auto& creature : creatures) {
        sum += creature.fitness;
    }
    return sum / creatures.size();
}

void printProgressBar(int current, int total) {
    const int barWidth = 50;

    float progress = (float) current / total;
    int pos = barWidth * progress;

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

std::tuple<std::vector<double>, std::vector<int>, std::vector<int>>runSimulation() {
    const int INITIAL_POP_SIZE = 1000;
    const int GENERATIONS = 100000;
    const int STARTING_FITNESS = 20;

    std::vector<Creature> currentCreatures(INITIAL_POP_SIZE, Creature(STARTING_FITNESS));
    std::vector<double> fitnessHistory;
    std::vector<int> minFitness;
    std::vector<int> populationSize;

    for (int gen = 0; gen < GENERATIONS; ++gen) {
        std::vector<Creature> childCreatures;
        while (childCreatures.size() < INITIAL_POP_SIZE) {
            Creature& creature = currentCreatures[std::rand() % currentCreatures.size()];
            if (survives(creature)) {
                std::vector<Creature> children = reproduce(creature);
                childCreatures.insert(childCreatures.end(), children.begin(), children.end());
            }
        }
        currentCreatures = childCreatures;
        fitnessHistory.push_back(calculateAverage(currentCreatures));

        int minFit = std::min_element(currentCreatures.begin(), currentCreatures.end(), 
                                      [](const Creature& a, const Creature& b) { return a.fitness < b.fitness; })->fitness;
        minFitness.push_back(minFit);
        populationSize.push_back(currentCreatures.size());

        printProgressBar(gen, GENERATIONS);
    }

    return {fitnessHistory, minFitness, populationSize};
}

int main() {
    const int SIMULATIONS = 3;
    std::vector<std::vector<double>> fitnessAverages;
    std::vector<std::vector<int>> minFitnessList;
    std::vector<std::vector<int>> populationSizes;

    for (int i = 0; i < SIMULATIONS; ++i) {
        std::tuple<std::vector<double>, std::vector<int>, std::vector<int>> simulationResult = runSimulation();
        fitnessAverages.push_back(std::get<0>(simulationResult));
        minFitnessList.push_back(std::get<1>(simulationResult));
        populationSizes.push_back(std::get<2>(simulationResult));
    }

    // Export the data to files for plotting.
    std::ofstream fitnessFile("fitnessData.csv");
    std::ofstream minFitnessFile("minFitnessData.csv");
    std::ofstream populationSizeFile("populationSizeData.csv");

    // Assume all simulations have the same number of generations.
    for (size_t gen = 0; gen < fitnessAverages[0].size(); ++gen) {
        for (int sim = 0; sim < SIMULATIONS; ++sim) {
            fitnessFile << fitnessAverages[sim][gen] << (sim < SIMULATIONS - 1 ? "," : "\n");
            minFitnessFile << minFitnessList[sim][gen] << (sim < SIMULATIONS - 1 ? "," : "\n");
            populationSizeFile << populationSizes[sim][gen] << (sim < SIMULATIONS - 1 ? "," : "\n");
        }
    }

    return 0;
}
