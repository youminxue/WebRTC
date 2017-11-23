#include "WebRTCSignal.h"

#include <unistd.h>
#include <syscall.h>
#include <iostream>

using namespace std;

WebRTCSignal::WebRTCSignal()
  : connect_finish(false)
  , onConnected(nullptr)
  , onMessage(nullptr)
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
  if (onConnected)
    onConnected();
  _lock.unlock();
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

void WebRTCSignal::connect(const string &url, std::function<void(void)> connected)
{
  h.connect(url);
  onConnected = connected;
}

void WebRTCSignal::bind_events()
{
  current_socket->on("message", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << ":";
    print_message(data);
    std::cout<< std::endl;
    if (onMessage)
      onMessage(data);
    _lock.unlock();
  }));

  current_socket->on("created", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << ":";
    print_message(data);
    std::cout<< std::endl;
    _lock.unlock();
  }));

  current_socket->on("full", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << ":";
    print_message(data);
    std::cout<< std::endl;
    _lock.unlock();
  }));

  current_socket->on("join", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << ":";
    print_message(data);
    std::cout<< std::endl;
    _lock.unlock();
  }));

  current_socket->on("joined", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << ":";
    print_message(data);
    std::cout<< std::endl;

    string msg = "got user media";
    current_socket->emit("message", msg);
    _lock.unlock();
  }));

  current_socket->on("log", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp){
    _lock.lock();
    std::cout<< name << syscall(SYS_gettid) << ":";
    print_message(data);
    std::cout<< std::endl;
    _lock.unlock();
  }));
}

void WebRTCSignal::set_message_listener(std::function<void(message::ptr const&)> listener)
{
  onMessage = listener;
}

void WebRTCSignal::send_message(const string& msg)
{
  current_socket->emit("message", msg);
}

void WebRTCSignal::create_join_room(const string& room)
{
  bind_events();
  current_socket->emit("create or join", room);
}

void WebRTCSignal::print_message(const message::ptr& msg)
{
  switch(msg->get_flag()) {
    case message::flag_integer:
      std::cout << msg->get_int();
      break;
    case message::flag_double:
      std::cout << msg->get_double();
      break;
    case message::flag_string:
      std::cout << msg->get_string();
      break;
    case message::flag_binary:
      std::cout << "binary";
      break;
    case message::flag_array:{
      const std::vector<message::ptr>& ptrs = msg->get_vector();
      std::cout << "[";
      for(int i=0; i<ptrs.size(); i++)
        print_message(ptrs[i]);
      std::cout << "]";
      break;
    }
    case message::flag_object:{
      const map<string,message::ptr>& maps = msg->get_map();
      map<string,message::ptr>::const_iterator it = maps.begin();
      std::cout << "{";
      while(it != maps.end()) {
        std::cout << it->first << "(";
        print_message(it->second);
        std::cout << ")";
        it++;
      }
      std::cout << "}";
      break;
    }
    case message::flag_boolean:
      std::cout << msg->get_bool() << std::endl;
      break;
    case message::flag_null:
      std::cout << "null";
      break;
  }
}
