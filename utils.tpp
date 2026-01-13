#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <ctime>

template <typename T>
std::vector<T> shuffle_vector(const std::vector<T>& v) {
    std::vector<T> tmp = v;
    std::vector<T> result;
    result.reserve(tmp.size());

    std::srand(std::time(0));
    while (!tmp.empty()) {
        int index = std::rand() % tmp.size();
        result.push_back(tmp[index]);
        tmp.erase(tmp.begin() + index);
    }

    return result;
}

template<typename T>
class Grid2D {
private:
    std::vector<T> m_data;
    int m_width;
    int m_height;
    T m_empty;

public:
    Grid2D(int width, int height, T empty)
        : m_width(width), m_height(height), m_empty(empty)
    {
        m_data.assign(width * height, empty);
    }

    bool check_coordinates(int x, int y) const {
        return x >= 0 && y >= 0 && x < m_width && y < m_height;
    }

    int width() const { return m_width; }
    int height() const { return m_height; }

    T data(int x, int y) const {
        if (check_coordinates(x, y))
            return m_data[y * m_width + x];
        return m_empty;
    }

    void set(int x, int y, T value) {
        if (check_coordinates(x, y))
            m_data[y * m_width + x] = value;
    }
};
    
