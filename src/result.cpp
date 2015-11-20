#include "broked/result.h"

namespace bd
{

bool operator==(const result& a, const result& b)
{
  return a.task == b.task && a.data == b.data;
}

bool operator!=(const result& a, const result& b)
{
  return !(a == b);
}

}
