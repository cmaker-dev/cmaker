#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::System {
  class Capability {
  public:
    Capability();
    std::string executable;
    std::string version;
    std::string path;
    bool installed;
    friend void from_json(const nlohmann::json &json_obj,
                          Capability &capability);
    friend void to_json(nlohmann::json &json_obj, const Capability &capability);
  };
} // namespace Frate::System
