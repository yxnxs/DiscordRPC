#include "ipc.hpp"
#include "activity.hpp"
#include <algorithm>
#include <djson/json.hpp>
#include <fileapi.h>
#include <minwindef.h>
#include <optional>
#include <string_view>

using namespace RPC;

Message Message::Create(std::string_view data) {
  auto message = Message{{1, static_cast<uint32_t>(data.size())}, {}};
  std::copy(data.begin(), data.end(), message.buffer);

  return message;
}

std::string_view Message::view() const { return {buffer, header.length}; }

Connection::Connection(std::string_view client_id) {
  auto name = std::wstring{L"\\\\?\\pipe\\discord-ipc-0"};

  for (wchar_t i{L'0'}; i <= L'9';) {

    pipe = CreateFileW(name.c_str(),
                       GENERIC_READ | GENERIC_WRITE | FILE_FLAG_OVERLAPPED, 0,
                       nullptr, OPEN_EXISTING, 0, nullptr);

    if (pipe != INVALID_HANDLE_VALUE) {
      break;
    }

    if (GetLastError() == ERROR_PIPE_BUSY) {
      WaitNamedPipeW(name.c_str(), 10000);
      continue;
    }
    i++;
  }

  if (pipe == INVALID_HANDLE_VALUE) {
    throw detail::HandshakeException("Can't connect to IPC pipe");
  }

  auto message = Message{};
  auto json = dj::Json{};
  {
    json["v"] = 1;
    json["client_id"] = client_id;

    message = Message::Create(json.serialized(0));
    message.header.code = 0;
  }

  if (!write(message)) {
    throw detail::HandshakeException("Failed to write handshake request");
  }

  if (!read(message)) {
    throw detail::HandshakeException("Failed to read handshake response");
  }

  json = dj::Json::parse(message.view());

  if (json["cmd"].as_string() != "DISPATCH" &&
      json["evt"].as_string() != "READY") {

    throw detail::HandshakeException(
        std::format("Bad handshake response: {}", json.serialized()));
  }
}

Connection::~Connection() { CloseHandle(pipe); }

bool Connection::write(const Message &message) const {
  DWORD sent{};
  DWORD size = sizeof(Header) + message.header.length;

  return WriteFile(pipe, &message, size, &sent, nullptr) && sent == size;
}

bool Connection::read(Message &message) const {
  DWORD read{};

  if (!ReadFile(pipe, &message.header, sizeof(Header), &read, nullptr) &&
      read == sizeof(Header))
    return false;

  if (!ReadFile(pipe, &message.buffer, message.header.length, &read, nullptr) &&
      read == message.header.length)
    return false;

  return true;
}