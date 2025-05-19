#ifndef CHARS_HPP
#define CHARS_HPP

#include<string>
#include<cstring>
#include<iostream>

struct chars {
    public:
        char a[61];

        chars(){a[0] = 0;}

        chars(std::string &x) {
            int len = x.length();
            for (int i = 0; i < len; i++) a[i] = x[i];
            a[len] = 0;
        }

        char& operator[](int id) {
            return a[id];
        }

        bool operator ==(const chars other) const {
            if (strlen(a) != strlen(other.a)) return 0;
            for (int i = 0; i < strlen(a); i++)
                if (a[i] != other.a[i]) return 0;
                    else if (!a[i]) return 1;
            return 1;
        }
        bool operator !=(const chars other) const {
            return !(*this == other);
        }
        bool operator <(const chars other) const {
            if (*this == other) return 0;
            int k = std::min(strlen(a), strlen(other.a)) + 1;
            for (int i = 0; i < k; i++) if (a[i] != other.a[i]) return a[i] < other.a[i];
            return 0;
        }
        bool operator <=(const chars other) const {
            return (*this == other) || (*this < other);
        }
        bool operator >(const chars other) const {
            return !(*this <= other);
        }
        bool operator >=(const chars other) const {
            return !(*this < other);
        }
};

chars Chars(std::string x) {
    int len = x.length();
    chars res;
    for (int i = 0; i < len; i++) res[i] = x[i];
    for (int i = len; i < 61; i++) res[i] = 0;
    return res;
}

std::string String(chars x) {
    int len = strlen(x.a);
    std::string res; res.resize(len);
    for (int i = 0; i < len; i++) res[i] = x.a[i];
    return res;
}

std::ostream& operator<<(std::ostream& out, const chars& now) {
    out << now.a;
    return out;
}

#endif