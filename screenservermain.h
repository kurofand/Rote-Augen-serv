#ifndef SCREENSERVERMAIN_H
#define SCREENSERVERMAIN_H

#include <QMainWindow>
#include <QTime>
#include "server.h"
class Server;
namespace Ui {
class ScreenServerMain;
}

class ScreenServerMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScreenServerMain(QWidget *parent = 0);
    ~ScreenServerMain();

private:
    Ui::ScreenServerMain *ui;
	Server *serv;
	//void changeEnable(bool isActive);

public slots:
	void addUserToGUI(QString name, QColor color);
	void addMessageToGUI(QString mess, QColor color);
	void changeEnableGUI(bool isActive);

private slots:
	void on_pbStart_clicked();
	void on_pbStop_clicked();
	void on_lwUserList_itemSelectionChanged();

signals:
	void startClient(QString user, quint32 sec);
	void stopClient(QString user);
};

#endif // SCREENSERVERMAIN_H
