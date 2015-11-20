#include "broked/broked.h"

int main(int argc, char* argv[]) {
  bd::subscriber sub;

  sub.on("ping", [](bd::task t) {
    return "pong";
  });

  sub.on_connect([]() {
    return bd::async("hello", [](bd::result r) {
      std::cout << "Received: " << r.data() << std::endl;
    });
  });

  bd::connect("foo", sub);
}
