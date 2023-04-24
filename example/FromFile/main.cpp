#include "activity.hpp"
#include <iostream>
#include <ostream>

auto static constexpr client_id = std::string_view{""};

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Please provide a .json file with the activity" << std::endl;
    return -1;
  }

  auto const connection = RPC::Connection{client_id};
  auto const activity = RPC::Activity::FromFile(argv[1]);

  auto message = activity.to_message();

  if (!connection.write(message)) {
    std::cerr << "Failed to write update" << std::endl;
    return -1;
  }

  if (!connection.read(message)) {
    std::cerr << "Failed to read update" << std::endl;
    return -1;
  }

  std::cout << "Got response: " << message.view() << std::endl;

  auto dummy = std::string{};
  std::getline(std::cin, dummy);
}