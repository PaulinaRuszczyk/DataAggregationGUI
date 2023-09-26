#include "cinfowindow.h"
#include "ui_cinfowindow.h"

CInfoWindow::CInfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInfoWindow)
{
    ui->setupUi(this);
}

CInfoWindow::~CInfoWindow()
{
    delete ui;
}

void CInfoWindow::on_buttonBox_accepted()
{
    emit okClicked();
    this->close();
}


void CInfoWindow::on_buttonBox_rejected()
{
    this->close();
}

