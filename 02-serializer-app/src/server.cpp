#include <inttypes.h>
#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

#include "bakery.hpp"

using asio::ip::tcp;

uint32_t unpack_request(const std::array<uint8_t, 3>& buf){
  return (buf[0] << 16) | (buf[1] << 8) | buf[2];
}

void send_response(tcp::socket& socket, uint16_t value){
  std::array<uint8_t, 2> buf{
    static_cast<uint8_t>((value >> 8) & 0xFF), 
    static_cast<uint8_t>(value & 0xFF)
  };
  asio::write(socket, asio::buffer(buf));
}


std::string employee_decode(uint8_t E){
  if (E == 0b00){
    return "Brad";
  }
  if (E == 0b01){
    return "Claudia";
  }
  if (E == 0b10){
    return "Simone";
  }
  return ""; // incvalid case
}

int main() {
  asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));

  // Use this bakery to handle queries from the client
  Bakery bakery = text_deserializer("../data/bakery.txt");

  uint16_t counter = 0;

  while (true) {
    // Wait for client
    // std::cout << "Blocked for read" << std::endl;
    tcp::socket socket(io_context);
    acceptor.accept(socket);

    std::array<uint8_t, 3> buf;
    asio::read(socket, asio::buffer(buf));

    uint32_t msg = unpack_request(buf);

    uint8_t M  = (msg >> 22) & 0x3;
    uint8_t E  = (msg >> 20) & 0x3;
    uint8_t Q1 = (msg >> 16) & 0xF;
    uint8_t Q2 = (msg >> 12) & 0xF;
    uint8_t Q3 = (msg >> 8)  & 0xF;
    uint8_t Q4 = (msg >> 4)  & 0xF;
    uint8_t Q5 = msg & 0xF;

    uint16_t response = 0;

    if (M == 0b00){
      response = bakery.orders.size();
    }
    else if (M==0b01){
      std::string emp = employee_decode(E);
      for (const auto& order : bakery.orders){
        if (order.employee == emp){
          response++;
        }
      }
    }
    else if (M == 0b10){
      std::string emp = employee_decode(E);
      if (!emp.empty()){
        Order order;
        order.employee = emp;

        if (Q1) order.items.push_back({"Biscuit", std::to_string(Q1)});
        if (Q2) order.items.push_back({"Bun", std::to_string(Q2)});
        if (Q3) order.items.push_back({"Brownie", std::to_string(Q3)});
        if (Q4) order.items.push_back({"White Loaf", std::to_string(Q4)});
        if (Q5) order.items.push_back({"Wheat Loaf", std::to_string(Q5)});

        bakery.orders.push_back(order);
      }
      response = 0;
    }
    send_response(socket, response);
    // asio::error_code error;
    // size_t len = socket.read_some(asio::buffer(buf), error);

    // Add x to counter (just an example, you should implement the protocol here!)
    // auto x = uint8_t(buf[0]);
    // counter += x;
    // std::cout << +x << " " << counter << std::endl;

    // buf.fill(0); // Clear buffer

  //   std::memcpy(&buf, &counter, sizeof(uint16_t)); // Copy counter to buffer

  //   asio::write(socket, asio::buffer(buf), error); // Send buffer back to client

  }

  return 0;
}