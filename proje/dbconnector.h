#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <QSqlQuery>
#include <QSqlDatabase>


class dbconnector
{
public:
    dbconnector();
    ~dbconnector();

    bool ConnecT(const QString dbname);
    bool DisconnecT();
    QSqlQuery Query(QString q);
    QSqlDatabase db;
    bool dbstate;
};

namespace QDBMLite {
    class DB : public dbconnector{};
}

#endif // DBCONNECTOR_H
