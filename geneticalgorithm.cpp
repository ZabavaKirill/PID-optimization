#include "geneticalgorithm.h"
#include "modeling.h"
#include "individual.h"
#include <QVector>

/*
    Данный класс описывает основные методы генетического алгоритма
*/

GeneticAlgorithm::GeneticAlgorithm()
{

}

GeneticAlgorithm::GeneticAlgorithm(int populationSize, double upperBound) {
    this->populationSize = populationSize;
    this->upperBound = upperBound;
    generalLog.append("Начальная популяция:");
    generatePopulation(populationSize, upperBound);
    sortPopulation();
    printPopulation();
}

/*
    Запуск алгоритма
*/
void GeneticAlgorithm::startAlgorithm(int numOfCross, int numOfMutations, int numOfIterations) {
    mutationsLog.append("Мутирует каждый " + QString::number(numOfMutations));

    for(int i=0;i<numOfIterations;i++) {
        generalLog.append("\nИтерация №" + QString::number(i+1));
        crossLog.append("\nИтерация №" + QString::number(i+1));
        mutationsLog.append("\nИтерация №" + QString::number(i+1));

        crossPopulation(numOfCross);
        mutatePopulation(numOfMutations);
        sortPopulation();
        printPopulation();
    }

    generalLog.append("\n\nКонец алгоритма.");
    generalLog.append("Параметры лучшей особи в популяции:");
    generalLog.append("Q1 = " + QString::number(population.at(0)->getGenes()[0]));
    generalLog.append("Q2 = " + QString::number(population.at(0)->getGenes()[1]));
    generalLog.append("Q3 = " + QString::number(population.at(0)->getGenes()[2]));
    generalLog.append("Интегральный критерий равен " + QString::number(population.at(0)->getFlexFunction()));
}

/*
    Генерация новой популяции
*/
void GeneticAlgorithm::generatePopulation(int numOfIndividuals, double upperBound) {
    for(int i=0;i<numOfIndividuals;i++) {
        population.append(new Individual(upperBound));
    }
}

/*
    Сортировка популяции по возрастанию значения функции приспособленности
*/
void GeneticAlgorithm::sortPopulation() {
    std::sort(population.begin(), population.end(), [](Individual *left, Individual *right) {
        return left->getFlexFunction() < right->getFlexFunction();
    });
}

/*
    Скрещивание популяции
*/
void GeneticAlgorithm::crossPopulation(int numOfCross) {
    QVector<Individual *> temp(population);

    crossLog.append("До скрещивания: " + QString::number(populationSize));

    population.clear();
    populationSize = 0;

    for(int i=0;i<numOfCross-1;i++) {
        for(int j=i+1;j<numOfCross;j++) {
            population.append(temp.at(i)->cross(temp.at(j)));
            crossLog.append("Особь №" + QString::number(i+1) + " + особь №" + QString::number(j+1) + ":\t[" +
                            QString::number(temp.at(i)->getGenes()[0]) + "; " + QString::number(temp.at(i)->getGenes()[1]) + "; " + QString::number(temp.at(i)->getGenes()[2]) + "] + [" +
                    QString::number(temp.at(j)->getGenes()[0]) + "; " + QString::number(temp.at(j)->getGenes()[1]) + "; " + QString::number(temp.at(j)->getGenes()[2]) + "] = [" +
                    QString::number(population.last()->getGenes()[0]) + "; " + QString::number(population.last()->getGenes()[1]) + "; " + QString::number(population.last()->getGenes()[2]) + "]");
            populationSize++;
        }
    }

    crossLog.append("После скрещивания: " + QString::number(populationSize));
}

/*
    Мутация популяции
*/
void GeneticAlgorithm::mutatePopulation(int numOfMutations) {
    double tempGenes[3], tempFlex;
    QVector<Individual *> temp(population);
    for(int i=numOfMutations-1;i<populationSize;i+=numOfMutations) {
        for(int j=0;j<3;j++) {
            tempGenes[j] = population.at(i)->getGenes()[j];
        }
        tempFlex = population.at(i)->getFlexFunction();

        population.at(i)->mutate();
        mutationsLog.append("Мутирует особь №" + QString::number(i+1) + ":\t[" +
                            QString::number(tempGenes[0]) + "; " + QString::number(tempGenes[1]) + "; " + QString::number(tempGenes[2]) + "] --> [" +
                            QString::number(population.at(i)->getGenes()[0]) + "; " + QString::number(population.at(i)->getGenes()[1]) + "; " + QString::number(population.at(i)->getGenes()[2]) + "];\n\t\tI=" +
                            QString::number(tempFlex) + " --> " + QString::number(population.at(i)->getFlexFunction()));
       }
}

void GeneticAlgorithm::printPopulation() {
    for(int i=0;i<populationSize;i++) {
        generalLog.append(QString("Особь " + QString::number(i+1) + ":\t[" + QString::number(population.at(i)->getGenes()[0]) + "; " + QString::number(population.at(i)->getGenes()[1]) + "; " + QString::number(population.at(i)->getGenes()[2]) + "] \tI = " + QString::number(population.at(i)->getFlexFunction())));
    }
}

void GeneticAlgorithm::addToGeneralLog(QString str) {
    generalLog.append(str);
}

QVector<QString> GeneticAlgorithm::getGeneralLog() {
    return generalLog;
}

QVector<QString> GeneticAlgorithm::getCrossLog() {
    return crossLog;
}

QVector<QString> GeneticAlgorithm::getMutationsLog() {
    return  mutationsLog;
}

double* GeneticAlgorithm::getBestGenes() {
    return population.at(0)->getGenes();
}

bool GeneticAlgorithm::populationEmpty() {
    return population.empty();
}
