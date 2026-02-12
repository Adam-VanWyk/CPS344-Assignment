#include <algorithm> // For std::reverse
#include <array>
#include <asio.hpp>
#include <cstdint>
#include <cstring> // For memcpy and string operations
#include <iostream>
#include <ctime> // time

#include "app_message.h"

using asio::ip::tcp;

void handle_app1(const char *input, tcp::socket &socket) {
  std::string response(input);
  std::transform(response.begin(), response.end(), response.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  socket.send(asio::buffer(response), 0);
}

void handle_app2(const char *input, tcp::socket &socket) {
  std::string response(input);
  std::reverse(response.begin(), response.end());
  socket.send(asio::buffer(response), 0);
}

void handle_app3(const char *input, tcp::socket &socket) {
  std::string response = std::to_string(strlen(input));
  socket.send(asio::buffer(response), 0);
}

int main() {
  asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));
  while (true) {
    tcp::socket socket(io_context);
    acceptor.accept(socket);
    std::vector<char> buffer(sizeof(uint32_t) + MAX_DATA_SIZE);
    asio::error_code error;

    size_t len = socket.read_some(asio::buffer(buffer), error);
    if (error)
      break; // Handle error or disconnection

    AppMessage msg = AppMessage::deserialize(buffer);

    time_t timestamp;
    time(&timestamp);

    std::cout << ctime(&timestamp) << " + port: " << msg.port << " - msg: " << msg.data << std::endl;
    // Route to the correct app based on port
    switch (msg.port) {
    case 1:
      handle_app1(msg.data, socket);
      break;
    case 2:
      handle_app2(msg.data, socket);
      break;
    case 3:
      handle_app3(msg.data, socket);
      break;
    default:
      std::cerr << "Unknown port: " << msg.port << std::endl;
    }
  }

  return 0;
}
