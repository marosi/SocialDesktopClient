/**
 * @file controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef B_CONTROLLER_H_
#define B_CONTROLLER_H_

#include "sdc.h"

class BModel;
class BView;
class BuddycloudView;
class ContactFrameView;

class BController : public QObject, public sdc::ServiceController {

  Q_OBJECT

 public:
  BController();
  ~BController();
  void Initiate();

  void SynchronizeContacts() {}

  /// XMPP testing @{
  void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
  void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);
  void DoSomeThing(const std::string &param);
  /// @}

  // Qt signal wrappers

 public slots:
  void SwitchOnlineState(const QString &state);
  void ShowXmppCommandsWindow();

 signals: // signals in controller are used to manipulate Qt view
  void signalOnlineState(const QString &);
  void signalShowContact(const QString & uid, const QString & name);
  void signalShowPost(const QString & author, const QString & content);

 private:
  void ConnectView();
// Actions
  void GoOnline();
  void HandleIsOnline();
  void GoOffline();
  //void HandleIsOffline();
//
  void GetRemoteContacts();
  void HandleRemoteContacts(sdc::Contacts::Ref contacts);

  void GetPosts();
  void HandleGetPosts(sdc::Posts::Ref posts);

  boost::shared_ptr<BModel> model_;
  BuddycloudView* channel_view_;

  BView* xmpp_commands_view_;

  std::string state_to_be_acknowledged_;
};

#endif /* B_CONTROLLER_H_ */
