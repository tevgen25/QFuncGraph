#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "muParser.h"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    functionInput = new QLineEdit(this);
    functionInput->setPlaceholderText("Введите функцию, например: sin(x) + x*x");

    domainInput = new QLineEdit(this);
    domainInput->setPlaceholderText("Область определения: x_min x_max, например: -5 5");

    calculateButton = new QPushButton("Рассчитать", this);
    plotButton = new QPushButton("Построить график", this);

    plotWidget = new QCustomPlot(this);
    plotWidget->xAxis->setLabel("x");
    plotWidget->yAxis->setLabel("y");

    QSharedPointer<QCPAxisTickerFixed> tickerX = QSharedPointer<QCPAxisTickerFixed>::create();
    tickerX->setTickStep(1.0);
    plotWidget->xAxis->setTicker(tickerX);

    QSharedPointer<QCPAxisTickerFixed> tickerY = QSharedPointer<QCPAxisTickerFixed>::create();
    tickerY->setTickStep(1.0);
    plotWidget->yAxis->setTicker(tickerY);

    plotWidget->setBackground(Qt::white);
    plotWidget->axisRect()->setupFullAxesBox();
    plotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    QWidget *inputPanel = new QWidget(this);
    QVBoxLayout *inputPanelLayout = new QVBoxLayout(inputPanel);
    inputPanelLayout->addWidget(new QLabel("Функция:"));
    inputPanelLayout->addWidget(functionInput);
    inputPanelLayout->addWidget(new QLabel("Область определения (x_min x_max):"));
    inputPanelLayout->addWidget(domainInput);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(plotButton);
    inputPanelLayout->addLayout(buttonLayout);

    inputPanel->setMaximumHeight(150);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(inputPanel);
    mainLayout->addWidget(plotWidget);

    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 4);

    connect(calculateButton, &QPushButton::clicked, this, &AppWindow::onCalculateClicked);
    connect(plotButton, &QPushButton::clicked, this, &AppWindow::onPlotClicked);
}

void AppWindow::onCalculateClicked()
{
    QString funcStr = functionInput->text().trimmed();
    QString domainStr = domainInput->text().trimmed();

    if (funcStr.isEmpty() || domainStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля.");
        return;
    }

    QStringList parts = domainStr.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Ошибка", "Область должна содержать два числа: x_min x_max");
        return;
    }

    bool ok1, ok2;
    double xMin = parts[0].toDouble(&ok1);
    double xMax = parts[1].toDouble(&ok2);
    if (!ok1 || !ok2 || xMin >= xMax) {
        QMessageBox::warning(this, "Ошибка", "Некорректная область определения.");
        return;
    }

    try {
        mu::Parser parser;
        double var_x = 0.0;
        parser.DefineVar("x", &var_x);
        parser.SetExpr(funcStr.toStdString());

        xValues.clear();
        yValues.clear();
        const int N = 500;
        double step = (xMax - xMin) / (N - 1);
        for (int i = 0; i < N; ++i) {
            var_x = xMin + i * step;
            double y = parser.Eval();
            xValues.push_back(var_x);
            yValues.push_back(y);
        }
        QMessageBox::information(this, "Успех", "Функция рассчитана.");
    } catch (mu::Parser::exception_type &e) {
        QMessageBox::critical(this, "Ошибка", QString::fromStdString(e.GetMsg()));
    }
}

void AppWindow::onPlotClicked()
{
    if (xValues.empty() || yValues.empty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала рассчитайте значения функции.");
        return;
    }

    plotWidget->clearGraphs();
    plotWidget->addGraph();
    plotWidget->graph(0)->setData(
        QVector<double>(xValues.begin(), xValues.end()),
        QVector<double>(yValues.begin(), yValues.end())
    );
    plotWidget->graph(0)->setPen(QPen(Qt::blue, 2));
    plotWidget->rescaleAxes();
    plotWidget->replot();
}
