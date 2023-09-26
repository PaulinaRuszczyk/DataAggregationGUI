#ifndef CDATABASE_H
#define CDATABASE_H
#define SQL_DATABASE_TYPE  0

#include <iostream>
#include <vector>
#include "sqlite3.h"
#include <QDateTime>
#include <QtSql>
#include <aws/dynamodb/DynamoDBClient.h>

#include <aws/core/Aws.h>
class CDataBase{
    std::string m_sDataBaseName;
    sqlite3 *m_Database;
public:
    CDataBase(std::string DBname);

    ~CDataBase() {
        m_db.close();
    }

    Aws::SDKOptions m_options;
    static std::pair<std::string, std::string> getCredentials();

public:
    Aws::DynamoDB::DynamoDBClient *m_dynamoClient;

    void InsertData(std::string name, std::string id);

    void DeleteItem(std::string itemName);

    std::vector<std::string> GetNames();

    std::string GetIdFromName(std::string chosenName);

    std::vector <std::pair<int, QDateTime>> GetData(std::string chosenName);
    QSqlDatabase m_db;
private:
};

#endif // CDATABASE_H
