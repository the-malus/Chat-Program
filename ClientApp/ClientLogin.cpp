#include "ClientLogin.h"
#include "./ui_ClientLogin.h"

ClientLogin::ClientLogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientLogin)
{
    ui->setupUi(this);
}

ClientLogin::~ClientLogin()
{
    delete ui;
}

//void ClientLogin::closeEvent(QCloseEvent* event)
//{
//    emit windowClosed();
//    event->accept();
//}

