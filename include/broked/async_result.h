#ifndef BROKED_ASYNC_RESULT
#define BROKED_ASYNC_RESULT

#include "result.h"

class async_result
{
public:
  /*! Wrap a string in an async_result
   */
  explicit async_result(std::string s);

  /*! Wrap a result in an async_result.
   */
  explicit async_result(result r);

  template <class ReadyFunc>
  async_result on_ready(const ReadyFunc& f)
  {
    return { [f](result r) { return f(r); } };
  }

private:
  using callback_type = std::function<result(result)>;

  async_result(const callback_type& callback)
    : callback(callback)
  {
  }

  std::vector<task> dependencies;
  callback_type callback;
};


#endif // BROKED_ASYNC_RESULT
