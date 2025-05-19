#ifndef READ_HPP
#define READ_HPP

#include<string>

class READ {
    private:
        std::string a;
    
    public:
        void check() {
            while (!a.empty() && a[0] == ' ') a.erase(0, 1);
            int k = a.length();
            while (k && a[k - 1] == ' ') k--, a.erase(k, 1);
        }

        READ() {}
        READ(std::string a) :a(a) {
            check();
        }

        int length() {
            check();
            return a.length();
        }

        std::string get_string() {
            check();
            if (!a.length()) throw 0;
            std::string re = "";
            while (!a.empty() && a[0] != ' ') {
                re += a[0];
                a.erase(0, 1);
            }
            return re;
        }
};

int string_to_int(std::string &x) {
    long long res = 0; bool zf = 0;
    int k = x.length();
    for (int i = 0; i < k; i++) {
        if (x[i] < '0' || x[i] > '9') {
            if (x[i] == '-' && !i) {zf = 1; continue;}
            throw 0;
        }
        res = res * 10 + x[i] - '0';
        if (res > 2147483647) throw 0;
    }
    return zf ? -res : res;
}

double string_to_double(std::string &x) {
    double res = 0; bool zf = 0, turn = 0;
    int k = x.length(); int lsize = 0, rsize = 0; double di = 1;
    for (int i = 0; i < k; i++) {
        if (x[i] < '0' || x[i] > '9') {
            if (x[i] == '-' && !i) {zf = 1; continue;}
            if (x[i] == '.') {
                if (turn) throw 0;
                turn = 1;
                continue;
            }
            throw 0;
        }
        if (!turn) {
            lsize++;
            res = res * 10 + x[i] - '0';
        }
        else {
            rsize++;
            di = di * 0.1;
            res = res + di * (x[i] - '0');
        }
        if (res > 2147483647) throw 0;
    }
    if (!lsize) throw 0;//不能 .12
    if (turn && !rsize) throw 0;//不能 12.
    if (rsize > 2) throw 0;
    return zf ? -res : res;
}

#endif