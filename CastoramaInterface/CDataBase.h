#ifndef CDATABASE_H
#define CDATABASE_H



#include <iostream>
#include <vector>
#include "sqlite3.h"
#include <QDateTime>
#include <QtSql>

class CDataBase{
    std::string m_sDataBaseName;
    sqlite3 *m_Database;
public:
    CDataBase(std::string DBname);

    ~CDataBase() {
        m_db.close();
    }

    void InsertData(const char *sql_insert);

    void DeleteItem(const char *sql_insert);

    std::vector<std::string> GetNames();
    std::vector <std::pair<int, QDateTime>> GetData(std::string chosenName);
    QSqlDatabase m_db;
private:
};

#endif // CDATABASE_H
