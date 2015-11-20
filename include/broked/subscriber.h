#ifndef BROKED_SUBSCRIBER_H
#define BROKED_SUBSCRIBER_H

#include "socket.hpp"

#include <unordered_map>

namespace bd
{

class subscriber
{
public:
  using on_connect_callback_type = std::function<void(channel&)>;

  void on_connect(const internal_callback_type& callback)
  {
    m_connect_callback = callback;
  }

  void on_connect(const nullary_callback_type& callback)
  {
    m_connect_callback = [=](socket&) { callback(); };
  }

  void on(std::string event, const callback_type& callback)
  {
    m_callbacks[event] = callback;
  }

  void on(std::string event, const unary_callback_type& callback)
  {
    m_callbacks[event] = [=](socket& s, message&&) { callback(s); };
  }

  void on(std::string event, const nullary_callback_type& callback)
  {
    m_callbacks[event] = [=](socket&, message&&) { callback(); };
  }

  void trigger_connect(socket& s) const
  {
    if (m_connect_callback)
      m_connect_callback(s);
  }

  void trigger_callback(socket& s, message m) const
  {
    auto it = m_callbacks.find(m.event());
    if (it != m_callbacks.end())
      it->second(s, std::move(m));
  }

private:
  internal_callback_type m_connect_callback;
  std::unordered_map<std::string, callback_type> m_callbacks;
};

}

#endif // BROKED_SUBSCRIBER_H
