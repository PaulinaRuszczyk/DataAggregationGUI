#include "cinsertwindow.h"
#include "ui_cinsertwindow.h"
#include <QMessageBox>
#include <stdexcept>

CInsertWindow::CInsertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInsertWindow)
{
    ui->setupUi(this);
}

CInsertWindow::~CInsertWindow()
{
    delete ui;
}

void CInsertWindow::on_buttonBox_accepted()
{
    try{
    std::string id=ui->m_insertLink->text().toStdString();
    std::reverse(id.begin(), id.end());
    if(id.find("_") == std::string::npos || id.find("/") == std::string::npos || id.find("/") <id.find("_") )
        throw std::out_of_range("");
    id=id.substr(id.find("_")+1);
    id=id.substr(0,id.find("/"));
    std::reverse(id.begin(), id.end());

    if(!id.empty())
        emit onOkClicked( ui->m_insertName->text().toStdString(), id);
    else
        throw std::out_of_range("");
    }
    catch(std::exception e)
    {
        QMessageBox msgBox;
        msgBox.setText("Błędny link");
        msgBox.exec();
    }
}

