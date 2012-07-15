#ifndef ABSTRACT_MODEL_H
#define ABSTRACT_MODEL_H

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

#endif // ABSTRACT_MODEL_H
