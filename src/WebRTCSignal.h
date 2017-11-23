#include "sio_client.h"

#include <mutex>

using namespace sio;

class WebRTCSignal {
  public:
    WebRTCSignal();
    ~WebRTCSignal();
    void connect(const std::string &url);

    void bind_events();
    void set_message_listener();
    void send_message(const std::string& msg);

  private:
    sio::client h;
    socket::ptr current_socket;
    std::mutex _lock;
    bool connect_finish;

    void on_connected();
    void on_close(client::close_reason const& reason);
    void on_fail();
};
