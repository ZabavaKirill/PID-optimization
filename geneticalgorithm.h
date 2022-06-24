#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#include "individual.h"
#include <QVector>

class GeneticAlgorithm
{

public:
    GeneticAlgorithm();
    GeneticAlgorithm(int populationSize, double upperBound);

    void startAlgorithm(int numOfCross, int numOfMutations, int numOfIterations);
    void addToGeneralLog(QString str);
    QVector<QString> getGeneralLog();
    QVector<QString> getCrossLog();
    QVector<QString> getMutationsLog();
    double* getBestGenes();
    bool populationEmpty();

private:
    double upperBound;
    int populationSize;
    QVector<Individual *> population;
    QVector<QString> generalLog, crossLog, mutationsLog;

    void generatePopulation(int numOfIndividuals, double upperBound);
    void sortPopulation();
    void crossPopulation(int numOfCross);
    void mutatePopulation(int numOfMutations);
    void printPopulation();
};

#endif // GENETICALGORITHM_H
