#pragma once

#include <map>
#include <string>
#include <iostream>

template<typename Key, typename It> inline bool mapContains(std::map<Key, It> map, Key key) {
    auto it = map.find(key);

    if (it != map.end()) {
        return true;
    }
    return false;
}

// Map At: Returns map[key], or a default-initialized value if the key is missing. map: copied, since it is passed by value. error_message: printed if the key is missing.
template<typename Key, typename It> inline It mapAt(std::map<Key, It>& r_map, Key key, std::string error_message) {
    auto it = r_map.find(key);

    if (it != r_map.end()) {
        return it->second;
    }
    else {
        //It false_it; 
        std::cout << error_message + "\n";
        return it->second;
    } 
};
