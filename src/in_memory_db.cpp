#include "in_memory_db.hpp"

InMemoryDatabase::InMemoryDatabase(size_t capacity) : capacity_(capacity) {}

void InMemoryDatabase::Put(const std::string& key, const std::string& value) {
    auto it = cache_map_.find(key);
    
    if (it != cache_map_.end()) {
        it->second->value = value;
        it->second->expiration_time = TimePoint::max();
        cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
    } 
    else {
        if (cache_map_.size() == capacity_) {
            cache_map_.erase(cache_list_.back().key);
            cache_list_.pop_back();
        }
        cache_list_.push_front({key, value, TimePoint::max()});
        cache_map_[key] = cache_list_.begin();
    }
}

void InMemoryDatabase::Put(const std::string& key, const std::string& value, std::chrono::milliseconds ttl) {
    auto expiration = std::chrono::steady_clock::now() + ttl;
    auto it = cache_map_.find(key);
    
    if (it != cache_map_.end()) {
        it->second->value = value;
        it->second->expiration_time = expiration;
        cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
    } else {
        if (cache_map_.size() == capacity_) {
            cache_map_.erase(cache_list_.back().key);
            cache_list_.pop_back();
        }
        cache_list_.push_front({key, value, expiration});
        cache_map_[key] = cache_list_.begin();
    }
}

std::optional<std::string> InMemoryDatabase::Get(const std::string& key) {
    auto it = cache_map_.find(key);
    
    if (it == cache_map_.end()) return std::nullopt;

    if (it->second->expiration_time <= std::chrono::steady_clock::now()) {
        cache_list_.erase(it->second);
        cache_map_.erase(it);
        return std::nullopt;
    }

    cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
    return it->second->value;
}
