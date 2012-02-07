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
#include "mvc_typedefs.h"
#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include <vector>

class QApplication;
class MainWindow;
class TestController;

namespace sdc {

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
	ConnectionManager* connection_manager() { return g_connection_manager; }

	int GetReturnCode() { return return_code_; }

	void RegisterController(ControllerRef controller);
	void RegisterModel(ModelRef model);

	void Process(boost::shared_ptr<Message> message); // Method that takes care of incomming message processing, it will be probably change to individual manager

 private:
	void Init();
	void Exec();
	void InitUi();
	void ExecUi();

	boost::thread core_;
	boost::mutex mutex_;
	boost::condition_variable gui_unprepared_;
	bool is_gui_prepared_;
	//boost::thread ui_; // Qt GUI cannot run in secondary thread
	QApplication* qt_app_;
	MainWindow* main_view_;

	boost::shared_ptr<TestController> test_controller_;

	std::vector<ModelRef> models_;
	std::vector<ViewRef> views_;
	std::vector<ControllerRef> controllers_;

	int return_code_;
};

}

#endif /* CORE_SOCIALDESKTOPCLIENT_H_ */
