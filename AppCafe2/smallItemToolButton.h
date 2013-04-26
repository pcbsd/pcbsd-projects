#ifndef _APPCAFE_SMALL_WIDGET_H
#define _APPCAFE_SMALL_WIDGET_H

#include <QToolButton>
#include <QAction>
#include <QString>
#include <QIcon>

class SmallItemToolButton : public QToolButton{
	Q_OBJECT
	
  private:
    QString uniqueAppID; //should be something unique like <name>-<version>-<arch>	
    
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    SmallItemToolButton(QString name, QString icon, QString version, QString textlocation, QString appID){
      //Create the action
      QAction* act = new QAction( QIcon(icon), name, 0);
      act->setToolTip(version);
      //Set the ToolButton style
      if(textlocation=="under"){ this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); }
      else if(textlocation=="beside"){ this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); }
      else{ this->setToolButtonStyle(Qt::ToolButtonIconOnly); }
      //Add the action
      this->setDefaultAction(act);
      //Save the app identifier
      uniqueAppID=appID;
      connect(this,SIGNAL(triggered(QAction*)), this, SLOT(sendSignal()) );
    }
    virtual ~SmallItemToolButton(){}
 
  	  
};

#endif
