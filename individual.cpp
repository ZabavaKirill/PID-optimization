#include "individual.h"
#include "modeling.h"
#include <QList>

/*
    Данный класс описывает отдельную особь
*/

Individual::Individual(double* genes) {
    for(int i=0;i<3;i++) {
        this->genes[i] = genes[i];
    }

    this->flexFunction = Modeling::getIntegralCriterion(genes[0], genes[1], genes[2]);
}

Individual::Individual(double upperBound) {
    do {
        for(int i=0;i<3;i++) {
            this->genes[i] = rg->bounded(upperBound);
        }
    } while(qAbs(genes[0] - genes[1]) > 10 || qAbs(genes[0] - genes[2]) > 10 || qAbs(genes[1] - genes[2]) > 10 );
    this->flexFunction = Modeling::getIntegralCriterion(genes[0], genes[1], genes[2]);
}

/*
    Скрещивание особей
*/
Individual* Individual::cross(Individual* secondParent) {
    double newGenes[3];
    std::copy(std::begin(genes), std::end(genes), std::begin(newGenes));
    int numOfGenes = rg->bounded(1,4), genID;
    QList<int> crossedGenes;

    for(int i=0;i<numOfGenes;i++) {
        do {
            genID = rg->bounded(0,3);
        } while(crossedGenes.contains(genID));

        crossedGenes.append(genID);
       if(this->genes[genID] < secondParent->getGenes()[genID]) {
            newGenes[genID] += (secondParent->getGenes()[genID] - this->genes[genID])/2;
        }
        else {
            newGenes[genID] -= (this->genes[genID] - secondParent->getGenes()[genID])/2;
        }
    }

    return new Individual(newGenes);
}

/*
    Мутация особей
*/
void Individual::mutate() {
    double newGenes[3], mutatedNum;
    std::copy(std::begin(genes), std::end(genes), std::begin(newGenes));
    int numOfGenes = rg->bounded(1,4), genID, percent;
    QList<int> mutatedGenes;

    for(int i=0;i<numOfGenes;i++) {
        do {
            genID = rg->bounded(0,3);
        } while(mutatedGenes.contains(genID));

        mutatedGenes.append(genID);
        percent = rg->bounded(0, 101);

        if(percent <= 40) {
            if(rg->bounded(0,101) <= 50)
                mutatedNum = rg->bounded(0,3) + rg->generateDouble();
            else {
                do {
                    mutatedNum = rg->bounded(0,3) - rg->generateDouble();
                } while(mutatedNum < 0);
            }

            newGenes[genID] += mutatedNum;
        }
        else if(percent > 40 && percent <= 80) {
            do {
                if(rg->bounded(0,101) <= 50)
                    mutatedNum = rg->bounded(0,3) + rg->generateDouble();
                else {
                    do {
                        mutatedNum = rg->bounded(0,3) - rg->generateDouble();
                    } while(mutatedNum < 0);
                }
            } while(newGenes[genID] - mutatedNum <= 0);
        }
        else if(percent > 80 && percent <= 95) {
            mutatedNum = rg->bounded(1, 2) + rg->generateDouble();
            newGenes[genID] *= mutatedNum;
        }
        else if(percent > 95 && percent <= 100) {
            mutatedNum = rg->bounded(1, 2) + rg->generateDouble();
            newGenes[genID] /= mutatedNum;
        }
    }

    for(int i=0;i<3;i++)
        genes[i] = newGenes[i];
    flexFunction = Modeling::getIntegralCriterion(newGenes[0],newGenes[1],newGenes[2]);
}

double* Individual::getGenes() {
    return genes;
}

double Individual::getFlexFunction() {
    return flexFunction;
}
