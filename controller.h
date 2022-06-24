#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "modeling.h"
#include "geneticalgorithm.h"
#include "mainwindow.h"


class Controller : public QObject
{

Q_OBJECT

public:
    Controller();
    Controller(MainWindow *w);
    void connectFunctions();

Q_SIGNALS:
    void errorOccurred(QString errorMessage);

private Q_SLOTS:
    void onStartAlgorithm(int numOfIndividuals, int numOfCross, int numOfMutations, int numOfIterations,
                          int upperBound, int initialQ1, int initialQ2, int initialQ3, int numOfSteps);
    void onSetParameters(int K, int T1, int T2, double tau, int L, double dt);
    void onBuildGraph();

private:
    GeneticAlgorithm *ga;
    MainWindow *w;
};

#endif // CONTROLLER_H
