#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "qcustomplot.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);

private slots:
    void onCalculateClicked();
    void onPlotClicked();

private:
    QLineEdit *functionInput;
    QLineEdit *domainInput;
    QPushButton *calculateButton;
    QPushButton *plotButton;
    QCustomPlot *plotWidget;
    std::vector<double> xValues;
    std::vector<double> yValues;

};
