#include "Frate/Project.hpp"
#include <Frate/Generators.hpp>
#include <Frate/LuaAPI.hpp>
#include <inja.hpp>

namespace Frate::Generators::Project {
  using inja::Environment;

  bool initializeLua(Environment &env, sol::state &lua,
                     std::shared_ptr<Command::Project> pro) {
    LuaAPI::registerAPI(lua);
    Utils::info << "Registering project" << std::endl;
    if (!LuaAPI::registerProject(lua, pro)) {
      Utils::error << "Error while registering project" << std::endl;
      return false;
    }
    Utils::info << "Registering project scripts at: "
                << pro->template_path / "scripts" << std::endl;
    if (!LuaAPI::registerProjectScripts(env, lua,
                                        pro->template_path / "scripts", pro)) {
      Utils::error << "Error while registering project scripts" << std::endl;
      return false;
    }
    return true;
  }
} // namespace Frate::Generators::Project
