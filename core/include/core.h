/**
 * @file social_desktop_client.h
 * @brief The heart of SocialDesktopClient application.
 *
 * @author Maros Kasinec
 *   @date Jul 8, 2011
 */

#ifndef CORE_SOCIALDESKTOPCLIENT_H_
#define CORE_SOCIALDESKTOPCLIENT_H_

#include "common.h"
#include "model.h"
#include "view.h"
#include "controller.h"
#include "test_controller.h"
#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include <QApplication>
#include <QtGui>
#include <vector>

class MainWindow;

namespace sdc {

class Controller;
class Model;
class Message;

/**
 * @class SocialDesktopClient
 * @brief The heart of SocialDesktopClient application.
 */
class Core : boost::noncopyable {
 public:
	Core(int argc, char* argv[]);
	~Core();
	void Start();
	void Exit();
	EventManager* event_manager() { return g_event_manager; }

	int GetReturnCode() { return return_code_; }

	void RegisterController(Controller::Ref controller);
	void RegisterModel(Model::Ref model);

	void Process(boost::shared_ptr<Message> message); // Method that takes care of incomming message processing, it will be probably change to individual manager

 private:
	void Init();
	void Exec();
	void InitUi();
	void ExecUi();

	boost::thread core_;
	//boost::thread ui_; // Qt GUI cannot run in secondary thread
	QApplication qt_app_;
	MainWindow* main_view_;

	boost::shared_ptr<TestController> test_controller_;

	std::vector<Model::Ref> models_;
	std::vector<View::Ref> views_;
	std::vector<Controller::Ref> controllers_;

	int return_code_;
};

}

#endif /* CORE_SOCIALDESKTOPCLIENT_H_ */
