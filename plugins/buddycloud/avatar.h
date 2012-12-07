/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef AVATAR_H_
#define AVATAR_H_

#include <QObject>
#include <QPixmap>

/**
 * Structure for avatar images.
 */
class Avatar : public QObject {

    Q_OBJECT

 public:
  Avatar(const std::string &file) {
    Load(file);
  }
  Avatar(const QPixmap &pixmap) {
    avatar_ = pixmap;
  }

  /**
   * Loads avatar image file.
   * @param file image file name
   */
  void Load(const std::string &file) {
    avatar_.load(QString::fromStdString(file));
    changed(avatar_);
  }

  /**
   * Gets QPixmap of loaded avatar.
   * @return
   */
  QPixmap GetPixmap() {
    return avatar_;
  }

 signals:
  /**
   * Signal is emited when avatar changes.
   */
  void changed(const QPixmap &);

 private:
  QPixmap avatar_;
};

#endif // AVATAR_H_
