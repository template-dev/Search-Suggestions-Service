#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QtConcurrent>
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
};
#endif // MAINWINDOW_H
