// Copyright Ben Roberts 2024

#define DEFAULT_CAPACITY 16

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename KeyType, typename ValueType> class OpenAddressTable {
private:
  struct Entry {
    KeyType key;
    ValueType value;
    bool empty = true;
  };

  vector<Entry> table_;
  size_t num_elements_;
  float load_factor_;

  size_t getCapacity() const { return table_.size(); }

  size_t standardHash(const KeyType &key) const {
    return std::hash<KeyType>{}(key) % table_.size();
  }

  void rehash(size_t newCapacity) {
    auto old_table = table_;
    table_ = vector<Entry>(newCapacity);
    num_elements_ = 0;

    for (const auto &entry : old_table) {
      if (!entry.empty) {
        insert(entry.key, entry.value);
      }
    }
  }

public:
  explicit OpenAddressTable(size_t capacity = 8, float load_factor = 0.5)
      : table_(capacity), num_elements_(0), load_factor_(load_factor) {}

  void insert(const KeyType &key, const ValueType &value) {
    if (static_cast<float>(num_elements_ + 1) / getCapacity() > load_factor_) {
      rehash(getCapacity() * 2);
    }

    size_t index = standardHash(key);
    size_t probe = index;

    while (!table_[probe].empty) {
      if (table_[probe].key == key) {
        // Update existing key
        table_[probe].value = value;
        return;
      }
      probe = (probe + 1) % getCapacity();
    }

    table_[probe] = {key, value, false};
    num_elements_++;
  }

  ValueType get(const KeyType &key) {
    size_t index = standardHash(key);
    size_t probe = index;

    while (!table_[probe].empty) {
      if (table_[probe].key == key) {
        return table_[probe].value;
      }
      probe = (probe + 1) % getCapacity();
      if (probe == index) {
        break; // Searched the entire table
      }
    }

    throw runtime_error("Element is not in the table");
  }

  size_t getSize() const { return num_elements_; }
};