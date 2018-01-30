#include "server.h"

Server::Server(QWidget *widget, QObject *parent) :
    QTcpServer(parent)
{
	_widget=widget;
}

bool Server::doStartServer(QHostAddress addr, qint16 port)
{
    if(!this->listen(addr, port))
    {
        qDebug()<<"Server not started";
        qDebug()<<this->errorString();
        return false;
    }
    qDebug()<<"Server started at"<<addr<<":"<<port;
    return true;
}

void Server::incomingConnection(int handle)
{
	serversClient *servClient=new serversClient(handle, this, this);
    if(_widget!=0)
    {
		connect(servClient, SIGNAL(addUserToGUI(QString, QColor)), _widget, SLOT(addUserToGUI(QString, QColor)));
		connect(servClient, SIGNAL(removeUserFromGUI(QString)), _widget, SLOT(onRemoveUserFromGUI(QString)));
		connect(servClient, SIGNAL(addMessageToGUI(QString,QColor)), _widget, SLOT(addMessageToGUI(QString, QColor)));
		connect(servClient, SIGNAL(changeEnableGUI(bool)), _widget, SLOT(changeEnableGUI(bool)));
    }
    connect(servClient, SIGNAL(removeUser(serversClient*)), this, SLOT(onRemoveUser(serversClient*)));
	_clients.append(servClient);
}

void Server::onRemoveUser(serversClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

void Server::startClient(QString name, quint32 sec)
{
	for(quint16 i=0;i<_clients.length();i++)
		if(_clients[i]->_name==name)
		{
			QByteArray block;
			QDataStream out(&block, QIODevice::WriteOnly);
			out<<(quint32)0<<_clients[i]->onStartClient<<sec;
			out.device()->seek(0);
			out<<(quint32)(block.size()-sizeof(quint32));
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
			out<<(quint32)0<<_clients[i]->onClientDisable<<" ";
			out.device()->seek(0);
			out<<(quint32)(block.size()-sizeof(quint32));
			_clients[i]->_sok->write(block);
		}
}

void Server::changeClientName(QString oldName, QString newName)
{
    for(quint16 i=0;i<_clients.length();i++)
        if(_clients[i]->_name==oldName)
        {
			_clients[i]->_name=newName;
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out<<(quint32)0<<_clients[i]->onSetClientName<<newName;
            out.device()->seek(0);
            out<<(quint32)(block.size()-sizeof(quint32));
            _clients[i]->_sok->write(block);
        }
}
