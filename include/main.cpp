#include <iostream>
#include <string>
#include <sstream>
#include "in_memory_db.hpp"

int main() {
    InMemoryDatabase db(3); 
    std::string line;

    std::cout << "Commands:\n";
    std::cout << "      put <key> <value>\n";
    std::cout << "      put <key> <value> <ttl_ms>\n";
    std::cout << "      get <key>\n";
    std::cout << "      exit\n";

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "exit") break;
        else if (cmd == "put") {
            std::string key, value;
            if (iss >> key >> value) {
                int ttl;
                if (iss >> ttl) {
                    db.Put(key, value, std::chrono::milliseconds(ttl));
                    std::cout << "OK (TTL: " << ttl << "ms)\n";
                } 
                else {
                    db.Put(key, value);
                    std::cout << "OK (No TTL)\n";
                }
            } 
            else std::cout << "Error: put requires <key> and <value>\n";
        } 
        else if (cmd == "get") {
            std::string key;
            if (iss >> key) {
                auto res = db.Get(key);
                if (res.has_value()) std::cout << res.value() << "\n";
                else std::cout << "null\n";
            } 
            else std::cout << "Error: GET requires <key>\n";
        } 
        else std::cout << "Unknown command\n";
    }
    return 0;
}