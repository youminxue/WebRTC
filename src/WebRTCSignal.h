#include "sio_client.h"

#include <mutex>

using namespace sio;

class WebRTCSignal {
  public:
    WebRTCSignal();
    ~WebRTCSignal();

    void connect(const std::string &url, std::function<void(void)> connected);
    void create_join_room(const std::string& room);
    void set_message_listener(std::function<void(message::ptr const&)> listener);
    void send_message(const std::string& msg);

  private:
    sio::client h;
    socket::ptr current_socket;
    std::mutex _lock;
    bool connect_finish;
    std::function<void(void)> onConnected;
    std::function<void(message::ptr const&)> onMessage;

    void print_message(message::ptr const& msg);
    void bind_events();
    void on_connected();
    void on_close(client::close_reason const& reason);
    void on_fail();
};
