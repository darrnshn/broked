#include "gtest/gtest.h"
#include "broked/serialization.h"
#include "broked/transport.h"

class SerializationTest : public ::testing::Test {
public:
  SerializationTest()
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

TEST_F(SerializationTest, CanSendSingleString)
{
  bd::socket_send_archive s_ar{req};
  s_ar("hello");
  auto msg = bd::transport::recv_multipart(rep);

  ASSERT_EQ(1U, msg.size());
  EXPECT_EQ("hello", msg[0]);
}

TEST_F(SerializationTest, CanSendTwoStrings)
{
  bd::socket_send_archive s_ar{req};
  s_ar("hello", "world");
  auto msg = bd::transport::recv_multipart(rep);

  ASSERT_EQ(2U, msg.size());
  EXPECT_EQ("hello", msg[0]);
  EXPECT_EQ("world", msg[1]);
}

TEST_F(SerializationTest, CanSendStringsAndVectors)
{
  bd::socket_send_archive s_ar{req};
  s_ar("hello", std::vector<std::string> { "A", "B" }, "world");
  auto msg = bd::transport::recv_multipart(rep);

  ASSERT_EQ(3U, msg.size());
  EXPECT_EQ("hello", msg[0]);
  EXPECT_EQ(std::string("A\0B", 3), msg[1]);
  EXPECT_EQ("world", msg[2]);
}

TEST_F(SerializationTest, CanRecvSingleString)
{
  bd::transport::send_multipart(req, { "hello" });

  bd::socket_recv_archive r_ar{rep};
  std::string result;
  r_ar(result);

  EXPECT_EQ("hello", result);
}

TEST_F(SerializationTest, CanRecvTwoStrings)
{
  bd::transport::send_multipart(req, { "hello", "world" });

  bd::socket_recv_archive r_ar{rep};
  std::string result1, result2;
  r_ar(result1, result2);

  EXPECT_EQ("hello", result1);
  EXPECT_EQ("world", result2);
}

TEST_F(SerializationTest, CanRecvStringsAndVectors)
{
  bd::transport::send_multipart(req, { "hello", std::string{"A\0B", 3}, "world" });

  bd::socket_recv_archive r_ar{rep};
  std::string result1, result3;
  std::vector<std::string> result2;
  r_ar(result1, result2, result3);

  EXPECT_EQ("hello", result1);
  ASSERT_EQ(2U, result2.size());
  EXPECT_EQ("A", result2[0]);
  EXPECT_EQ("B", result2[1]);
  EXPECT_EQ("world", result3);
}
