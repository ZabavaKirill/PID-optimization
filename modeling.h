#ifndef MODELING_H
#define MODELING_H
#include <QVector>

class Modeling
{
public:
    static void setParameters(double k, double T1, double T2, double tau, double l, double dt);
    static QVector <QVector <double>> getGraphWithPID(double q1,  double q2, double q3);
    static void optimization(double q1, double q2, double q3, int numOfSteps);
    static double getIntegralCriterion(double q1, double q2, double q3);
    static double getOptimalQ1();
    static double getOptimalQ2();
    static double getOptimalQ3();
    static double getFlexFunction();

private:
    static double k, T1, T2, tau, l, dt, optimalQ1, optimalQ2, optimalQ3, flexFunction;
    static double RungeKutta(double a0, double alpha, double x, double z);

};

#endif // MODELING_H
