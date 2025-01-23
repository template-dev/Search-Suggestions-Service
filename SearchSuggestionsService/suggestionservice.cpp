#include "suggestionservice.h"

std::unique_ptr<SuggestionService> SuggestionService::m_instance;
std::once_flag SuggestionService::m_flag;

SuggestionService& SuggestionService::getInstance() {
    static SuggestionService instance;
    return instance;
}

SuggestionService::SuggestionService()
        : m_db(QSqlDatabase::addDatabase("QSQLITE")){
    m_db.setDatabaseName("autocomplete.db");

    if (!m_db.open()) {
        qDebug() << "Ошибка подключения к БД: " << m_db.lastError().text();
    } else {
        qDebug() << "Подключено к БД SQLite";
        createTables();
    }
}

SuggestionService::~SuggestionService() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void SuggestionService::createTables() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS suggestions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "text TEXT NOT NULL UNIQUE, "
               "type TEXT CHECK(type IN ('word', 'phrase')) NOT NULL, "
               "category TEXT NOT NULL, "
               "frequency INTEGER DEFAULT 1)");
}

void SuggestionService::addSuggestion(const QString& category, const QString& text, const QString& type) {
    QSqlQuery sql;
    sql.prepare("INSERT INTO suggestions (text, type, category, frequency) VALUES (:text, :type, :category, 1) "
                "ON CONFLICT(text) DO UPDATE SET frequency = frequency + 1;");
    sql.bindValue(":text", text);
    sql.bindValue(":type", type);
    sql.bindValue(":category", category);

    if (!sql.exec()) {
        qDebug() << "Ошибка при добавлении запроса: " << sql.lastError().text();
    }
}

QStringList SuggestionService::fetchSuggestions(const QString& category, const QString& prefix, int limit) {
    QSqlQuery sql;
    sql.prepare("SELECT text FROM suggestions WHERE category = :category AND text LIKE :prefix "
                "ORDER BY frequency DESC LIMIT :limit");
    sql.bindValue(":category", category);
    sql.bindValue(":prefix", prefix + "%");
    sql.bindValue(":limit", limit);

    QStringList results;
    if (sql.exec()) {
        while (sql.next()) {
            results << sql.value(0).toString();
        }
    } else {
        qDebug() << "Ошибка при поиске саджестов: " << sql.lastError().text();
    }

    return results;
}
