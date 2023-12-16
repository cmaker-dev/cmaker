#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddAuthorMultiple(std::vector<std::string> names) {
    

    Utils::info << "Testing add author command with names: " << std::endl;
    for(auto name : names){
      Utils::info << name << std::endl;
    }

    if (!testNew()) {
      Utils::error << "Failed to create new project" << std::endl;
      return false;
    }

    std::string command = "frate add author ";

    for (auto name : names) {
      command += name + " ";
    }
    auto [failed, inter] = init(command);

    if (failed) {
      Utils::error << "Failed to init" << std::endl;
      
      return false;
    }

    if (inter->pro->authors.size() != names.size()) {
      
      Utils::error << "Failed to add author project wrong number of authors" << std::endl;
      Utils::error << "Got: " << inter->pro->authors.size() << std::endl;
      return false;
    }
    for(auto name : names){
      bool found = false;
      for(auto author : inter->pro->authors){
        if(author == name){
          found = true;
        }
      }
      if(!found){
        Utils::error << "Failed to add author project" << std::endl;
        Utils::error << "Got: " << inter->pro->authors[0] << std::endl;
        
        return false;
      }
    }


    
    return true;
  }
}
#endif
