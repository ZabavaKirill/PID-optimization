#include "modeling.h"
#include <QVector>
#include <QtMath>

/*
    Данный класс отвечает за моделирование звена, а так же за вычисление параметров ПИД-регулятора методом градиентного спуска
*/

double Modeling::k;
double Modeling::T1;
double Modeling::T2;
double Modeling::tau;
double Modeling::l;
double Modeling::dt;
double Modeling::flexFunction;
double Modeling::optimalQ1;
double Modeling::optimalQ2;
double Modeling::optimalQ3;


/*
    Установка параметров звена
*/
void Modeling::setParameters(double k, double T1, double T2, double tau, double l, double dt) {
    Modeling::k = k;
    Modeling::T1 = T1;
    Modeling::T2 = T2;
    Modeling::tau = tau;
    Modeling::l = l;
    Modeling::dt = dt;
}

/*
    Моделирование звена с заданными параметрами ПИД-регулятора
*/
QVector <QVector <double>> Modeling::getGraphWithPID(double q1, double q2, double q3) {
    QVector <QVector <double>> data;
        QVector <double> x(int(l/dt)), delay(int(l/dt));
        QVector <double> y;
        double std = 0, err, prevErr=0, in=1, out=0, y1=0, y2=0, I=0, ev=0, PIDoutput;
        int ns = tau/dt, i=0;
        while(std < l) {
            err = in - out;
            ev += std::abs(err)*dt;
            I += err*dt;
            PIDoutput = q1*err + q2*I + q3*(err - prevErr)/dt;  //выход пид регулятора

            y1 += RungeKutta(1/T1, k/T1, PIDoutput, y1);
            y2 += RungeKutta(1/T2, 1/T2, y1, y2);

            y.append(y2);

            if(i > ns) {   //запаздывание
                out = y.at(i-ns);
            }
            else
                out = 0;
            i++;

            x.append(std);
            delay.append(out);

            prevErr = err;
            std += dt;
        }

        data.append(x);
        data.append(delay);

        return data;
}

/*
    Метод Рунге-Кутты для апериодического звена 1го порядка
*/
double Modeling::RungeKutta(double a0, double alpha, double x, double z) {
    double K1, K2, K3, K4;

    K1 = (-a0*z + alpha*x)*dt;
    K2 = (-a0*(z + K1/2) + alpha*x)*dt;
    K3 = (-a0*(z + K2/2) + alpha*x)*dt;
    K4 = (-a0*(z + K3) + alpha*x)*dt;

    return (K1 + 2*K2 + 2*K3 + K4)/6;
}

/*
    Вычисление оптимальных ПИД-регулятора параметров методом градиентного спуска
*/
void Modeling::optimization(double q1, double q2, double q3, int numOfSteps) {
    QVector <double> xVector, evVector, yVector, xi1Vector, xi2Vector, xi3Vector, dI1Vector, dI2Vector, dI3Vector, q1Vector, q2Vector, q3Vector,
            i1DelayVector(int(l/dt)), xi2DelayVector(int(l/dt)), xi3DelayVector(int(l/dt));
    double std, err, prevErr, in=1, out,
            y1, y2, y11, y12, y13, y21, y22, y23,
            I, IXi1, IXi2, IXi3,
            PIDoutput,
            xi1, prevXi1,
            xi2, prevXi2,
            xi3, prevXi3,
            dI1, dI2, dI3,
            du1, du2, du3,
            h=0.03, ev, prevEv=0,
            prevQ1, prevQ2, prevQ3;
    int ns = tau/dt, i, step=0;

    do
    {
        std=0, out=0, prevErr=0;
        y1=0, y2=0, y11=0, y12=0, y13=0, y21=0, y22=0, y23=0;
        I=0, IXi1=0, IXi2=0, IXi3=0;
        xi1=0, prevXi1=0;
        xi2=0, prevXi2=0;
        xi3=0, prevXi3=0;
        dI1=0, dI2=0, dI3=0;
        ev=0;
        i=0;
        yVector.clear();
        xi1Vector.clear();
        xi2Vector.clear();
        xi3Vector.clear();
        while(std < l) {
            err = in - out;
            ev += std::abs(err) * dt;

            I += err*dt;
            IXi1 += xi1*dt;
            IXi2 += xi2*dt;
            IXi3 += xi3*dt;

            PIDoutput = q1*err + q2*I + q3*(err - prevErr)/dt;                  //пид регулятор

            du1 = err - q1*xi1 - q2*IXi1 - q3*(xi1-prevXi1)/dt;                 //du/dq1
            du2 = -q1*xi2 + I - q2*IXi2 - q3*(xi2-prevXi2)/dt;                  //du/dq2
            du3 = -q1*xi3 - q2*IXi3 + (err-prevErr)/dt - q3*(xi3-prevXi3)/dt;   //du/dq3

            y1 += RungeKutta(1/T1, k/T1, PIDoutput, y1);       //вычисление функций чувствительности
            y2 += RungeKutta(1/T2, 1/T2, y1, y2);

            y11 += RungeKutta(1/T1, k/T1, du1, y11);
            y21 += RungeKutta(1/T2, 1/T2, y11, y21);

            y12 += RungeKutta(1/T1, k/T1, du2, y12);
            y22 += RungeKutta(1/T2, 1/T2, y12, y22);

            y13 += RungeKutta(1/T1, k/T1, du3, y13);
            y23 += RungeKutta(1/T2, 1/T2, y13, y23);

            yVector.append(y2);
            xi1Vector.append(y21);
            xi2Vector.append(y22);
            xi3Vector.append(y23);

            prevXi1 = xi1;
            prevXi2 = xi2;
            prevXi3 = xi3;

            if(i > ns) {
                xi1 = xi1Vector.at(i-ns);
                xi2 = xi2Vector.at(i-ns);
                xi3 = xi3Vector.at(i-ns);
                out = yVector.at(i-ns);
            }
            else {
                xi1 = 0;
                xi2 = 0;
                xi3 = 0;
                out = 0;
            }
            i++;

            dI1 += (-2 * err * xi1) * dt;
            dI2 += (-2 * err * xi2) * dt;
            dI3 += (-2 * err * xi3) * dt;

            if(err >= 0) {          //вычисление dI1/dq1, dI1/dq2, dI1/dq3
                dI1 += -xi1 * dt;
                dI2 += -xi2 * dt;
                dI3 += -xi3 * dt;
            }
            else {
                dI1 += xi1 * dt;
                dI2 += xi2 * dt;
                dI3 += xi3 * dt;
            }

            prevErr = err;
            std += dt;
        }

        prevQ1 = q1;
        prevQ2 = q2;
        prevQ3 = q3;

        q1 -= h*dI1/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));         //вычисление новых параметров Q
        q2 -= h*dI2/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));
        q3 -= h*dI3/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));

        if(q1 < 0)
            q1 = prevQ1;
        if(q2 < 0)
            q2 = prevQ2;
        if(q3 < 0)
            q3 = prevQ3;

        if(ev <= prevEv)
            h *= 1.5;
        else
            h *= 0.5;

        prevEv = ev;
        step++;
    } while(step <= numOfSteps);

    optimalQ1 = q1;
    optimalQ2 = q2;
    optimalQ3 = q3;
    flexFunction = ev;
}

/*
    Вычисление модульного интегрального критерия при заданных параметрах ПИД-регулятора
*/
double Modeling::getIntegralCriterion(double q1, double q2, double q3) {
    QVector <double> y;
    double std = 0, err, prevErr=0, in=1, out=0, y1=0, y2=0, I=0, ev=0, PIDoutput;
    int ns = tau/dt, i=0;

    while(std < l) {
        err = in - out;
        ev += std::abs(err)*dt;
        I += err*dt;
        PIDoutput = q1*err + q2*I + q3*(err - prevErr)/dt;  //выход пид регулятора

        y1 += RungeKutta(1/T1, k/T1, PIDoutput, y1);
        y2 += RungeKutta(1/T2, 1/T2, y1, y2);

        y.append(y2);

        if(i > ns) {   //запаздывание
            out = y.at(i-ns);
        }
        else
            out = 0;
        i++;

        prevErr = err;
        std += dt;
    }
    return ev;
}

double Modeling::getOptimalQ1() {
    return optimalQ1;
}

double Modeling::getOptimalQ2() {
    return optimalQ2;
}

double Modeling::getOptimalQ3() {
    return optimalQ3;
}

double Modeling::getFlexFunction() {
    return flexFunction;
}
