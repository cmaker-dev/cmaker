#ifdef TEST
#include "Frate/Utils/General.hpp"
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNewWithType(std::string type){
  Utils::info << "Testing new command with type: " << type << std::endl;
  test_path = genTestDirectory(); 
  auto[failed,inter] = init("frate new -d -t " + type);

  if(failed) return false;

  if(inter->pro->type != type){
    
    Utils::error << "Failed to create new project : project type: " 
      << type << " not set" 
      << std::endl;
    return false;
  }
  return true;
}
#endif
