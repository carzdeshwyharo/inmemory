#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <chrono>
#include <optional>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

struct Node {
    std::string key;
    std::string value;
    TimePoint expiration_time;
};

class InMemoryDatabase {
public:
    explicit InMemoryDatabase(size_t capacity);
    void Put(const std::string& key, const std::string& value);
    void Put(const std::string& key, const std::string& value, std::chrono::milliseconds ttl);
    std::optional<std::string> Get(const std::string& key);

private:
    size_t capacity_;
    std::list<Node> cache_list_;
    std::unordered_map<std::string, std::list<Node>::iterator> cache_map_;
};
