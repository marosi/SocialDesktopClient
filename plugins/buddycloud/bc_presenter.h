/**
 * @file bc_presenter.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 5, 2012
 */

#ifndef BC_PRESENTER_H_
#define BC_PRESENTER_H_

#include "service_presenter.h"
#include "bc_model.h"

class BcPresenter : public sdc::ServicePresenter {

    Q_OBJECT

 public:
  void Init();

 public slots:
  void Test();

 private:
  BcModel* model_;
};

#endif /* BC_PRESENTER_H_ */
