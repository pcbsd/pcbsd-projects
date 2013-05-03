#ifndef _APPCAFE_LARGE_WIDGET_H
#define _APPCAFE_LARGE_WIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QGridLayout>
#include <QAction>
#include <QString>
#include <QIcon>

class LargeItemWidget : public QWidget{
	Q_OBJECT
	
  private:
    QString uniqueAppID; //should be something unique like <name>-<version>-<arch>	
    
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    LargeItemWidget(QString appID, QString name, QString description, QString icon) : QWidget(){
      QGridLayout *layout = new QGridLayout;
      //Create the toolbutton
      QToolButton *button = new QToolButton();
        button->setIconSize(QSize(34,34));
          QAction *act = new QAction( QIcon(icon), "", 0);
        button->setDefaultAction(act);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);

      //Create the labels
      QLabel *appName = new QLabel("<b>"+name+"</b>");
      QLabel *appDesc = new QLabel(description);
        appDesc->setWordWrap(TRUE);
      //Add the items to the widget
      layout->addWidget(button,0,0,2,1);
      layout->addWidget(appName,0,1);
      layout->addWidget(appDesc,1,1);
      this->setLayout(layout);
      //Save the app identifier
      uniqueAppID=appID;
      connect(button,SIGNAL(triggered(QAction*)), this, SLOT(sendSignal()) );
    }
    virtual ~LargeItemWidget(){}
 
  	  
};

#endif
