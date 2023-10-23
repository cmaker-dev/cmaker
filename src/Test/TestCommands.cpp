#include <catch2/catch_test_macros.hpp>
#include "../Command/Command.h"
#include <cxxopts.hpp>
#include <memory>


namespace TestCommands {

  bool testCommandInit() {
    std::shared_ptr<Command::Context> context = std::make_shared<Command::Context>();

    return true;
  }

  TEST_CASE("TestCommands", "[commands]"){

    REQUIRE(testCommandInit());
  }
}

