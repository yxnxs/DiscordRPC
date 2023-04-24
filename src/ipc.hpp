#pragma once

#include <Windows.h>
#include <handleapi.h>
#include <stdexcept>
#include <string>
#include <string_view>

namespace RPC {
struct Header {
  uint32_t code{};
  uint32_t length{};
};

static constexpr auto FrameSize = 64 * 1024 - sizeof(Header);

struct Message {
  Header header{};
  char buffer[FrameSize];

  static Message Create(std::string_view data);

  std::string_view view() const;
};

struct Connection {
  HANDLE pipe{INVALID_HANDLE_VALUE};

  explicit Connection(std::string_view client_id);
  ~Connection();

  bool read(Message &message) const;
  bool write(const Message &message) const;
};
} // namespace RPC