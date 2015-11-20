#include "broked/message.h"

namespace bd {

bool operator==(const hello_message& a, const hello_message& b) {
  return a.subscribed_tasks == b.subscribed_tasks;
}

}
