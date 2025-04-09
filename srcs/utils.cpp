#include <sstream> // needed for std::stringstream
#include <stdexcept>
#include <cstdlib>
#include <cctype>
#include "../includes/utils.hpp"

namespace Utils {
  int validateInputs(const std::string& portStr, const std::string& password) {
    std::string errorMsg;

    // Validate port received
    bool validPort = true;
    for (size_t i = 0; i < portStr.length(); ++i) {
      if (!std::isdigit(portStr[i])) {
        errorMsg += "Port must be a number. ";
        validPort = false;
        break;
      }
    }

    int port = -1;
    if (validPort) {
      std::stringstream ss(portStr);
      ss >> port;
      if (ss.fail() || !ss.eof())
        errorMsg += "Invalid port number format. ";
      else if (port < 1024 || port > 65535)
        errorMsg += "Port must be between 1024 and 65535. ";
    }

    // Validate password received
    if (password.empty())
      errorMsg += "Password cannot be empty.";

    // Put errors together
    if (!errorMsg.empty())
      throw std::invalid_argument(errorMsg);

    return port;
  }
}
