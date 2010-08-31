#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTextDocument>
#include <QtGui/QWidget>
#include <QtGui/QDialog>

class InfoDialog : public QDialog 
{
	Q_OBJECT
public:
	InfoDialog(QWidget *parent=NULL);
	
public slots:
	void slotLoadAbout();
	void slotLoadTutorial();
	void slotLoadLicense();

private:
	QPushButton *pushButtonClose;
	QGridLayout *gridLayout;
	QSpacerItem *spacerItemRight, *spacerItemLeft;
	QTextEdit *textEdit;
};

#endif
