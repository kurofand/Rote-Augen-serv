#include "server.h"

Server::Server(QWidget *widget, QObject *parent) :
    QTcpServer(parent)
{
	_widget=widget;
}

bool Server::doStartServer(QHostAddress addr, qint16 port)
{
    if(!listen(addr, port))
    {
        qDebug()<<"Server not started";
        return false;
    }
    qDebug()<<"Server started at"<<addr<<":"<<port;
    return true;
}

void Server::incomingConnection(int handle)
{
	serversClient *servClient=new serversClient(handle, this, this);
//	qDebug()<<_widget;
    if(_widget!=0)
    {
		connect(servClient, SIGNAL(addUserToGUI(QString, QColor)), _widget, SLOT(addUserToGUI(QString, QColor)));
//        connect(servClient, SIGNAL(removeUSERFROMGUI(QString)), _widget, SLOT(onRemoveUserFromGUI(QString)));
//        connect(servClient, SIGNAL(messageToGUI(QString, QString, QStringList)), _widget, SLOT(onMessageToGUI(QString, QString, QStringList)));
		connect(servClient, SIGNAL(addMessageToGUI(QString,QColor)), _widget, SLOT(addMessageToGUI(QString, QColor)));
		connect(servClient, SIGNAL(changeEnableGUI(bool)), _widget, SLOT(changeEnableGUI(bool)));
    }
    connect(servClient, SIGNAL(removeUser(serversClient*)), this, SLOT(onRemoveUser(serversClient*)));
//	this->sendName('nah', servClient);
/*	QString n="nah";
	QByteArray block;
	QDataStream out(&block, QIODevice::ReadWrite);
	out<<(quint16)0;
	out<<1;
	out<<n;
	out.device()->seek(0);
	out<<(quint16)(block.size()-sizeof(quint16));
	servClient->_sok->write(block);*/
	_clients.append(servClient);
}

void Server::onRemoveUser(serversClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

void Server::sendName(QString name, QTcpSocket *cl)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
	out<<(quint16)0<<name;
    out.device()->seek(0);
	out<<(quint16)(block.size()-sizeof(quint16));
    cl->write(block);
/*    for(int i=0;i<_clients.length();++i)
        if(_clients.at(i)->getName==name)
            _clients.at(i)->_sok->write(block);*/

}

void Server::startClient(QString name, quint32 sec)
{
	for(quint16 i=0;i<_clients.length();i++)
		if(_clients[i]->_name==name)
		{
			QByteArray block;
			QDataStream out(&block, QIODevice::WriteOnly);
			out<<(quint16)0<<_clients[i]->onStartClient<<sec;
			out.device()->seek(0);
			out<<(quint16)(block.size()-sizeof(quint16));
			_clients[i]->_sok->write(block);
		}
}

void Server::stopClient(QString name)
{
	for(quint16 i=0;i<_clients.length();i++)
		if(_clients[i]->_name==name)
		{
			QByteArray block;
			QDataStream out(&block, QIODevice::WriteOnly);
			out<<(quint16)0<<_clients[i]->onClientDisable<<" ";
			out.device()->seek(0);
			out<<(quint16)(block.size()-sizeof(quint16));
			_clients[i]->_sok->write(block);
		}
}
