#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <QRandomGenerator>

class Individual
{
public:
    Individual(double upperBound);
    Individual(double* genes);

    Individual* cross(Individual* secondParent);
    void mutate();
    double* getGenes();
    double getFlexFunction();

private:
    double genes[3];
    double flexFunction;
    QRandomGenerator *rg = QRandomGenerator::global();
};

#endif // INDIVIDUAL_H
