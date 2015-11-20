#include "broked/broked.h"

int main(int argc, char* argv[]) {
  bd::subscriber sub;

  sub.on("fib", [](bd::task t) {
    using bd::serializers::text;

    int n = bd::get<int>(t.data);
    auto f1 = bd::async("fib", n - 1);
    auto f2 = bd::async("fib", n - 2);

    return bd::wait_all(f1, f2, [](bd::result r1, bd::result r2) {
      return { bd::get<int>(r1.data) + bd::get<int>(r2.data) };
    });
  });
}
