/**
 * @file xmltextedit.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 4, 2012
 */

#ifndef XMLTEXTEDIT_H_
#define XMLTEXTEDIT_H_

#include <QtGui/QTextEdit>
#include <QtGui>

#include "log.h"

class XmlTextEdit : public QTextEdit {

    Q_OBJECT

 public:
  XmlTextEdit(QWidget* &widget) : QTextEdit(widget) {}
  virtual void keyPressEvent(QKeyEvent* e) {
    if(e->key() == Qt::Key_Enter) {
      LOG(DEBUG3) << "ENTER KEY HIT!";
      emit onEnter();
    }
    QTextEdit::keyPressEvent(e);
  }
 signals:
  void onEnter();
};

#endif /* XMLTEXTEDIT_H_ */
