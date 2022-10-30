#include "dbconnector.h"
dbconnector::dbconnector()
{
}
dbconnector::~dbconnector()
{
    DisconnecT();
}

bool dbconnector::ConnecT(const QString dbname)
{
    if (!this->db.isOpen())
    {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(dbname);

        if (!this->db.open())
        {
            return  false;
        }
        else
        {
            return  true;
        }
    }
    else
    {
        return false;
    }
}

bool dbconnector::DisconnecT()
{
    if (this->db.isOpen())
    {
        this->db.close();
        return  true;
    }
    else
    {
        return  false;
    }
}

QSqlQuery dbconnector::Query(QString q)
{
    QSqlQuery query(q);
    return query;
}
