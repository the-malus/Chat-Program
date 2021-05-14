#ifndef CLIENTLOGIN_H
#define CLIENTLOGIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientLogin; }
QT_END_NAMESPACE

class ClientLogin : public QMainWindow
{
    Q_OBJECT

public:
    ClientLogin(QWidget *parent = nullptr);
    ~ClientLogin();
//signals:
//    void windowClosed();
private:
    //void closeEvent(QCloseEvent* event) override;

    Ui::ClientLogin *ui;
};
#endif // CLIENTLOGIN_H
