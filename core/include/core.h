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

#include "config_manager.h"
#include "connection_manager.h"
#include "plugin_manager.h"
#include "event_manager.h"
#include "data_manager.h"

#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"

namespace sdc {

/**
 * @class SocialDesktopClient
 * @brief The heart of SocialDesktopClient application.
 */
class Core : boost::noncopyable {
 public:
	Core();
	~Core();
	void Start();
	void Exit();
	void Join();
	void SetGui(void* main_window);
	void* GetGui();

 private:
	void Init();
	void Exec();

	boost::thread core_;
	void* gui_;
};

}

#endif /* CORE_SOCIALDESKTOPCLIENT_H_ */
