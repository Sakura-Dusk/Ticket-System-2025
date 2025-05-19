#include "MemoryRiver.hpp"
#include<iostream>

using namespace std;

int main() {
    MemoryRiver <int, 1> T;
    T.initialise("qwq");
    T.write_info(5, 1);
    T.write_info(2, 2);
    int n;
    T.get_info(n, 1);
    
    int id[10];
    for (int i = 1; i <= n; i++) {
        int x = 2 * i;
        id[i] = T.write(x);
        cout << i << " " << id[i] << endl;
    }
    for (int i = 1; i <= n; i++) {
        int x;
        T.read(x, id[i]);
        cout << x << endl;
        x = -1;
        T.update(x, id[i]);
    }
    for (int i = 1; i <= n; i++) {
        int x;
        T.read(x, id[i]);
        cout << x << endl;
    }

    return 0;
}