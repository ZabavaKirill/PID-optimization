#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
    Главное окно приложения
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
    Построение графика
*/
void MainWindow::setDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel) {
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(xAxis, yAxis);
    ui->widget->graph(0)->setPen(QPen(Qt::red, 2));
    ui->widget->xAxis->setLabel(xLabel);
    ui->widget->yAxis->setLabel(yLabel);
    ui->widget->rescaleAxes();
    ui->widget->replot();
}


/*
    Вывод информации о результатах алгоритма
*/
void MainWindow::setDataOnGeneralLog(QVector<QString> logs) {
    ui->GeneralLog->clear();
    foreach(QString str, logs) {
        ui->GeneralLog->append(str);
    }
}

/*
    Вывод информации о скрещиваниях
*/
void MainWindow::setDataOnCrossLog(QVector<QString> logs) {
    ui->CrossLog->clear();
    foreach(QString str, logs) {
        ui->CrossLog->append(str);
    }
}

/*
    Вывод информации о мутациях
*/
void MainWindow::setDataOnMutationsLog(QVector<QString> logs) {
    ui->MutationsLog->clear();
    foreach(QString str, logs) {
        ui->MutationsLog->append(str);
    }
}

/*
    Вывод ошибки
*/
void MainWindow::showErrorMessage(QString errorMessage) {
    QErrorMessage *error = new QErrorMessage(this);
    error->setWindowTitle("Ошибка");
    error->showMessage("Во время выполнения программы возникла ошибка:\n" + errorMessage);
}

/*
    Нажатие кнпоки "Запустить алгоритм"
*/
void MainWindow::on_pushButton_clicked()
{
    emit setParameters(ui->K_spinBox->value(), ui->T1_spinBox->value(), ui->T2_spinBox->value(), ui->Tau_doubleSpinBox->value(), ui->L_spinBox->value(), ui->Dt_doubleSpinBox->value());
    emit startAlgorithm(ui->NumOfIndividuals_spinBox->value(), ui->NumOfCross_spinBox->value(), ui->NumOfMutations_spinBox->value(), ui->NumOfIterations_spinBox->value(), ui->UpperBound_spinBox->value(), ui->InitialQ1_spinBox->value(), ui->InitialQ2_spinBox->value(), ui->InitialQ3_spinBox->value(), ui->NumOfCross_spinBox->value());
}

/*
    Нажатие кнпоки "Построить график"
*/
void MainWindow::on_pushButton_2_clicked()
{
    emit setParameters(ui->K_spinBox->value(), ui->T1_spinBox->value(), ui->T2_spinBox->value(), ui->Tau_doubleSpinBox->value(), ui->L_spinBox->value(), ui->Dt_doubleSpinBox->value());
    emit buildGraph();
}
