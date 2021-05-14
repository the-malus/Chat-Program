#include "ClientLogin.h"
#include "./ui_ClientLogin.h"

ClientLogin::ClientLogin(Client* client, QWidget *parent)
    : QDialog(parent)
    , m_client(client)
    , ui(new Ui::ClientLogin)
{
    ui->setupUi(this);

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(onConnect()));
    connect(ui->nameInput, SIGNAL(returnPressed()), this, SLOT(onConnect()));
    connect(ui->serverInput, SIGNAL(returnPressed()), this, SLOT(onConnect()));
}

ClientLogin::~ClientLogin()
{
    delete ui;
}

void ClientLogin::onConnect()
{
    std::string username(ui->nameInput->text().toUtf8());
    std::string server(ui->serverInput->text().toUtf8());

    m_client->setName(username);
    m_client->connectToServer(server);

    this->close();
}

