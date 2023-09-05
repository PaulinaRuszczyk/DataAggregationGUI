#include "CDataBase.h"

#include <stdexcept>
#include <memory>
#include <ctime>
#include <QtDebug>
#include <QMessageBox>
#include <QFileInfo>

CDataBase::CDataBase(std::string DBname):
        m_sDataBaseName(std::move(DBname))
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("/home/pulinka/Desktop/cpp/Castorama/itemki.db");
    m_db.open();
    if(!m_db.isOpen())   {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Nie można załadować bazy danych! Sprawdź połączenie z internetem i sprobuj ponownie.");
        messageBox.setFixedSize(500,200);
    }
}

void CDataBase::InsertData(const char *sql_insert){
//

}
void CDataBase::DeleteItem(const char *sql_insert){
//

}
std::vector<std::string> CDataBase::GetNames(){

    std::vector <std::string>  result;
    QSqlQuery query = QSqlQuery (m_db);

    query.exec(QString::fromStdString("SELECT * FROM names;"));

    while (query.next())
        result.push_back(query.value(1).toString().toStdString().c_str());

    query.clear();
    return result;
}

std::vector <std::pair<int, QDateTime>> CDataBase::GetData(std::string chosenName){

    std::vector <std::pair<int, QDateTime>>  result;
    QSqlQuery query = QSqlQuery (m_db);

    query.exec(QString::fromStdString("SELECT * FROM quantity WHERE quantity.id IS (SELECT id FROM names WHERE names.name='"+chosenName+"');"));

    while (query.next())
    {
        if(query.value(1)!="")
            result.push_back(std::make_pair(query.value(1).toInt(), QDateTime::fromTime_t(std::stod( query.value(2).toString().toStdString()))));
        else
            result.push_back(std::make_pair(result.back().first, QDateTime::fromTime_t(std::stod( query.value(2).toString().toStdString()))));

    }

    query.clear();
    return result;
}
