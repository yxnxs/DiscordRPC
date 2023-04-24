#include "activity.hpp"
#include "ipc.hpp"
#include <iostream>
#include <optional>

auto static constexpr client_id = std::string_view{"client_id_here"};

int main() {
  auto const connection = RPC::Connection{client_id};
  auto activity = RPC::Activity{}
                      .setDetails("Coding...")
                      .setDetails("Why does it not work!?")
                      .setTimestamps({RPC::Activity::time(), std::nullopt});

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