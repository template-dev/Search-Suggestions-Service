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
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    QListView *popupView = new QListView();
    popupView->setStyleSheet(
        "QListView {"
        "   font-size: 14px;"
        "}"
        "QListView::item {"
        "   padding: 5px;"
        "   color: black;"
        "   font-size: 12px;"
        "   background-color: white;"
        "}"
        "QListView::item:selected {"
        "   background-color: lightblue;"
        "   color: black;"
        "}"
        "QListView::item:hover {"
        "   background-color: lightgray;"
        "}"
        );
    m_completer->setPopup(popupView);
    ui->lineEdit_2->setCompleter(m_completer);

    ui->clearBtn->hide();

    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &MainWindow::updateSuggestions);
    connect(ui->clearBtn, &QPushButton::clicked, this, &MainWindow::onClearSuggestionForm);
    connect(ui->searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchGoogle);
    connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated), this, &MainWindow::onSuggestionSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSuggestions(const QString &text) {
    if (text.isEmpty()) {
        m_model->setStringList({});
        ui->clearBtn->hide();
        return;
    }

    ui->clearBtn->setVisible(!text.isEmpty());

    QtConcurrent::run([this, text]() {
        QStringList suggestions = SuggestionService::getInstance().fetchSuggestions("Search", text);

        QMetaObject::invokeMethod(this, [this, suggestions, text]() {
                m_model->setStringList(suggestions);
                m_completer->setCompletionPrefix(text);
                if (!suggestions.isEmpty()) {
                    m_completer->complete();
                }
            }, Qt::QueuedConnection);
    });
}

void MainWindow::onSuggestionSelected(const QString &suggestion) {
    qDebug() << "A suggestion has been selected:" << suggestion;
    SuggestionService::getInstance().addSuggestion("Search", suggestion, "phrase");
    updateSuggestions(suggestion);
}

void MainWindow::onClearSuggestionForm() {
    ui->lineEdit_2->clear();
}

void MainWindow::onUpdateButtonVisibility(const QString &text) {
    ui->clearBtn->setVisible(!text.isEmpty());
}

void MainWindow::onSearchGoogle() {
    QString query = ui->lineEdit_2->text().trimmed();
    if (query.isEmpty()) return;

    QString type = query.contains(" ") ? "phrase" : "word";

    SuggestionService::getInstance().addSuggestion("Search", query, type);

    if (!query.isEmpty()) {
        QString url = "https://www.google.com/search?q=" + QUrl::toPercentEncoding(query);
        QDesktopServices::openUrl(QUrl(url));
    }
}
