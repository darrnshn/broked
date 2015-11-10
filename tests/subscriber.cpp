#include "gtest/gtest.h"
#include "subscriber.hpp"

#include "socket.hpp"

TEST(Subscriber, CanAttachOnConnect)
{
  bd::subscriber sub;
  sub.on_connect([]() { });
}

TEST(Subscriber, CanAttachAnEmptyCallback)
{
  bd::subscriber sub;
  sub.on("test", []() { });
}

TEST(Subscriber, CanAttachACallbackWithSocket)
{
  bd::subscriber sub;
  sub.on("test", [](bd::socket&) { });
}
