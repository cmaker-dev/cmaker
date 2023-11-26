#include <Frate/Command/RemoteServers.hpp>  
#include <Frate/Utils/General.hpp>

namespace Command::RemoteServers {

  bool  get(Interface* inter){
    Utils::TableFormat table;
    table.width = 20;
    table << "Name" << "Address" << "Port" << "Username" << "AuthMethod" << ENDL;
    table << inter->pro->build_server.name << inter->pro->build_server.ip << inter->pro->build_server.port << inter->pro->build_server.username << inter->pro->build_server.authMethod << ENDL;
    return true;
  }

}
