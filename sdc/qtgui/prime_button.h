#ifndef PRIME_BUTTON_H
#define PRIME_BUTTON_H

#include <QWidget>
#include <QToolButton>

namespace Ui {
class PrimeButton;
}

namespace sdc {

class PrimeButton : public QWidget {

    Q_OBJECT

 public:
  explicit PrimeButton(QWidget *parent = 0);
  ~PrimeButton();

  QToolButton* button();

 private:
  Ui::PrimeButton *ui;
};

} /* namespace sdc */

#endif // PRIME_BUTTON_H
