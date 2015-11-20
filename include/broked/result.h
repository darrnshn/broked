#ifndef BROKED_RESULT_H
#define BROKED_RESULT_H

#include <string>

namespace bd {

/*! Represents the result of a task computed by a worker.
 */
struct result {
  std::string task;
  std::string data;
};

bool operator==(const result& a, const result& b);
bool operator!=(const result& a, const result& b);

}

#endif // BROKED_RESULT_H
