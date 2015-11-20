#include "gtest/gtest.h"
#include "broked/async.h"

TEST(AsyncCall, CanCreateFromTask)
{
  const bd::task t{"connect", "hello world"};
  const auto call = bd::make_async<bd::async_call_impl>(t);
  EXPECT_EQ(call->task, t);
}

TEST(AsyncCall, CanCreateFromHelper)
{
  const bd::task t{"connect", "hello world"};
  const auto call = bd::make_async<bd::async_call_impl>(t);
  EXPECT_EQ(call->task, t);
}

TEST(AsyncWaitAll, CanCreateWaitOnOne)
{
  const auto call = bd::make_async<bd::async_call_impl>(bd::task{"hi", "data"});

  const auto callback = [](bd::result r) {
    return r;
  };

  const auto wait = bd::make_async<
    bd::async_wait_all_impl<decltype(callback), bd::async_call>
  >(callback, std::make_tuple(call));

  EXPECT_EQ(wait->callback, callback);
}

TEST(AsyncWaitAll, CanCreateFromHelper)
{
  const auto call = bd::async(bd::task{"hi", "data"});
  const auto callback = [](bd::result r) {
    return r;
  };

  const auto wait = bd::wait_all(call, callback);

  EXPECT_EQ(wait->callback, callback);
}
