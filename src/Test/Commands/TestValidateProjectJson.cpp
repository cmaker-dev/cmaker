#ifdef TEST
#include "Frate/Command.hpp"
#include "Frate/Utils/General.hpp"
#include <catch2/catch_test_macros.hpp>
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool validateProjectJson(Frate::Command::Interface* inter){
    Frate::Utils::Error error;
    Frate::Utils::Info info;
    bool valid = true;
    Utils::info << "Testing valid project json" << std::endl;

    if (inter->pro->lang == "") {
      valid = false;
      Utils::error << "language is not cpp or c" << std::endl;
    }
    if(inter->pro->name == ""){
      valid = false;
      Utils::error << "project name is empty" << std::endl;
    }
    if(inter->pro->path == ""){
      valid = false;
      Utils::error << "project path is empty" << std::endl;
    }
    if(inter->pro->version == ""){
      valid = false;
      Utils::error << "project version is empty" << std::endl;
    }
    if(inter->pro->src_dir == ""){
      valid = false;
      Utils::error << "src path is empty" << std::endl;
    }
    if(inter->pro->include_dir == ""){
      valid = false;
      Utils::error << "include path is empty" << std::endl;
    }
    if(inter->pro->lang_version == ""){
      valid = false;
      Utils::error << "language version is empty" << std::endl;
    }
    if(inter->pro->build_dir == ""){
      valid = false;
      Utils::error << "build path is empty" << std::endl;
    }
    if(inter->pro->cmake_version == ""){
      valid = false;
      Utils::error << "cmake version is empty" << std::endl;
    }
    if(inter->pro->modes.size() == 0){
      valid = false;
      Utils::error << "modes is empty" << std::endl;
    }else{
      for(Frate::Command::Mode mode : inter->pro->modes){
        if(mode.name == ""){
          valid = false;
          Utils::error << "mode "+mode.name+" has no name" << std::endl;
        }
      }
    }
    return valid;
  }
}
#endif
