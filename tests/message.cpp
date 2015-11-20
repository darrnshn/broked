#include "gtest/gtest.h"
#include "broked/message.h"
#include "broked/serialization.h"

#include <vector>

class MessageTest : public ::testing::Test {
public:
  MessageTest()
    : ctx{1}
    , req{ctx, ZMQ_REQ}
    , rep{ctx, ZMQ_REP} { }

protected:
  virtual void SetUp() {
    rep.bind("tcp://*:5555");
    req.connect("tcp://localhost:5555");
  }

  zmq::context_t ctx;
  zmq::socket_t req;
  zmq::socket_t rep;
};

TEST_F(MessageTest, CanSendRecvHelloWithNoSubscribedTasks) {
  bd::hello_message original{{}};

  bd::socket_send_archive s_ar(req);
  serialize(s_ar, original);

  bd::socket_recv_archive r_ar(rep);
  bd::hello_message received;
  serialize(r_ar, received);

  EXPECT_EQ(original, received);
}

TEST_F(MessageTest, CanSendRecvHelloWithSubscribedTasks) {
  bd::hello_message original{{"A", "B"}};

  bd::socket_send_archive s_ar(req);
  serialize(s_ar, original);

  bd::socket_recv_archive r_ar(rep);
  bd::hello_message received;
  serialize(r_ar, received);

  EXPECT_EQ(original, received);
}
