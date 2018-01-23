#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <serversclient.h>

class serversClient;

class Server : public QTcpServer
{
    Q_OBJECT
public:
	explicit Server(QWidget *widget=0, QObject *parent = 0);
    bool doStartServer(QHostAddress addr, qint16 port);
    QStringList getUserOnline();
    void sendName(QString name, QTcpSocket *cl);
protected:
    void incomingConnection(int handle);
private:
    QWidget *_widget;
    QList<serversClient *> _clients;
signals:

public slots:
    void onRemoveUser(serversClient *client);
	void startClient(QString name, quint32 sec);
	void stopClient(QString name);
};

#endif // SERVER_H
