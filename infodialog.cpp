#include "infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent) 
{
	setModal(true);
	resize(600, 500);
	setWindowTitle(QString::fromUtf8("Information"));
	gridLayout = new QGridLayout(this);
	textEdit = new QTextEdit(this);
	textEdit->setReadOnly(true);
	gridLayout->addWidget(textEdit, 0, 0, 1, 3);	
	pushButtonClose = new QPushButton("Close", this);
	connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(accept()));
	gridLayout->addWidget(pushButtonClose, 1, 1, 1, 1);
	spacerItemLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	gridLayout->addItem(spacerItemLeft, 1, 0, 1, 1);
	spacerItemRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	gridLayout->addItem(spacerItemRight, 1, 2, 1, 1);
}

void InfoDialog::slotLoadAbout()
{
	
	QFile file;
	QString path = QDir::currentPath();
	path.append("/documentation/about.txt");
	file.setFileName(path);
	
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray data = file.readAll();
	QString str = QString::fromUtf8(data);
	textEdit->setPlainText(str);
	show();
	return;
}

void InfoDialog::slotLoadTutorial()
{
	QFile file;
	QString path = QDir::currentPath();
	path.append("/documentation/tutorial.txt");
	file.setFileName(path);
	
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray data = file.readAll();
	QString str = QString::fromLocal8Bit(data);
	textEdit->setPlainText(str);
	show();
	return;
}




void InfoDialog::slotLoadLicense()
{
	QFile file;
	QString path = QDir::currentPath();
	path.append("/documentation/license.txt");
	file.setFileName(path);
	
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray data = file.readAll();
	QString str = QString::fromLocal8Bit(data);
	textEdit->setPlainText(str);
	show();
	return;
}
