#include "CDataBase.h"

#include <stdexcept>
#include <memory>
#include <ctime>
#include <QtDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <aws/core/utils/Outcome.h>
#include <fstream>
#include <aws/dynamodb/model/ScanRequest.h>
#include "CDataBase.h"

#include <stdexcept>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>

CDataBase::CDataBase(std::string DBname):
        m_sDataBaseName(std::move(DBname))
{

    if(SQL_DATABASE_TYPE){
        m_db=QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("/home/pulinka/Desktop/cpp/Castorama/itemki.db");
        m_db.open();
        if(!m_db.isOpen())   {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Nie można załadować bazy danych! Sprawdź połączenie z internetem i sprobuj ponownie.");
            messageBox.setFixedSize(500,200);
        }
    }
    else {

        std::pair <std::string, std::string> keys =getCredentials();
        Aws::Auth::AWSCredentials credentials(keys.first, keys.second);
        Aws::InitAPI( m_options);
        Aws::Client::ClientConfiguration clientConfiguration;
        clientConfiguration.region = "eu-north-1";// Change to your desired region
        m_dynamoClient=new Aws::DynamoDB::DynamoDBClient(credentials, clientConfiguration);
    }
}

std::pair<std::string, std::string> CDataBase::getCredentials(){

    std::ifstream credentialsFile("/home/pulinka/Desktop/cpp/Castorama/cmake-build-debug/Credentials.env");
    std::string keys;
    if(!credentialsFile.is_open())
        throw  std::runtime_error("Cannot find credentials file. ");
    getline (credentialsFile, keys);
    std::string accessKey= keys.substr(keys.find("accessKeyId:")+12, keys.find('\n'));

    getline (credentialsFile, keys);
    std::string secretKey= keys.substr(keys.find("secretKey:")+10, keys.find('\n'));
    return std::make_pair(accessKey, secretKey);
};

void CDataBase::InsertData(std::string name, std::string id){
    if(SQL_DATABASE_TYPE){
        QSqlQuery query = QSqlQuery (m_db);
        std::string sqlReq="INSERT INTO names(id, name) VALUES (\""+name+"\",\""+id+"\";";
        query.exec(QString::fromStdString(sqlReq));
        query.clear();
    }
    else {
        Aws::DynamoDB::Model::PutItemRequest InsertRequest;
        InsertRequest.SetTableName("names");
        InsertRequest.AddItem("id", Aws::DynamoDB::Model::AttributeValue().SetS(id));
        InsertRequest.AddItem("name", Aws::DynamoDB::Model::AttributeValue().SetS(name));

        const Aws::DynamoDB::Model::ScanOutcome& scanOutcome = m_dynamoClient->PutItem(InsertRequest);
        std::vector <std::string> temp;
        if (!scanOutcome.IsSuccess()) {
            throw std::runtime_error("Unable to connect to Data Base. Try again later!"+scanOutcome.GetError().GetMessage());
        }
    }
}

void CDataBase::DeleteItem(std::string itemName){
    if(SQL_DATABASE_TYPE){
        QSqlQuery query = QSqlQuery (m_db);
        std::string sqlReq="DELETE FROM names WHERE name=\""+itemName+"\";";
        query.exec(QString::fromStdString(sqlReq));
        query.clear();
    }
    else {
        //Find data to delete from quantity table
        Aws::DynamoDB::Model::ScanRequest scanRequest;
        scanRequest.SetTableName("quantity");

        scanRequest.SetFilterExpression("#name = :val");
        scanRequest.AddExpressionAttributeNames("#name", "id");
        Aws::DynamoDB::Model::AttributeValue value;
        value.SetS(" "+GetIdFromName( itemName));
        scanRequest.AddExpressionAttributeValues(":val", value);

        const Aws::DynamoDB::Model::ScanOutcome& scanOutcome = m_dynamoClient->Scan(scanRequest);
        std::vector <std::string> temp;
        if (scanOutcome.IsSuccess()) {
            const auto& items = scanOutcome.GetResult().GetItems();

            for (const auto& item : items) {
                auto nameValue = item.find("lp");
                if (nameValue != item.end())
                     temp.push_back( nameValue->second.GetS());
            }
        } else {
            const Aws::Client::AWSError<Aws::DynamoDB::DynamoDBErrors>& error = scanOutcome.GetError();

            if (error.GetErrorType() == Aws::DynamoDB::DynamoDBErrors::PROVISIONED_THROUGHPUT_EXCEEDED)
            {
                throw std::runtime_error("Unable to connect to Data Base. Try again later!"+scanOutcome.GetError().GetMessage());
            }
        }
        //Delete from quantity table
        for(int i=0;i< temp.size(); i++){
            Aws::DynamoDB::Model::DeleteItemRequest deleteFromQuantityRequest ;
            deleteFromQuantityRequest.AddKey("lp", Aws::DynamoDB::Model::AttributeValue().SetS(temp[i]));
            deleteFromQuantityRequest.SetTableName("quantity");
            const Aws::DynamoDB::Model::DeleteItemOutcome &outcome1 = m_dynamoClient->DeleteItem(deleteFromQuantityRequest);
            if (!outcome1.IsSuccess())
                throw std::runtime_error("Unable to connect to Data Base. Try again later!"+outcome1.GetError().GetMessage());
        }
        //Delete from names table
        Aws::DynamoDB::Model::DeleteItemRequest deleteRequest ;
        deleteRequest.AddKey("id", Aws::DynamoDB::Model::AttributeValue().SetS(GetIdFromName(itemName)));
        deleteRequest.SetTableName("names");
        const Aws::DynamoDB::Model::DeleteItemOutcome &outcome = m_dynamoClient->DeleteItem(deleteRequest);
        if (!outcome.IsSuccess())
           throw std::runtime_error("Unable to connect to Data Base. Try again later!"+outcome.GetError().GetMessage());
    }


}

std::vector<std::string> CDataBase::GetNames(){

    if(SQL_DATABASE_TYPE){
        std::vector <std::string>  result;
        QSqlQuery query = QSqlQuery (m_db);

        query.exec(QString::fromStdString("SELECT * FROM names;"));

        while (query.next())
            result.push_back(query.value(1).toString().toStdString().c_str());

        query.clear();
        return result;
    }
    else {
        std::vector <std::string> vNames;
        Aws::DynamoDB::Model::ScanRequest scanRequest;
        scanRequest.SetTableName("names");
        const Aws::DynamoDB::Model::ScanOutcome& outcome = m_dynamoClient->Scan(scanRequest);

        if (outcome.IsSuccess()) {
            const auto& items = outcome.GetResult().GetItems();

            for (const auto& item : items) {
                auto nameValue = item.find("name");
                if (nameValue != item.end())
                    vNames.push_back(nameValue->second.GetS());
            }
        } else {
            throw std::runtime_error("Unable to connect to Data Base. Try again later!"+outcome.GetError().GetMessage());
        }
        return vNames;
    }
}

std::string CDataBase::GetIdFromName(std::string chosenName){

    if(!SQL_DATABASE_TYPE){
        Aws::DynamoDB::Model::ScanRequest scanRequest;
        scanRequest.SetTableName("names");

        scanRequest.SetFilterExpression("#name = :val");
        scanRequest.AddExpressionAttributeNames("#name", "name");
        Aws::DynamoDB::Model::AttributeValue value;
        value.SetS(chosenName);
        scanRequest.AddExpressionAttributeValues(":val", value);

        const Aws::DynamoDB::Model::ScanOutcome& outcome = m_dynamoClient->Scan(scanRequest);

        if (outcome.IsSuccess()) {
            const auto& items = outcome.GetResult().GetItems();

            for (const auto& item : items) {
                auto nameValue = item.find("id");
                if (nameValue != item.end())
                    return nameValue->second.GetS();
            }
        } else {
            throw std::runtime_error("Unable to connect to Data Base. Try again later!"+outcome.GetError().GetMessage());
        }
        return 0;
    }

}

std::vector <std::pair<int, QDateTime>> CDataBase::GetData(std::string chosenName){

    if(SQL_DATABASE_TYPE){
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
    else{
        std::vector <std::pair<int, QDateTime>>  result;
        std::string tempQuantity, tempDate;
        Aws::DynamoDB::Model::ScanRequest scanRequest;
        scanRequest.SetTableName("quantities");
        scanRequest.SetFilterExpression("#id = :val");
        scanRequest.AddExpressionAttributeNames("#id", "id");
        Aws::DynamoDB::Model::AttributeValue value;
        value.SetS(GetIdFromName(chosenName)); // Change the value as needed
        scanRequest.AddExpressionAttributeValues(":val", value);
        const Aws::DynamoDB::Model::ScanOutcome& outcome = m_dynamoClient->Scan(scanRequest);

        if (outcome.IsSuccess()) {
            const auto& items = outcome.GetResult().GetItems();

            for (const auto& item : items) {
                auto quantityValue = item.find("quantity");
                if (quantityValue != item.end()){
                    if(quantityValue->second.GetS()!="NULL")
                        tempQuantity = quantityValue->second.GetS();
                    else
                        continue;
                }
                auto dateValue = item.find("date");
                if (dateValue  != item.end())
                    tempDate=dateValue ->second.GetS();
                result.push_back(std::make_pair(std::stoi(tempQuantity), QDateTime::fromTime_t(std::stod(tempDate))));

            }
        } else {
            throw std::runtime_error("Unable to connect to Data Base. Try again later!"+outcome.GetError().GetMessage());
        }
        return result;
    }
}
