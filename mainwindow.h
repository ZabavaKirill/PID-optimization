#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDataOnGraph(QVector<double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel);
    void setDataOnGeneralLog(QVector<QString> logs);
    void setDataOnCrossLog(QVector<QString> logs);
    void setDataOnMutationsLog(QVector<QString> logs);

Q_SIGNALS:
    void startAlgorithm(int numOfIndividuals, int numOfCross, int numOfMutations, int numOfIterations, int upperBound, int initialQ1, int initialQ2, int initialQ3, int numOfSteps);
    void buildGraph();
    void setParameters(int K, int T1, int T2, double tau, int L, double dt);

public Q_SLOTS:
    void showErrorMessage(QString errorMessage);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
