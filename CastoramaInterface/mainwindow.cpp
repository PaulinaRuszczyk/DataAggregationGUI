#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cinfowindow.h"
#include "cinsertwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dataBase=new CDataBase("/home/pulinka/Desktop/cpp/Castorama/itemki.db");

    m_customPlot = ui->m_Plot;
    m_customPlot->setMouseTracking(true);
    connect(m_customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
    ui->m_searchBox->setText("Szukaj...");
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
       m_customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
       m_customPlot->graph(0)->setData(xData, yData);
       m_customPlot->xAxis->setRange(*std::min_element(xData.constBegin(), xData.constEnd())-86400, *std::max_element(xData.constBegin(), xData.constEnd())+86400);

       double Max = std::numeric_limits<double>::min();
       double Min = std::numeric_limits<double>::max();

       foreach (double p, yData) {
           Max = qMax(Max, p);
           Min = qMin(Min,p);
       }
       m_customPlot->yAxis->setRange(Min*0.98,Max*1.02);

       m_customPlot->xAxis->setLabel("data");
       m_customPlot->yAxis->setLabel("Ilość");

       m_customPlot->replot();
}


void MainWindow::onMouseMove(QMouseEvent *event) {
    if(m_customPlot->graphCount()>0)
    for (int k=0; k<m_customPlot->graph(0)->dataCount() ;k++ )
    {
        if( std::abs(m_customPlot->graph(0)->dataPixelPosition(k).x()-event->pos().x())/m_customPlot->graph(0)->dataCount() <1 &&
            std::abs(m_customPlot->graph(0)->dataPixelPosition(k).y()-event->pos().y())/m_customPlot->graph(0)->dataCount() <1)
        {

            QString tooltipText = QString("%1").arg(m_customPlot->yAxis->pixelToCoord(m_customPlot->graph(0)->dataPixelPosition(k).y()));
            m_customPlot->setToolTip(tooltipText);

            m_customPlot->addGraph();
            QVector<double> x,y;
            y.push_back(m_customPlot->graph(0)->dataMainValue(k));
            x.push_back(m_customPlot->graph(0)->dataMainKey(k));
            m_customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));

            m_customPlot->graph(1)->setData(x,y);
            m_customPlot->replot();
            break;
        }

    }


}


void MainWindow::on_m_searchBox_selectionChanged()
{
    if( ui->m_searchBox->toPlainText()=="Szukaj...")
        ui->m_searchBox->setText("");

}


void MainWindow::on_m_searchBox_textChanged()
{
        QList<QListWidgetItem *> allItems = ui->ListsOfObjects->findItems("",Qt::MatchContains);

        QList<QListWidgetItem *> searched = ui->ListsOfObjects->findItems(ui->m_searchBox->toPlainText(),Qt::MatchContains);
        if(ui->ListsOfObjects->count()>0 && !searched.empty() && ui->m_searchBox->toPlainText()!=""){
            for(int i=0; i<allItems.size(); i++)
                allItems[i]->setHidden(true);
            for(int i=0; i<searched.size(); i++)
                searched[i]->setHidden(false);
        }
        else
            for(int i=0; i<allItems.size(); i++)
                allItems[i]->setHidden(false);

}


void MainWindow::on_m_showQuantityButton_pressed()
{
    CInfoWindow* infoWindow=new CInfoWindow();

    connect(infoWindow, &CInfoWindow::okClicked, this, &MainWindow::okClicked);

    infoWindow->exec();

}

void MainWindow::okClicked()
{
    m_dataBase->DeleteItem(ui->ListsOfObjects->selectedItems()[0]->text().toStdString());
    ui->ListsOfObjects->findItems(ui->ListsOfObjects->selectedItems()[0]->text(),Qt::MatchExactly)[0]->setHidden(true);
    ui->ListsOfObjects->update();
}

void MainWindow::on_pushButton_pressed()
{
    CInsertWindow* insertWindow = new CInsertWindow();
    connect(insertWindow, &CInsertWindow::onOkClicked, this, &MainWindow::InsertButtonClicked);
    insertWindow->exec();
}

void MainWindow::InsertButtonClicked(std::string name, std::string id){
    m_dataBase->InsertData(name, id);
    ui->ListsOfObjects->addItem(QString::fromStdString(name));
}

void MainWindow::on_pushButton_2_pressed()
{

}

