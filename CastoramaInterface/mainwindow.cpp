#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dataBase=new CDataBase("/home/pulinka/Desktop/cpp/Castorama/itemki.db");

    m_customPlot = ui->m_Plot;
    for(auto &i : m_dataBase->GetNames())
        ui->ListsOfObjects->addItem(QString::fromStdString(i));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ListsOfObjects_itemActivated(QListWidgetItem *item)
{
    m_customPlot->clearGraphs();

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d.M.");
    m_customPlot->xAxis->setTicker(dateTicker);
    QVector<double> xData;
    QVector<double> yData;
      std::vector <std::pair<int, QDateTime>> data=m_dataBase->GetData(item->text().toStdString());
        for(auto i : data )
        {
            QCPGraphData temp;
           xData.push_back(i.second.toSecsSinceEpoch());
            yData.push_back(i.first);
        }

       m_customPlot->addGraph();
       m_customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));       m_customPlot->graph(0)->setData(xData, yData);
       m_customPlot->xAxis->setRange(xData.first()-86400, xData.last()+86400);

       double Max = std::numeric_limits<double>::min();
       double Min = std::numeric_limits<double>::max();

       foreach (double p, yData) {
           Max = qMax(Max, p);
           Min = qMin(Min,p);
       }
       m_customPlot->yAxis->setRange(Min*0.96,Max*1.05);

       m_customPlot->xAxis->setLabel("data");
       m_customPlot->yAxis->setLabel("Ilość");

       m_customPlot->replot();
}



