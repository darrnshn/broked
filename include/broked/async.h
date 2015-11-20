#ifndef BROKED_ASYNC_H
#define BROKED_ASYNC_H

#include "broked/task.h"
#include "broked/result.h"

#include <memory>
#include <tuple>

namespace bd {

template <class AsyncImpl>
class async_wrapper : private std::shared_ptr<AsyncImpl> {
public:
  async_wrapper(AsyncImpl* impl)
    : std::shared_ptr<AsyncImpl>(impl) { }

  AsyncImpl* operator->() { return std::shared_ptr<AsyncImpl>::operator->(); };
  const AsyncImpl* operator->() const { return std::shared_ptr<AsyncImpl>::operator->(); };
};

template <class AsyncImpl, class...T>
async_wrapper<AsyncImpl> make_async(T&&...args) {
  return async_wrapper<AsyncImpl>(new AsyncImpl(std::forward<T>(args)...));
}

struct async_call_impl {
  async_call_impl(bd::task task)
    : task(std::move(task)) { }

  bd::task task;
};

template <class Callback, class...Results>
struct async_wait_all_impl {
  async_wait_all_impl(Callback callback, std::tuple<Results...> deps)
    : callback(callback), deps(deps) { }

  Callback callback;
  std::tuple<Results...> deps;
};

using async_call = async_wrapper<async_call_impl>;

template <class Callback, class...Results>
using async_wait_all = async_wrapper<async_wait_all_impl<Callback, Results...>>;

async_call async(task t) {
  return make_async<async_call_impl>(t);
}

template <class T, class...Args>
async_call wait_all(Args&&...args) {
  return make_async<async_call_impl>(callback, results...);
}

}

/*
class async_result {
private:
  template <class...Args>
  async_result(Args&&...args)
    : m_impl(new async_impl(std::forward<Args>(args)...)) { }

  std::shared_ptr<async_impl> m_impl;
};

class async_impl {
public:
  virtual ~async_impl() { }

  result result() const { return m_result; }

  result m_result;
};

class async_call_impl : public async_impl {
public:
  task_event_subscriber on_result(result r) override {
    task_event_subscriber sub;
    sub.unsubscribe(r.task.name);
    return sub;
  }

private:
  task m_task;
};

template <class Callback, class...Results>
class async_wait : public async_result {
public:
  async_wait_all_impl(std::tuple<Results...> deps) {

  task_event_subscriber on_result(result r) override {
    m_deps[].on_result(r);
    // unsubscribe
    // if not more deps, result = m_callback(results)
  }
private:
  std::tuple<Results...> m_deps;
  Callback m_callback;
}*/

#endif // BROKED_ASYNC_H
