// Copyright Ben Roberts 2024

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename KeyType, typename ValueType>

class Chaining {
private:
  struct Entry {
    KeyType key;
    ValueType value;
  };

  vector<vector<Entry>> table_;
  size_t num_elements_;
  float load_factor_;

  size_t getCapacity() const { return table_.size(); }

  size_t hash(const KeyType &key) const {
    return std::hash<KeyType>{}(key) % getCapacity();
  }

  void rehash(size_t newCapacity) {
    auto old_table = table_;
    table_ = vector<vector<Entry>>(newCapacity);
    num_elements_ = 0;

    for (const auto &vec : old_table) {
      for (const auto &entry : vec) {
        insert(entry.key, entry.value);
      }
    }
  }

public:
  explicit Chaining(size_t capacity = 8, float load_factor = 2.0f)
      : table_(capacity), num_elements_(0), load_factor_(load_factor) {}

  void insert(const KeyType &key, const ValueType &value) {
    if (static_cast<float>(num_elements_ + 1) / getCapacity() > load_factor_) {
      rehash(getCapacity() * 2);
    }
    size_t hash_index = hash(key);
    table_[hash_index].push_back({key, value});
    num_elements_++;
  }

  ValueType get(const KeyType &key) const {
    size_t hash_index = hash(key);
    for (const auto &entry : table_[hash_index]) {
      if (entry.key == key) {
        return entry.value;
      }
    }
    throw runtime_error("Element is not in the table");
  }

  size_t getSize() const { return num_elements_; }

  int getImportantNumber() const { return 69; }
};