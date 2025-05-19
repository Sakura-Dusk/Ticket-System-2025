#include<fstream>
#include<iostream>
#include<string>
#include "FileStore-B+.hpp"

// int main() {
//     std::ofstream inp;
//     inp.open("read.txt", std::ios::out);
//     if (!inp.is_open()) {
//         std::ofstream inp;
//         inp.open("read.txt", std::ios::out);
//     }
//     int n;
//     std::cin >> n;
//     inp << n;
//     std::string line;
//     while (getline(std::cin, line)) {
//         inp << line << std::endl;
//     }
//     inp.close();
// }

 int main() {
     int n; std::cin >> n;
     FileStore <chars, int> T;
     T.Init("FileStore");

     // std::cerr << sizeof(T.qweqw);

     chars index;
     for (int i = 1; i <= n; i++) {
         // std::cerr << "i = " <<i <<std::endl;
         std::string op; std::cin >> op;
         // std::cerr << op << std::endl;
         if (op == "insert") {
             int value; std::cin >> index.a >> value;
             T.data_insert(index, value);
             continue;
         }
         if (op == "delete") {
             int value; std::cin >> index.a >> value;
             T.data_delete(index, value);
             continue;
         }
         if (op == "find") {
             std::cin >> index.a;
             vector <int> ans = T.data_find(index);
             if (!ans.size()) std::cout << "null";
                 else {
                     for (int j = 0; j < ans.size(); j++) std::cout << ans[j] << " ";
                 }
             std::cout << std::endl;
         }
     }
    // std::cerr << "about ~ part start.\n";
     return 0;
 }