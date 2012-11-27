/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ACCOUNTS_MODEL_H_
#define ACCOUNTS_MODEL_H_

#include <QAbstractTableModel>

namespace sdc {

class Core;

class AccountsModel : public QAbstractTableModel {
 public:
  AccountsModel(Core* core);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  Qt::ItemFlags flags(const QModelIndex &index) const;

 private:
  Core* core_;
};

} // namespace sdc

#endif // ACCOUNTS_MODEL_H_

