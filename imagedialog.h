#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtGui/QMovie>
#include <QtGui/QPixmap>

class ImageDialog : public QDialog 
{
	Q_OBJECT
public:
	ImageDialog(QWidget *parent=NULL);
	
public slots:
	void slotYouLost();
	void slotYouWon();

private:
	QPushButton *pushButtonClose;
	QGridLayout *gridLayout;
	QSpacerItem *spacerItemRight, *spacerItemLeft;
	QLabel *imageLabel;
};

#endif
