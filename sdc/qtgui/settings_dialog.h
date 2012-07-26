#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QtGui/QDialog>
#include "ui_settings_dialog.h"

namespace sdc {

class Service;

class SettingsListItem : public QListWidgetItem {
 public:
  SettingsListItem(QListWidget* parent, QWidget* referenced_page) : QListWidgetItem(parent) {
    page_ = referenced_page;
  }

  QWidget* GetPage() {
    return page_;
  }

 private:
  QWidget* page_;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

 public:
  SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();

 private slots:
  void ChangePage(QListWidgetItem* current, QListWidgetItem* previous);
  void ShowNewAccountDialog();
  void AccountSelected();
  void DeleteAccount();
  void EditAccount();

 private:
  Ui::SettingsDialogClass ui;
  std::vector<Service*> services_;
};

} /* namespace sdc */

#endif // SETTINGS_DIALOG_H
