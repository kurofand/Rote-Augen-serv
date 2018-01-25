#ifndef SERVERSCLIENT_H
#define SERVERSCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <screenservermain.h>
#include <server.h>
#include <QDir>

class ScreenServerMain;
class Server;
class serversClient : public QObject
{
    friend class Server;
    Q_OBJECT
public:
	static const quint8 comAutchReq=1;
	bool isActive=false;
	QString _name;
    explicit serversClient(int desc, Server *serv, QObject *parent = 0);
    ~serversClient();

private:
    QTcpSocket *_sok;
    Server *_serv;
	quint32 _blockSize;
    bool _isAutched;
	static const quint8 onSetClientActive=1;
	static const quint8 onSetClientName=2;
	static const quint8 onClientDisable=3;
	static const quint8 onClientAnswer=4;
	static const quint8 onRequestName=5;
	static const quint8 onStartClient=6;
	static const quint8 onSendPicture=7;

signals:
	void addUserToGUI(QString name, QColor color);
	void addMessageToGUI(QString mess, QColor color);
	void changeEnableGUI(bool);
    void removeUserFromGUI(QString name);
    void removeUser(serversClient *client);
public slots:

private slots:
    void onConnect();
    void onDisconnect();
	void onReadyRead();

};

#endif // SERVERSCLIENT_H
