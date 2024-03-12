#include <stdint.h>
#include <termcolor/termcolor.hpp>
#include <Frate/Utils/CLIPrompt.hpp>
#include <Frate/Utils/CLIColors.hpp>
#include <Frate/Utils/CLIExceptions.hpp>
#if __LINUX___
#include <termio.h>
#endif
#if __APPLE__
#include <sys/ioctl.h>
#endif
#include <termios.h>
#include <unistd.h>
#include <sstream>

namespace Frate::Utils::CLI {
  Prompt::Prompt(std::string prompt) {
    this->prompt = prompt;
    this->color = Ansi::GREEN;
  }

  Prompt::Prompt(std::string prompt, std::string default_input)
      : prompt(prompt), color(Ansi::GREEN), input(default_input) {}

  Prompt &Prompt::addOption(std::string option) {
    this->options.emplace_back(option);
    return *this;
  }

  Prompt &Prompt::addOption(int option) {
    this->options.emplace_back(std::to_string(option));
    return *this;
  }

  Prompt &Prompt::addOption(float option) {
    this->options.emplace_back(std::to_string(option));
    return *this;
  }

  Prompt &Prompt::addOption(size_t option) {
    this->options.emplace_back(std::to_string(option));
    return *this;
  }

  Prompt &Prompt::maxLength(int max_length) {
    this->max_length = max_length;
    return *this;
  }

  Prompt &Prompt::addOptions(std::vector<std::string> options) {
    this->options = options;
    return *this;
  }

  Prompt &Prompt::setColor(std::string color) {
    this->color = color;
    return *this;
  }

  Prompt &Prompt::setValidator(std::function<bool(std::string)> validator) {
    this->validator = validator;
    return *this;
  }

  Prompt &Prompt::isBool() {
    this->is_bool = true;
    return *this;
  }

  Prompt &Prompt::exitOnFailure() {
    this->exit_on_failure = true;
    return *this;
  }

  Prompt &Prompt::printValidOptions() {
    print_valid_options = true;
    return *this;
  }

  bool Prompt::has_options() { return !options.empty(); }

  bool Prompt::has_max_length() { return max_length > 0; }

  bool Prompt::has_validator() { return validator != nullptr; }

  bool Prompt::is_valid() {
    if (has_max_length()) {
      if (input.length() > max_length) {
        return false;
      }
    }
    if (has_options()) {
      if (!is_in_options(input)) {
        return false;
      }
    }
    if (has_validator()) {
      if (!validator(input)) {
        return false;
      }
    }
    return true;
  }

  void Prompt::current_input() {
    if (is_valid()) {
      std::cout << termcolor::green << input << Ansi::RESET;
    }
    else {
      std::cout << termcolor::red << input << Ansi::RESET;
    }
  }

  void Prompt::prompt_prefix(int term_width) {
    int text_length = 0;
    if (is_bool) {
      std::string booloptions = "[y/n]";
      std::cout << color << prompt << termcolor::white << booloptions
                << Ansi::RESET;
      text_length += prompt.length() + booloptions.length();
    }
    else if (has_options()) {
      // Displays the default option
      std::cout << color << prompt << termcolor::white << "[" << color
                << default_input << termcolor::white << "]:" << Ansi::RESET
                << "\n";

      text_length = 0;
      cursor_position++;

      if (print_valid_options) {

        for (size_t i = 0; i < visible_options.size(); i++) {

          std::cout << "[" << visible_options[i] << "]";
          text_length += visible_options[i].length() + 2;
          if (i % columns == columns - 1) {

            std::cout << "\n";
            text_length = 0;
            cursor_position++;
          }
          else if (i == visible_options.size() - 1) {

            text_length = 0;
            cursor_position++;
            std::cout << "\n";
          }
        }
      }
      std::cout << ">";
      text_length += 1;
    }
    else {
      std::ostringstream size_fraction;
      if (has_max_length()) {
        size_fraction << termcolor::green << "[" << input.size() << "/"
                      << max_length << "]";
      }
      else {
        size_fraction << "";
      }
      std::cout << color << prompt << termcolor::white << size_fraction.str()
                << " : " << Ansi::RESET;
      text_length += prompt.length() + size_fraction.str().length() + 3;
    }
    cursor_position += text_length / term_width;
  }

  void Prompt::get_input() {

#ifdef TEST
    return;
#endif

    struct termios oldt, newt;
    struct winsize term_size;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);

    prompt_prefix(term_size.ws_col);
    std::cout << input;

    char c;
    std::string completion;
    size_t completion_index = 0;
    while (true) {
      if (!visible_options.empty()) {
        visible_options.clear();
      }
      for (std::string &option : options) {
        if (option.find(input) != std::string::npos) {
          visible_options.emplace_back(option);
          if (visible_options.size() >= result_limit) {
            break;
          }
        }
      }

      c = getchar();
      if (c == '\n') {
        // Checks the options to see if there is an exact match
        for (std::string &option : options) {
          if (option == input) {
            completion = option;
            break;
          }
        }
        // If there is a completion then it will set the input to the completion
        if (completion.size() > 0) {
          input = completion;
        }
        // If it's not valid then we don't want to finish
        if (!is_valid()) {
          continue;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return;
        // Tab
      }
      else if (c == 9) {
        if (has_options()) {
          if (completion.empty()) {
            if (visible_options.size() == 1) {
              completion = visible_options[0];
            }
            else {
              completion = visible_options[completion_index];
            }
          }
          else {
            completion_index++;
            if (completion_index >= visible_options.size()) {
              completion_index = 0;
            }
            completion = visible_options[completion_index];
          }
        }
      }
      else if (c == 127) {
        if (completion.size() > 0) {
          input = completion;
          completion = "";
        }

        if (input.size() > 0) {
          input.pop_back();
        }
      }
      else {
        input += c;
      }
      for (int i = 0; i < cursor_position; i++) {
        std::cout << "\33[2K\r";
        std::cout << "\33[A";
      }
      std::cout << "\33[2K\r";
      cursor_position = 0;
      prompt_prefix(term_size.ws_col);
      if (completion.size() > 0) {
        std::cout << completion;
      }
      else {
        current_input();
      }
    }
    // std::getline(std::cin, input);
  };

  bool Prompt::is_in_options(std::string option) {
    for (size_t i = 0; i < options.size(); i++) {
      if (options[i] == option) {
        return true;
      }
    }
    return false;
  }

  template <> bool Prompt::get() {
#ifdef TEST
    return true;
#endif
    if (input == "y" || input == "Y") {
      return true;
    }
    else if (input == "n" || input == "N") {
      return false;
    }
    else {
      throw InputInvalid("Input is not a boolean");
    }
  }

  template <> std::string Prompt::get() {
    if (has_options()) {
      if (is_in_options(input)) {
        return input;
      }
      else {
        throw InputNotInOptions("Input is not in option " + input);
      }
    }
    else {
      return input;
    }
  }

  template <> int Prompt::get() {
    if (has_options()) {
      if (is_in_options(input)) {
        try {
          return std::stoi(input);
        } catch (std::exception &e) {
          std::cout << "Invalid int: " << input << std::endl;
          throw InputInvalid("Input " + input + " is not an int");
        }
      }
      else {
        throw InputNotInOptions("Input is not in option " + input);
      }
    }
    else {
      try {
        return std::stoi(input);
      } catch (std::exception &e) {
        throw InputInvalid("Input " + input + " is not an int");
      }
    }
  }

  template <> float Prompt::get() {
    if (has_options()) {
      if (is_in_options(input)) {
        try {
          return std::stof(input);
        } catch (std::exception &e) {
          throw InputInvalid("Input " + input + " is not a float");
        }
      }
      else {
        throw InputNotInOptions("Input is not in option " + input);
      }
    }
    else {
      try {
        return std::stof(input);
      } catch (std::exception &e) {
        throw InputInvalid("Input " + input + " is not a float");
      }
    }
  }

  void Prompt::run() {
    get_input();
    if (has_max_length()) {
      while (input.length() > max_length) {
        if (exit_on_failure) {
          exit(1);
        }
        throw InputTooLong("Input too long");
      }
    }
    std::cout << std::endl;
  }
} // namespace Frate::Utils::CLI
