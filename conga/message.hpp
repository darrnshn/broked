#pragma once

namespace conga
{

class message
{
public:
  message(std::string event = "", std::string data = "")
    : m_event(std::move(event))
    , m_data(std::move(data))
  {
  }

  const std::string& event() const { return m_event; }
  const std::string& data() const { return m_data; }
  const std::size_t size() const { return m_data.size(); }

private:
  std::string m_event;
  std::string m_data;
};

bool operator==(const message& a, const message& b)
{
  return a.event() == b.event() &&
         a.data() == b.data();
}

}
