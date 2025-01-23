#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <memory>
#include <mutex>

class Database
{
public:
    static Database& getInstance();
    QSqlDatabase& getDb() { return m_db; }
private:
    static std::unique_ptr<Database> m_instance;
    static std::once_flag m_flag;
    QSqlDatabase m_db;

    Database();
    void createTables();
};

#endif // DATABASE_H
