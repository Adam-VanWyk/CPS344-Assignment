#include <chrono>
#include <iostream>
#include "bakery.hpp"
using namespace std::chrono;

int main() {
  auto start = high_resolution_clock::now();
  Bakery bakery = text_deserializer("../data/small.txt");
  auto deser_time = high_resolution_clock::now() - start;
  std::cout << "Text deserializer took: "
            << duration_cast<milliseconds>(deser_time).count() << "ms"
            << std::endl
            << std::endl;
  print_bakery(bakery);

  // TODO: Implement, in order (check the bakery.cpp file):
  // 1. Text serializer
  text_serializer(bakery, "../data/small.txt");
  // 2. Binary serializer
  binary_serializer(bakery, "../data/smallbinary.txt");
  // 3. Binary deserializer
  binary_deserializer("../data/smallbinary.txt");
  return 0;
}