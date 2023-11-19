#include <string>
#include <nlohmann/json.hpp>
#include <Frate/Command.hpp>
#include <Frate/Utils/General.hpp>
#include <inja.hpp>

namespace Generators::Toolchain{
  using nlohmann::json;
  std::string generateToolchain(std::string  toolchain){
    std::string toolchain_template = R"(
set(CMAKE_C_COMPILER "zig")
set(CMAKE_CXX_COMPILER "zig")

set(ZIG_TARGET "{{ toolchain }}")

set(CMAKE_C_FLAGS "cc -target ${ZIG_TARGET}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "c++ -target ${ZIG_TARGET}" CACHE STRING "" FORCE)
)";
    return inja::render(toolchain_template, json{{"toolchain",toolchain}});
  }



}
