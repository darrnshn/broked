#include "broked/task.h"

namespace bd
{

bool operator==(const task& a, const task& b)
{
  return a.name == b.name && a.data == b.data;
}

bool operator!=(const task& a, const task& b)
{
  return !(a == b);
}

}
