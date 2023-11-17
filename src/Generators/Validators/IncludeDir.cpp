
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <CMaker/Command.hpp> 
#include <CMaker/Generators.hpp> 


namespace Generators::ConfigJson{
  /*
   * Reads the data from the user
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the data is valid
   */
  bool validateIncludeDir(std::string prefix, std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_json->include_dir);
#endif
    //If the include directory is empty we're gonna set it
    if(config_json->include_dir == "") {
      goto end;
    }
    if (config_json->include_dir.size() > 255) {
      return false;
    }

    //check if the include directory is valid
    if (std::regex_match(config_json->include_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }

    return false;
    end:
      pro->include_dir = config_json->include_dir == "" ? pro->include_dir : config_json->include_dir;
    
    return true;
  }
}
