#include "controller.h"
/*
    Данный класс обеспечивает обмен данными между UI и логикой
*/

Controller::Controller()
{

}

Controller::Controller(MainWindow *w) {
    this->w = w;
    ga = new GeneticAlgorithm();
}


/*
    Соединение сигналов и слотов
*/
void Controller::connectFunctions() {
    connect(w, SIGNAL(setParameters(int, int, int, double, int, double)),
            this, SLOT(onSetParameters(int, int, int, double, int, double)));
    connect(w, SIGNAL(startAlgorithm(int, int, int, int, int, int, int, int, int)),
            this, SLOT(onStartAlgorithm(int, int, int, int, int, int, int, int, int)));
    connect(w, SIGNAL(buildGraph()), this, SLOT(onBuildGraph()));
    connect(this, SIGNAL(errorOccurred(QString)), w, SLOT(showErrorMessage(QString)));
}

void Controller::onSetParameters(int K, int T1, int T2, double tau, int L, double dt) {
    Modeling::setParameters(K,T1,T2,tau,L,dt);
}


/*
    Запуск алгоритма при нажатии кнопки
*/
void Controller::onStartAlgorithm(int numOfIndividuals, int numOfCross, int numOfMutations, int numOfIterations, int upperBound, int initalQ1, int initialQ2, int initialQ3, int numOfSteps) {
    if(numOfCross > numOfIndividuals)
        emit errorOccurred("Количество особей для скрещивания больше общего количества особей в популяции.");
    else {
        ga = new GeneticAlgorithm(numOfIndividuals, upperBound);
        ga->startAlgorithm(numOfCross, numOfMutations, numOfIterations);
        Modeling::optimization(initalQ1, initialQ2, initialQ3, numOfSteps);
        ga->addToGeneralLog("\nПараметры вычисленные методом градиента:");
        ga->addToGeneralLog("Q1 = " + QString::number(Modeling::getOptimalQ1()));
        ga->addToGeneralLog("Q2 = " + QString::number(Modeling::getOptimalQ2()));
        ga->addToGeneralLog("Q3 = " + QString::number(Modeling::getOptimalQ3()));
        ga->addToGeneralLog("Интегральный критерий равен " + QString::number(Modeling::getFlexFunction()) + "\n\n");
        w->setDataOnGeneralLog(ga->getGeneralLog());
        w->setDataOnCrossLog(ga->getCrossLog());
        w->setDataOnMutationsLog(ga->getMutationsLog());
    }
}


/*
    Построение графика при нажатии кнопки
*/
void Controller::onBuildGraph() {
   if(ga->populationEmpty())
        emit errorOccurred("Нет особей в популяции.");
   else {
        QVector<QVector<double>> data = Modeling::getGraphWithPID(ga->getBestGenes()[0], ga->getBestGenes()[1], ga->getBestGenes()[2]);
        w->setDataOnGraph(data.at(0), data.at(1), "t", "h(t)");
    }
}
