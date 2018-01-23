#include "screenservermain.h"
#include "ui_screenservermain.h"

ScreenServerMain::ScreenServerMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScreenServerMain)
{
	ui->setupUi(this);
	serv=new Server(this, this);
	if(serv->doStartServer(QHostAddress::LocalHost,111))
	{
		ui->lwServerLog->addItem(QTime::currentTime().toString()+": Server started at localhost:"+QString::number(serv->serverPort()));
		ui->lwServerLog->item(0)->setTextColor("#00FF00");
		connect(this, SIGNAL(startClient(QString,quint32)), serv, SLOT(startClient(QString,quint32)));
		connect(this, SIGNAL(stopClient(QString)), serv, SLOT(stopClient(QString)));
	}
	else
	{
		ui->lwServerLog->addItem(QTime::currentTime().toString()+": Server could not start at localhost:"+QString::number(serv->serverPort()));
		ui->lwServerLog->item(0)->setTextColor("#FF0000");
	}
	ui->pbStart->setEnabled(false);
	ui->pbStop->setEnabled(false);
	ui->sbInterval->setEnabled(false);
}

void ScreenServerMain::addUserToGUI(QString name, QColor color)
{
	ui->lwUserList->addItem(name);
	ui->lwUserList->item(ui->lwUserList->count()-1)->setTextColor(color);
}

void ScreenServerMain::addMessageToGUI(QString mess, QColor color)
{
	ui->lwServerLog->addItem(QTime::currentTime().toString()+" "+mess);
	quint16 high=ui->lwServerLog->count()-1;
	ui->lwServerLog->item(high)->setTextColor(color);
}

void ScreenServerMain::on_pbStart_clicked()
{
	foreach (QListWidgetItem *i, ui->lwUserList->selectedItems())
		i->setTextColor(Qt::green);
	this->changeEnableGUI(true);
	QString user=ui->lwUserList->selectedItems()[0]->text();
	quint32 sec=ui->sbInterval->value();
	emit startClient(user, sec);
	//ui->lwUserList->item(ui->lwUserList->selectedItems())->setTextColor(Qt::green);
}

void ScreenServerMain::on_pbStop_clicked()
{
	foreach(QListWidgetItem *i, ui->lwUserList->selectedItems())
		i->setTextColor(Qt::red);
	this->changeEnableGUI(false);
	QString user=ui->lwUserList->selectedItems()[0]->text();
	emit stopClient(user);
}

void ScreenServerMain::on_lwUserList_itemSelectionChanged()
{
	QListWidgetItem *item=ui->lwUserList->selectedItems()[0];
	if(item->textColor()==Qt::red)
	{
		this->changeEnableGUI(false);
	}
	else
	{
		this->changeEnableGUI(true);
	}
}

void ScreenServerMain::changeEnableGUI(bool isActive)
{
	if(isActive)
	{
		ui->pbStart->setEnabled(false);
		ui->sbInterval->setEnabled(false);
		ui->pbStop->setEnabled(true);
	}
	else
	{
		ui->pbStart->setEnabled(true);
		ui->sbInterval->setEnabled(true);
		ui->pbStop->setEnabled(false);
	}
}

ScreenServerMain::~ScreenServerMain()
{
    delete ui;
}
