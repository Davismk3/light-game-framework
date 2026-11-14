#pragma once

#include "vector.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

constexpr std::uint64_t DATA_MAX_ELEMENTS = 10'000'000;

/*Write Data:
/ -
T: a number, bool, or enum.
out: a stream opened in binary mode.
/ -
Writes the value's raw bytes. Throws if the write fails.*/
template<typename T>
inline void writeData(std::ostream& out, const T& value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>);

    out.write(reinterpret_cast<const char*>(&value), sizeof(T));

    if (!out) throw std::runtime_error("Failed to write data.");
}

/* Read Data:
/ -
T: a number, bool, or enum.
in: a stream opened in binary mode.
/ -
Reads the value's raw bytes. Throws if the read fails. */
template<typename T>
inline void readData(std::istream& in, T& value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>);

    in.read(reinterpret_cast<char*>(&value), sizeof(T));

    if (!in) throw std::runtime_error("Failed to read save data");
}

/* Write Data (std::vector<T>):
/ -
T: any type writeData accepts.
/ -
Writes the element count, then each element. */
template<typename T>
inline void writeData(std::ostream& out, const std::vector<T>& values) {
    const std::uint64_t count = static_cast<std::uint64_t>(values.size());

    writeData(out, count);

    for (const T& value : values) writeData(out, value);
}

/* Read Data (std::vector<T>):
/ -
T: any type readData accepts.
/ -
Reads the element count, then each element. Throws if the count is above DATA_MAX_ELEMENTS. */
template<typename T>
inline void readData(std::istream& in, std::vector<T>& values) {
    std::uint64_t count;
    readData(in, count);

    if (count > DATA_MAX_ELEMENTS) throw std::runtime_error("Invalid vector size");

    values.clear();
    values.reserve(static_cast<std::size_t>(count));

    for (std::uint64_t i = 0; i < count; ++i) {
        T value{};
        readData(in, value);
        values.push_back(std::move(value));
    }
}

/* Write Data (std::array<T, N>):
/ -
T: any type writeData accepts.
/ -
Writes each element. The size is not written, since N is fixed. */
template<typename T, std::size_t N>
inline void writeData(std::ostream& out, const std::array<T, N>& values) {
    for (const T& value : values) writeData(out, value);
}

/* Read Data (std::array<T, N>):
/ -
T: any type readData accepts.
/ -
Reads N elements. */
template<typename T, std::size_t N>
inline void readData(std::istream& in, std::array<T, N>& values) {
    for (T& value : values) readData(in, value);
}

/* Write Data (std::map<K, V>):
/ -
K, V: any types writeData accepts.
/ -
Writes the entry count, then each key and value. */
template<typename K, typename V>
inline void writeData(std::ostream& out, const std::map<K, V>& values) {
    const std::uint64_t count = static_cast<std::uint64_t>(values.size());

    writeData(out, count);

    for (const auto& [key, value] : values) {
        writeData(out, key);
        writeData(out, value);
    }
}

/* Read Data (std::map<K, V>):
/ -
K, V: any types readData accepts.
/ -
Reads the entry count, then each key and value. Throws if the count is above DATA_MAX_ELEMENTS. */
template<typename K, typename V>
void readData(std::istream& in, std::map<K, V>& values) {
    std::uint64_t count;
    readData(in, count);

    if (count > DATA_MAX_ELEMENTS) throw std::runtime_error("Invalid map size");

    values.clear();

    for (std::uint64_t i = 0; i < count; ++i) {
        K key{};
        V value{};

        readData(in, key);
        readData(in, value);

        values.emplace(std::move(key), std::move(value));
    }
}

/* Write Data (Vector3<T>):
/ -
T: a number type.
/ -
Writes x, y, then z. */
template<typename T>
inline void writeData(std::ostream& out, const Vector3<T>& value) {
    writeData(out, value.x);
    writeData(out, value.y);
    writeData(out, value.z);
}

/* Read Data (Vector3<T>):
/ -
T: a number type.
/ -
Reads x, y, then z. */
template<typename T>
inline void readData(std::istream& in, Vector3<T>& value) {
    readData(in, value.x);
    readData(in, value.y);
    readData(in, value.z);
}
