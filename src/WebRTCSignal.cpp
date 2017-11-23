#include "WebRTCSignal.h"

#include <iostream>
#include <thread>
#include <condition_variable>

using namespace std;

WebRTCSignal::WebRTCSignal()
  : connect_finish(false)
{
  h.set_open_listener(bind(&WebRTCSignal::on_connected, this));
  h.set_close_listener(bind(&WebRTCSignal::on_close, this, placeholders::_1));
  h.set_fail_listener(bind(&WebRTCSignal::on_fail, this));
}

WebRTCSignal::~WebRTCSignal()
{
  h.sync_close();
  h.clear_con_listeners();
}

void WebRTCSignal::on_connected()
{
  std::cout<<"sio connected"<<std::endl;
  _lock.lock();
  connect_finish = true;
	current_socket = h.socket();
  _lock.unlock();

  string room = "foo";
  current_socket->emit("create or join", room);
}

void WebRTCSignal::on_close(client::close_reason const& reason)
{
  std::cout<<"sio closed "<<std::endl;
  _lock.lock();
  connect_finish = false;
  _lock.unlock();
}

void WebRTCSignal::on_fail()
{
  std::cout<<"sio failed "<<std::endl;
  _lock.lock();
  connect_finish = false;
  _lock.unlock();
}

void WebRTCSignal::connect(const string &url)
{
  h.connect(url);
}

void WebRTCSignal::bind_events()
{
  current_socket->on("message", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
  }));
}

void WebRTCSignal::set_message_listener()
{
}

void WebRTCSignal::send_message(const string& msg)
{
  current_socket->emit("message", msg);
}
