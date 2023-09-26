#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CDataBase.h>
#include <QListWidgetItem>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ListsOfObjects_itemActivated(QListWidgetItem *item);
    void onMouseMove(QMouseEvent *event);

    void on_m_searchBox_selectionChanged();
    void on_m_searchBox_textChanged();

    void on_m_showQuantityButton_pressed();

    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

private:
    void okClicked();
    void InsertButtonClicked(std::string name, std::string id);
    bool begginigOfSearch;
    CDataBase* m_dataBase;
    QCustomPlot *m_customPlot;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
