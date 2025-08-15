#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>

namespace lfu {

template <typename T, typename KeyT>
class lfu_cache_t {
 public:
    using frequency   = size_t;
    struct Node{
        T value_;
        frequency frequency_;
        using CacheListIt = typename std::list<std::pair<KeyT, Node>>::iterator;
        CacheListIt it_;
    };

    using CacheList   = typename std::list<std::pair<KeyT, Node>>;

    size_t capacity_;
    size_t hits_ = 0;
    frequency min_frequency_ = 1;

    std::unordered_map<frequency, CacheList> lists_;
    std::unordered_map<KeyT, Node> nodes_;

    bool full() {return nodes_.size() == capacity_;};
    size_t retHits() {return hits_;};

    lfu_cache_t(size_t capacity): capacity_(capacity){};

    /*TODO ret smth*/ void lookupUpdate(const KeyT& key, const T& value) {
        if (!nodes_.contains(key)) {
            if (full())
                deleteElem();

            Node new_node;
            new_node.value_ = value;
            new_node.frequency_ = 1;

            insertElem(key, new_node);
            min_frequency_ = 1;
        }
        else {
            ++hits_;
            updateElem(key);

            for (; min_frequency_ < capacity_; ++min_frequency_) {
                if (lists_.contains(min_frequency_))
                    break;
            }
        }
    }

 private:
    /*TODO NOT VOID*/ void addNewFrequency(const frequency new_frequency) {
        CacheList new_list;
        auto [it, isInserted] = lists_.insert({new_frequency, new_list});
    }

    void insertElem(const KeyT& key, Node& new_node) {
        addNewFrequency(new_node.frequency_);
        CacheList& new_list = lists_.at(new_node.frequency_);
        new_list.push_back({key, new_node});
        new_node.it_ = new_list.begin();
        nodes_.insert({key, new_node});
    }

    void deleteElem() {
        CacheList& least_frequently_list = lists_.at(min_frequency_);
        std::pair<KeyT, Node> least_frequently_pair = least_frequently_list.front();

        least_frequently_list.pop_front();
        nodes_.erase(least_frequently_pair.first);
    }

    void updateElem(const KeyT& key) {
        Node& changing_node  = nodes_.at(key);
        CacheList& prev_list  = lists_.at(changing_node.frequency_);
        prev_list.erase(changing_node.it_);

        ++(changing_node.frequency_);

        //TODO maybe we need a new list with more frequency, check it
        if (!lists_.contains(changing_node.frequency_))
            addNewFrequency(changing_node.frequency_);

        CacheList& new_list = lists_.at(changing_node.frequency_);
        new_list.push_back({key, changing_node});
        changing_node.it_ = std::prev(new_list.end());
    }
};

} // namespace lfu
