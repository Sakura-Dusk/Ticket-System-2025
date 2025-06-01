//
// Created by Sakura on 25-6-1.
//

#ifndef CHAR3_HPP
#define CHAR3_HPP

class Char3 {
    unsigned char a[3];

public:
    Char3(const int x = 0) {
        a[0] = static_cast<unsigned char>(x % 256);
        a[1] = static_cast<unsigned char>(x / 256 % 256);
        a[2] = static_cast<unsigned char>(x / 256 / 256);
    }

    const int to_int() const {
        return a[0] + a[1] * 256 + a[2] * 256 * 256;
    }

    Char3 operator+(const Char3& other) const {
        return Char3(to_int() + other.to_int());
    }

    Char3 operator-(const Char3& other) const {
        return Char3(to_int() - other.to_int());
    }

    Char3& operator+=(const Char3& other) {
        int x = to_int() + other.to_int();
        a[0] = static_cast<unsigned char>(x % 256);
        a[1] = static_cast<unsigned char>(x / 256 % 256);
        a[2] = static_cast<unsigned char>(x / 256 / 256);
        return *this;
    }

    Char3& operator-=(const Char3& other) {
        int x = to_int() - other.to_int();
        a[0] = static_cast<unsigned char>(x % 256);
        a[1] = static_cast<unsigned char>(x / 256 % 256);
        a[2] = static_cast<unsigned char>(x / 256 / 256);
        return *this;
    }

    Char3& operator+=(const int& other) {
        int x = to_int() + other;
        a[0] = static_cast<unsigned char>(x % 256);
        a[1] = static_cast<unsigned char>(x / 256 % 256);
        a[2] = static_cast<unsigned char>(x / 256 / 256);
        return *this;
    }

    Char3& operator-=(const int& other) {
        int x = to_int() - other;
        // std::cout << " x = " << x << std::endl;
        a[0] = static_cast<unsigned char>(x % 256);
        a[1] = static_cast<unsigned char>(x / 256 % 256);
        a[2] = static_cast<unsigned char>(x / 256 / 256);
        // std::cout << (int)a[0] << " " << (int)a[1] << " " << (int)a[2] << std::endl;
        return *this;
    }
};

#endif //CHAR3_HPP
