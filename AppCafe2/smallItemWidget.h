#ifndef _APPCAFE_SMALL_WIDGET_H
#define _APPCAFE_SMALL_WIDGET_H

#include <QToolButton>
#include <QAction>
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSize>

class SmallItemWidget : public QWidget{
	Q_OBJECT
	
  private:
    QString uniqueAppID;	
    
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    SmallItemWidget(QString appID, QString name, QString icon, QString version) : QWidget(){
      //Create the items
      QToolButton* button = new QToolButton();
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        //button->setIconSize(QSize(32,32));
      QAction* act = new QAction(QIcon(icon),"",this);
        if(version.isEmpty()){ act->setText(name); }
        else{ act->setText(name+"\n"+version); }
        button->setDefaultAction(act);
      //QLabel* label = new QLabel();
        //label->setWordWrap(TRUE);
        //label->setText(version);
      //Now add them to the widget
      QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(button);
        //layout->addWidget(label);
      this->setLayout(layout);
      //Save the app identifier and connect the signal/slot
      uniqueAppID=appID;
      connect(button,SIGNAL(triggered(QAction*)), this, SLOT(sendSignal()) );
    }
    virtual ~SmallItemWidget(){}
 
  	  
};

#endif
