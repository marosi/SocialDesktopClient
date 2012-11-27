/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ABSTRACT_MODEL_H_
#define ABSTRACT_MODEL_H_

class BcModel;

class AbstractModel {
 public:
  AbstractModel(BcModel* model) : model_(model) {}
  AbstractModel(AbstractModel* model) : model_(model->model()) {}

 protected:
  BcModel* model() {
    return model_;
  }

 private:
  BcModel* model_;
};

#endif // ABSTRACT_MODEL_H_
