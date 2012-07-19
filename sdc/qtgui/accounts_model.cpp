#include "accounts_model.h"
#include "data_manager.h"
#include "bind.h"
#include "core.h"
#include <string>

using std::string;

namespace sdc {

AccountsModel::AccountsModel(Core* core) : core_(core) {
  sdc::bind(core_->data()->onAccountsChanged, [&] () {
    reset();
  });
}

int AccountsModel::rowCount(const QModelIndex &/*parent*/) const {
  return core_->data()->GetAccounts().size();
}

int AccountsModel::columnCount(const QModelIndex &/*parent*/) const {
  return 2;
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QVariant("Account Name");
      case 1:
        return QVariant("Enabled");
      default:
        return QVariant();
    }
  }
  return QVariant();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0: {
        string name = core_->data()->GetAccount(index.row())->GetUid();
        return QString::fromStdString(name);
      }
      case 1: {
        return QVariant();
      }
      default:
        return QVariant();
    }
  } else if (role == Qt::CheckStateRole && index.column() == 1) {
    bool enabled = core_->data()->GetAccount(index.row())->IsEnabled();
    if (enabled)
      return QVariant(Qt::Checked);
    else
      return QVariant(Qt::Unchecked);
  }
  return QVariant();
}

bool AccountsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::CheckStateRole) {
    if (index.column() == 1) {
      if (value.toInt() == Qt::Checked) {
        core_->data()->SetEnabledAccount(index.row(), true);
        emit dataChanged(index, index);
        return true;
      }
      else if (value.toInt() == Qt::Unchecked) {
        core_->data()->SetEnabledAccount(index.row(), false);
        emit dataChanged(index, index);
        return true;
      }
    }
  }
  return false;
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const {
  switch (index.column()) {
    case 0: {
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    case 1: {
      return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    }
    default:
      return Qt::NoItemFlags;
  }
}

} /* namespace sdc */
