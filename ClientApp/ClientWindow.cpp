#include "ClientWindow.h"
#include "ClientLogin.h"
#include "./ui_clientwindow.h"

ClientWindow::ClientWindow(Client* client, QWidget *parent)
    : QMainWindow(parent)
    , m_client(client)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    connectSignals();

    ClientLogin login(m_client);
    login.exec();

    if (!m_client->isConnected())
    {
        ui->textEdit->append("Failed to connect to server.");
    }
}

ClientWindow::~ClientWindow()
{
    delete ui;
    delete m_client;
}

void ClientWindow::connectSignals()
{
    m_client->listenToMessageReceived([this](const std::string& message) { emit messageReceived(message); });

    connect(this, SIGNAL(messageReceived(const std::string&)), this, SLOT(onMessageReceived(const std::string&)));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onSendMessage()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onSendMessage()));
}

void ClientWindow::onSendMessage()
{
    std::string message(ui->lineEdit->text().toUtf8());
    m_client->sendMessage(message);
    ui->textEdit->append(QString::fromUtf8("You: " + message));
    ui->lineEdit->clear();
}

void ClientWindow::onMessageReceived(const std::string& message)
{
    ui->textEdit->append(QString::fromUtf8(message.c_str()));
}

