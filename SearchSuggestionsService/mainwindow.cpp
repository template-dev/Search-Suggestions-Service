#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_completer(new QCompleter(this))
    , m_model(new QStringListModel(this))
{
    ui->setupUi(this);

    m_completer->setModel(m_model);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_2->setCompleter(m_completer);

    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &MainWindow::updateSuggestions);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSuggestions(const QString &text) {
    if (text.isEmpty()) {
        m_model->setStringList({});
        return;
    }

    QtConcurrent::run([this, text]() {
        QStringList suggestions = SuggestionService::getInstance().fetchSuggestions("Search", text);
        QMetaObject::invokeMethod(this, [this, suggestions]() {
                m_model->setStringList(suggestions);
            }, Qt::QueuedConnection);
    });
}
