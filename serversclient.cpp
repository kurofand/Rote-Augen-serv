#include "serversclient.h"

serversClient::serversClient(int desc, Server *serv, QObject *parent) :
    QObject(parent)
{
    _serv=serv;
    _isAutched=false;
	_name="";
    _blockSize=0;
    _sok=new QTcpSocket(this);
    _sok->setSocketDescriptor(desc);
    connect(_sok, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	connect(_sok, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	qDebug()<<"Client connected";
	_name=_sok->peerAddress().toString();
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out<<(quint16)0;
/*	out<<onSetClientName;
	out<<_name;*/
	out<<onSetClientActive;
	out<<" ";
	out.device()->seek(0);
	out<<(quint16)(block.size()-sizeof(quint16));
	_sok->write(block);
}

serversClient::~serversClient()
{

}

void serversClient::onConnect()
{

	_name=_sok->peerAddress().toString();
	_serv->sendName(_name, _sok);
}

void serversClient::onDisconnect()
{
    qDebug()<<"Client disconnected";
 /*   if(_isAutched)
    {
        emit removeUserFromGUI();
    }*/
    emit removeUser(this);
    deleteLater();
}

void serversClient::onReadyRead()
{
	QDataStream in(_sok);
	if(_blockSize==0)
	{
		if(_sok->bytesAvailable()<(int)sizeof(quint16))
			return;
		in>>_blockSize;
		qDebug()<<"_blockSize now "<<_blockSize;
	}
	if(_sok->bytesAvailable()<_blockSize)
		return;
	else
		_blockSize=0;
	quint8 command;
	in>>command;
	qDebug()<<"Received command "<<command;
/*	if(!_isAutched&&command!=comAutchReq)
		return;*/
	switch(command)
	{
	/*case comAutchReq:
	{
		QString name;
		in>>name;
		_name=name;
		_isAutched=true;
		emit addUserToGUI(name);
		break;
	}*/
	case onClientAnswer:
	{
		quint8 answer;
		in>>answer;
		qDebug()<<answer;
		quint8 answerType;
		in>>answerType;
		qDebug()<<answerType;
		switch(answerType)
		{
		case onRequestName:
		{
			if(answer)
			{
				qDebug()<<"send request name";
				QByteArray block;
				QDataStream out(&block, QIODevice::WriteOnly);
				QString name=_sok->peerAddress().toString();
				out<<(quint16)0;
				out<<onSetClientName;
				out<<name;
				out.device()->seek(0);
				out<<(quint16)(block.size()-sizeof(quint16));
				qDebug()<<block.size();
				_sok->write(block);
				emit(addUserToGUI(name, "#FF0000"));
				emit(addMessageToGUI(name+" has connected", "#00FF00"));
			}
			else
				emit(addMessageToGUI("Unexpected problems with client connect!", "#FF0000"));
		}
		break;
		case onStartClient:
		{
			if(answer)
				emit(addMessageToGUI("Screenshot request was successfully sended to "+ this->_name, "#00FF00"));
			else
			{
				emit(addMessageToGUI("Error with send screenshot request to "+ this->_name, "#FF0000"));
				emit(changeEnableGUI(false));
			}
			break;
		}
		case onClientDisable:
		{
			if(answer)
				emit(addMessageToGUI(this->_name+" was successfully stoped", "#00FF00"));
			else
				emit(addMessageToGUI(this->_name+" was not stopped!", "#FF0000"));
			break;
		}
		}
		break;
	}
	case onSendPicture:
	{
/*		QImage img;
		img.loadFromData(in);*/
		QDateTime datetime=QDateTime::currentDateTime();
//		in>>datetime;
		QImage img;
		img.load(in.device(),"PNG");
		qDebug()<<"loaded img";
		QString format="png";
		if(!QDir(this->_name).exists())
				QDir().mkdir(this->_name);
		emit(addMessageToGUI("Receive a screenshot from "+this->_name, "#00FF00"));
		QString filename=QDir::currentPath()+"/"+this->_name+"/"+datetime.toString()+"."+format;
		qDebug()<<filename;
		if(img.save(filename))
			emit(addMessageToGUI("Screenshot from "+this->_name+" was saved to folder","#00FF00"));
		else
			emit(addMessageToGUI("Error with saving a screenshot from "+this->_name+" to folder","#FF0000"));
	}
	}
}
