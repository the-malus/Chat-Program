#ifndef CLIENTLOGIN_H
#define CLIENTLOGIN_H

#include <Client.h>

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientLogin; }
QT_END_NAMESPACE

class ClientLogin : public QDialog
{
    Q_OBJECT

public:
    ClientLogin(Client* client, QWidget *parent = nullptr);
    ~ClientLogin();

private slots:
    void onConnect();

private:
    Ui::ClientLogin *ui;
    Client* m_client;
};
#endif // CLIENTLOGIN_H
