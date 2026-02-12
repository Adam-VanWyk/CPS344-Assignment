#include <array>
#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

const size_t MAX_STRING_SIZE = 256; // Maximum string message size

int main() {

  // Socket setup cod
  asio::io_context io_context;
  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve("127.0.0.1", "3000");

  tcp::socket socket(io_context);
  asio::connect(socket, endpoints);

  asio::error_code error;

  while (true){
    uint32_t port;
    std::string appMessage;

    std::cout << "Enter port # " << std::endl;
    std::cin >> port;

    std::cout << "Enter message " << std::endl;
    std::cin >> appMessage;

    std::string message = std::string(1, static_cast<char>(port)) + appMessage;

    asio::write(socket, asio::buffer(message), error);
    if (error) {
      std::cerr << "Error sending message: " << error.message() << std::endl;
      return 1;
    }
    std::array<char, MAX_STRING_SIZE> responseBuf;
    size_t len = socket.read_some(asio::buffer(responseBuf), error);
    if (error) {
      std::cerr << "Error reading response: " << error.message() << std::endl;
      return 1;
    }

    std::string response(responseBuf.data(), len);
    std::cout << "Server's response: " << response << std::endl;

  };

  // Hardcoded message for demonstration
  // Valid ports are 1, 2, or 3. Try each one to figure out what each "app" does
  // uint32_t port = 1;

  // Example message to send
  // std::string appMessage = "example";

  //
  // std::string message = std::string(1, static_cast<char>(port)) + appMessage;

  // Send the hardcoded message
  // asio::write(socket, asio::buffer(message), error);
  // if (error) {
  //   std::cerr << "Error sending message: " << error.message() << std::endl;
  //   return 1;
  // }

  // Read and print the server's response
  // std::array<char, MAX_STRING_SIZE> responseBuf;
  // size_t len = socket.read_some(asio::buffer(responseBuf), error);
  // if (error) {
  //   std::cerr << "Error reading response: " << error.message() << std::endl;
  //   return 1;
  // }

  // std::string response(responseBuf.data(), len);
  // std::cout << "Server's response: " << response << std::endl;

  return 0;
}
