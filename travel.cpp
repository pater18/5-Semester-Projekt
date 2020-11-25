#include "travel.h"
#include <time.h>
#include <exception>



double travel::calcPath(std::vector<int> order)
{

    double distance = 0;
    for (size_t j = 0; j < order.size() - 1; j++) {
        long long int index;
        long long int nextIndex;

        if (order[j] < order[j + 1]) {
            index = order[j];
            nextIndex = order[j + 1];
        } else {
            index = order[j + 1];
            nextIndex = order[j];
        }

        for (size_t k = 0; k < lookupTable[index - 1].distance.size(); k++) {
            if (lookupTable[index - 1].distance[k].first == nextIndex) {
                distance += lookupTable[index - 1].distance[k].second;
                break;
            }
        }
    }

    if (distance < overallBest) {
        overallBest = distance;
        overallBestPath = order;
    }
    if (distance == 0)
        std::cout << "Distance er ikke stor nok" << std::endl;
    return distance;
}

std::vector<int> travel::calcPathLexi(std::vector<std::vector<int> > &comb, std::vector<location> &lookup)
{
    double bestRoute = std::numeric_limits<double>::infinity();
    std::vector<std::vector<int>> bestCombination;
    bestCombination.push_back(comb[0]);


    for (size_t i = 0; i < comb.size(); i++) {
        double distance = 0;
        for (size_t j = 0; j < comb[i].size() - 2; j++) {
            long long int index;
            long long int nextIndex;

            if (comb[i][j] < comb[i][j + 1]) {
                index = comb[i][j];
                nextIndex = comb[i][j + 1];
            } else {
                index = comb[i][j + 1];
                nextIndex = comb[i][j];
            }

            if (index < 0) {
                index = -16;
            }

            for (size_t k = 0; k < lookup[index - 1].distance.size(); k++) {
                if (lookup[index - 1].distance[k].first == nextIndex) {
                    distance += lookup[index - 1].distance[k].second;
                    break;
                }
            }
        }
        if (distance < bestRoute) {
            bestRoute = distance;
            bestCombination[0] = comb[i];
        }
    }

    overallBest = bestRoute;
    return bestCombination[0];
}


void travel::createPopulation(int num)
{
    for (int i = 0; i < num; i++) {
        population.push_back(initialPath);
        std::random_shuffle(initialPath.begin(), initialPath.end());
    }
}

void travel::calculateFitness()
{
    fitness.clear();
    for (size_t i = 0; i < population.size(); i++) {
        double d = calcPath(population[i]);
        double fitnessScore = (1 / d) * 100000;
        fitness.push_back(fitnessScore);
    }

}

void travel::normalizeFitness()
{
    double sum = 0;
    for (size_t i = 0; i < fitness.size(); i++) {
        sum += fitness[i];
    }
    for (size_t i = 0; i < fitness.size(); i++) {
        fitness[i] = fitness[i] / sum;
    }
}


std::vector<int> travel::pickOne(std::vector<std::vector<int> > population, std::vector<double> fitness)
{
    int index = 0;
    double r = (rand() % 100) / 100.0;

    while (r > 0) {
        r = r - fitness[index++];
    }
    index--;
//    try {
    if (index < 0) {
        return population[0];
    }
        return population[index];
//    } catch (std::exception& e) {
//        std::cout << "Cannot return index: " << index << " " << e.what() << std::endl;
//    }

}

void travel::mutate(std::vector<int> order, int mutationRate)
{
    int indexA = rand() % order.size();
    int indexB = rand() % order.size();
    std::swap(order[indexA], order[indexB]);
}


void travel::nextGeneration()
{
    std::vector<std::vector<int>> newPopulation;
    std::vector<std::vector<int>> oldPopulation = population;
    std::vector<int> order;
    population.clear();

    for (size_t i = 0; i < oldPopulation.size(); i++) {
        order = pickOne(oldPopulation, fitness);
        mutate(order, 1);
        newPopulation.push_back(order);
    }

    population = newPopulation;
}

void travel::lexi()
{
    double distance = calcPath(initialPath);
    std::next_permutation(initialPath.begin(), initialPath.end());
    if (distance < overallBest) {
        overallBest = distance;
        overallBestPath = initialPath;
    }

}
