#include <gtest/gtest.h>
#include <thread>
#include "in_memory_db.hpp"

TEST(InMemoryDatabaseTest, Overwriting) {
    InMemoryDatabase db(2);
    
    db.Put("key1", "value1");
    db.Put("key1", "value2");

    auto result = db.Get("key1");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value() == "value2");
}

TEST(InMemoryDatabaseTest, Over_writing) {
    InMemoryDatabase db(2);
    
    db.Put("key1", "value1");
    db.Put("key2", "value2");
    
    db.Get("key1"); 
    db.Put("key3", "value3");

    EXPECT_FALSE(db.Get("key2").has_value());
    EXPECT_TRUE(db.Get("key1").has_value());
    EXPECT_TRUE(db.Get("key3").has_value());
}

TEST(InMemoryDatabaseTest, LazyTTLExpiration) {
    InMemoryDatabase db(2);
    
    db.Put("key1", "value1", std::chrono::milliseconds(50));
    
    EXPECT_TRUE(db.Get("key1").has_value());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    EXPECT_FALSE(db.Get("key1").has_value());
}

TEST(InMemoryDatabaseTest, ResetTTLOnUpdate) {
    InMemoryDatabase db(2);
    
    db.Put("key1", "value1", std::chrono::milliseconds(50));
    db.Put("key1", "value2");
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    auto result = db.Get("key1");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value() == "value2");
}
