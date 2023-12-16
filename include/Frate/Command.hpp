#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <cxxopts.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Frate.hpp>

#define ENDL "\n"

namespace Frate::Command {

  using nlohmann::json;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  using namespace cxxopts;


  /*
   * Package structure from index
   * @param name the name of the package
   * @param url the url of the package
   * @param versions the versions of the package
   * @param target_link the target link of the package
   * @param description the description of the package
   * @param score the score of the package, used for sorting
   */
  typedef struct Package_s {
    std::string name;
    std::string git;
    std::string git_short;
    std::string git_prefixed;
    std::vector<std::string> versions;
    std::string target_link;
    std::string description;
    std::string selected_version;
    std::string git_description;
    std::string language;
    std::string license;
    std::string owner;
    std::string owner_type;
    int stars;
    int forks;
    int open_issues;
    int watchers;
    int score;
    json toJson();
    void fromJson(json j);
    bool addCallback(sol::state &lua);
  } Package;//Deez nuts

  typedef struct RemoteServer{
    std::string name;
    std::string ip;
    std::string username;
    std::string authMethod;
    std::optional<std::string> password;
    std::optional<std::string> key;
    int port;
  } RemoteServer;

  namespace ProjectType {
    const std::string EXECUTABLE = "executable";
    const std::string HEADER_ONLY = "header_only";
    const std::string STATIC_LIBRARY = "static_library";
    const std::string SHARED_LIBRARY = "shared_library";
    constexpr bool validate(std::string type) {
      return type == EXECUTABLE 
        || type == HEADER_ONLY 
        || type == STATIC_LIBRARY 
        || type == SHARED_LIBRARY;
    }
  };

  typedef struct Mode{
    std::string name;
    std::vector<std::string> flags;
    std::vector<Package> dependencies{};
  } Mode;

  typedef struct ProjectPrompt_s {
    std::string value{""};
    std::string text{""};
    std::string type{"string"};
    // std::string key{""};
    std::string default_value{""};
    bool required{false};
    std::vector<std::string> options{};
    std::function<bool(std::string)> validator{
      [this](std::string s) -> bool {
        if(options.size() == 0) {
          return true;
        }else{
          for (std::string option: options){
            if (s == option){
              return true;
            }
          }
          return false;
        }
      }
    };
    template<typename T>
    T get();
  } ProjectPrompt;

  //TODO: MAKE MOST OF THESE OPTIONAL
  typedef struct Project_s {
    std::string name;
    std::string description;
    std::string type{""};
    RemoteServer build_server;
    std::filesystem::path path;
    std::string git{"null"};
    std::string homepage{"null"};
    std::string bugs{"null"};
    std::string lang{"cpp"};
    std::string cmake_version{"3.10"};
    std::string lang_version{"20"};
    std::string compiler{"g++"};
    std::string license{""};
    std::string default_mode{"Release"};
    std::string build_command;
    std::vector<Mode> modes{
      Mode{.name = "Release", .flags={"-O2 "}}, 
      Mode{.name= "Debug", .flags= {"-g"}},
      Mode{.name= "Test", .flags={"-g"}}
    };
    std::vector<std::string> authors;
    std::vector<std::string> keywords;
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::vector<RemoteServer> build_servers;
    std::vector<Package> dependencies;
    std::string build_dir{"build"};
    Package testing_lib;
    std::string version{"0.0.1"};
    std::vector<std::string> flags; 
    std::vector<std::string> toolchains {};
    std::vector<std::string> libs{};
    void fromJson(json j);
    nlohmann::json toJson();
    bool save();
    void checkKeys(json j);
    std::string getPath(){
      return path.string();
    };
    std::unordered_map<std::string,ProjectPrompt> prompts{};
    std::unordered_map<std::string,json> variables{};
  } Project;


  typedef struct Handler_s Handler;
  class Interface;

  typedef struct Handler_s {
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<Handler> subcommands{};
    std::vector<std::string> positional_args{};
    std::string docs{""};
    std::function<bool(std::shared_ptr<Interface> )> callback{
      [](std::shared_ptr<Interface> inter) -> bool {
        (void)inter;
        std::cout << "This command has not been implemented yet" << std::endl;
        return false;
      }
    };
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
  } Handler;

  class Interface : public std::enable_shared_from_this<Interface>{
    public:
      Interface(int argc, char **argv);
      void getHelpString(std::string name,std::vector<Handler> &handlers,bool is_subcommand = false);
      void getHelpString(Handler &handler);
      bool runCommand(std::string,std::vector<Handler>&);
      std::shared_ptr<Project> pro;
      bool project_present{false};
      //bool verbose{false};
      std::vector<Handler> commands{};

      //bool execute();
      bool parse();
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      char** argv;
      int argc;
      bool confirm_all{false};
      bool InitHeader();
      bool CreateCMakelists();
      bool LoadProjectJson();
      ~Interface();
  };

  bool execute(std::shared_ptr<Interface> inter);


  namespace OptionsInit{
      bool Main(std::shared_ptr<Interface> inter);
  };

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

}
