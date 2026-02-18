#include "bakery.hpp"
#include <fstream>
#include <iostream>

void print_bakery(const Bakery& bakery) {
  std::cout << "Employees: " << std::endl;
  for (auto employee : bakery.employees) {
    std::cout << employee << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Items: " << std::endl;
  for (auto item : bakery.items) {
    std::cout << item.name << ", " << item.price << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Orders: " << std::endl;
  for (auto order : bakery.orders) {
    std::cout << order.employee << ": ";
    auto j = 0;
    for (auto item : order.items) {
      std::cout << item.second << " " << item.first;
      j++;
      if (size_t(j) < order.items.size())
        std::cout << ", ";
    }
    std::cout << std::endl;
  }
}

// You shouldn't need to edit this function (unless you want!)
Bakery text_deserializer(std::string file_path) {
  std::ifstream infile(file_path);
  std::string line;
  Bakery bakery;

  while (!infile.eof()) {
    // Employees section
    if (line.compare("@employees") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        bakery.employees.push_back(line);
        std::getline(infile, line);
      }
    }

    // Items section
    if (line.compare("@items") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        auto end = line.find(", ");
        Item item;
        item.name = line.substr(0, end);
        item.price = line.substr(end + 2);
        bakery.items.push_back(item);
        std::getline(infile, line);
      }
    }

    // Orders section
    if (line.compare("@orders") == 0) {
      std::getline(infile, line);
      auto e = bakery.employees;
      while (line.size() > 0) {
        Order order;
        auto end = line.find(": ");
        order.employee = line.substr(0, end);

        // Find all the orders
        auto rest = line.substr(end + 2);
        size_t pos = 0;
        std::string token;
        while ((pos = rest.find(", ")) != std::string::npos) {
          token = rest.substr(0, pos);
          end = token.find(" ");
          auto quantity = token.substr(0, end);
          auto item_name = token.substr(end + 1);
          order.items.push_back(std::make_pair(item_name, quantity));
          rest.erase(0, pos + 2);
        }
        end = rest.find(" ");
        auto quantity = rest.substr(0, end);
        auto item_name = rest.substr(end + 1);
        order.items.push_back(std::make_pair(item_name, quantity));
        bakery.orders.push_back(order);

        // no more lines to read
        if (infile.eof())
          break;

        std::getline(infile, line);
      }
    }

    std::getline(infile, line);
  }

  return bakery;
}

// Implement these 3 functions!
// Hint: use print_bakery to help think about the text serializer
void text_serializer(const Bakery& bakery, std::string file_path){
    // print_bakery(bakery);
    std::string Bake = "Employees: \n";
    for (auto employee : bakery.employees){
      Bake += employee + "\n";
    }
    Bake += "\nItems:\n";
    for (auto item : bakery.items){
      Bake += item.name + ", " + item.price + "\n";
    }
    Bake += "\nOrders:\n";
    for (auto order : bakery.orders){
      Bake += order.employee + ": ";
      for (auto item : order.items){
        Bake += item.second + " " + item.first + ", ";
      }
      Bake.erase(Bake.length()-2, 2);
      Bake += "\n";
    }
    //std::cout << (Bake);
}

void write_string(std::ostream& os, const std::string& s){
  size_t size = s.size();
  os.write(reinterpret_cast<const char*>(&size), sizeof(size));
  os.write(s.data(), size);
}
std::string read_string(std::istream& is){
  size_t size;
  is.read(reinterpret_cast<char*>(&size), sizeof(size));
  std::string s(size, '\0');
  is.read(&s[0], size);
  return s;
}

Bakery binary_deserializer(std::string file_path){
  std::ifstream in(file_path, std::ios::binary);
  Bakery bakery;

  size_t employee_size;
  in.read(reinterpret_cast<char*>(&employee_size), sizeof(employee_size));
  for (size_t i = 0; i < employee_size; i++){
    bakery.employees.push_back(read_string(in));
  }

  size_t item_size;
  in.read(reinterpret_cast<char*>(&item_size), sizeof(item_size));
  for (size_t i = 0; i < item_size; i++){
    bakery.items.push_back({read_string(in), read_string(in)});
  }

  size_t order_count;
  in.read(reinterpret_cast<char*>(&order_count), sizeof(order_count));
  for (size_t i = 0; i < order_count; ++i) {
    Order order;
    order.employee = read_string(in);
    size_t pair_count;
    in.read(reinterpret_cast<char*>(&pair_count), sizeof(pair_count));
    for (size_t j = 0; j < pair_count; ++j) {
      order.items.push_back({read_string(in), read_string(in)});
    }
    bakery.orders.push_back(order);
  }
  //print_bakery(bakery);
  return bakery;
}

void binary_serializer(const Bakery& bakery, std::string file_path){
  std::ofstream out(file_path, std::ios::binary);

  size_t employee_size = bakery.employees.size();
  out.write(reinterpret_cast<const char*>(&employee_size), sizeof(employee_size));
  for (const auto& employee : bakery.employees){
    write_string(out, employee);
  }

  size_t item_size = bakery.items.size();
  out.write(reinterpret_cast<const char*>(&item_size), sizeof(item_size));
  for (const auto& item : bakery.items){
    write_string(out, item.name);
    write_string(out, item.price);
  }

  size_t order_size = bakery.orders.size();
  out.write(reinterpret_cast<const char*>(&order_size), sizeof(order_size));
  for (const auto& order : bakery.orders){
    write_string(out, order.employee);

    size_t pair_size = order.items.size();
    out.write(reinterpret_cast<const char*>(&pair_size), sizeof(pair_size));
    for (const auto& pair : order.items){
      write_string(out, pair.first);
      write_string(out, pair.second);
    }
  }


}

