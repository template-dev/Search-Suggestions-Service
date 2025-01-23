#include "database.h"

std::unique_ptr<Database> Database::m_instance;
std::once_flag Database::m_flag;

Database::Database()
        : m_db(QSqlDatabase::addDatabase("QSQLITE")) {
    m_db.setDatabaseName("autocomplete.db");

    if (!m_db.open()) {
        qDebug() << "Ошибка подключения к SQLite: " << m_db.lastError().text();
    } else {
        qDebug() << "Подключено к базе данных SQLite";
        createTables();
    }
}

void Database::createTables() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS suggestions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "text TEXT NOT NULL UNIQUE, "
               "type TEXT CHECK(type IN ('word', 'phrase')) NOT NULL, "
               "category TEXT NOT NULL, "
               "frequency INTEGER DEFAULT 1)");
}

Database& Database::getInstance() {
    std::call_once(m_flag, []() {
        m_instance = std::unique_ptr<Database>(new Database());
    });
    return *m_instance;
}
