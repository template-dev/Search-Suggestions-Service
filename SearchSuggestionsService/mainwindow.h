#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QtConcurrent>
#include <QDesktopServices>
#include <QAbstractItemView>
#include <QUrl>
#include <QDebug>
#include <QListView>
#include "suggestionservice.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCompleter *m_completer;
    QStringListModel *m_model;

private slots:
    void updateSuggestions(const QString &text);
    void onSuggestionSelected(const QString &suggestion);
    void onClearSuggestionForm();
    void onUpdateButtonVisibility(const QString &text);
    void onSearchGoogle();
};
#endif // MAINWINDOW_H
