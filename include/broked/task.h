#ifndef BROKED_TASK_H
#define BROKED_TASK_H

#include <string>

namespace bd {

/*! Represents a task sent by a worker.
 */
struct task {
  std::string name;
  std::string data;
};

bool operator==(const task& a, const task& b);
bool operator!=(const task& a, const task& b);

}

#endif // BROKED_TASK_H
