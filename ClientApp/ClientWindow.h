#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <Client.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(Client* client, QWidget *parent = nullptr);
    ~ClientWindow();

signals:
    void messageReceived(const std::string& message);

private slots:
    void onSendMessage();
    void onMessageReceived(const std::string& message);

private:
    void connectSignals();

    Ui::ClientWindow *ui;
    Client* m_client;
};
#endif // CLIENTWINDOW_H
