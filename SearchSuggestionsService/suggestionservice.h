#ifndef SUGGESTIONSERVICE_H
#define SUGGESTIONSERVICE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QThread>
#include <QDebug>
#include <memory>
#include <mutex>

class SuggestionService
{
public:
    static SuggestionService& getInstance();
    ~SuggestionService();

    void addSuggestion(const QString& category, const QString& text, const QString& type);
    QStringList fetchSuggestions(const QString& category, const QString& prefix, int limit = 15);

private:
    SuggestionService();
    void createTables();

    QSqlDatabase m_db;
    static std::unique_ptr<SuggestionService> m_instance;
    static std::once_flag m_flag;
};

#endif // SUGGESTIONSERVICE_H
