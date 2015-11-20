#ifndef BROKED_MESSAGE_H
#define BROKED_MESSAGE_H

#include <string>
#include <vector>

namespace bd {

struct hello_message {
  std::vector<std::string> subscribed_tasks;
};

template <class Archive>
void serialize(Archive& ar, hello_message& m) {
  ar(m.subscribed_tasks);
}

bool operator==(const hello_message& a, const hello_message& b);

/*
struct task_message
{
  task_message(std::string name, std::string data);

  std::string name;
  std::string data;
}

struct result_message
{
  result_message(std::string task, std::string data);

  std::string task;
  std::string data;
}

struct bye_message
{
};*/

}

#endif // BROKED_MESSAGE_H
