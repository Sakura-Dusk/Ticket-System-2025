#include<fstream>
#include<iostream>
#include<string>
// #include "FileStore-B+.hpp"
#include "User.hpp"
#include "Train.hpp"
#include "Ticket.hpp"

void Init() {
    User_Init();
    Train_Init();
    Ticket_Init();
}

void once_clean() {
    User_once_clean();
}

void ALL_CLEAN() {
    User_ALL_CLEAN();
    Train_ALL_CLEAN();
    Ticket_ALL_CLEAN();
}

int get_operator_time(string &operator_id) {
    int len = operator_id.length();
    int res = 0, i = 0;
    while (i < len && (operator_id[i] < '0' || operator_id[i] > '9')) i++;
    while (i < len && operator_id[i] >= '0' && operator_id[i] <= '9') {
        res = res * 10 + operator_id[i] - '0';
        i++;
    }
    return res;
}

int main() {
    // freopen("3.in", "r", stdin);
    // freopen("write.txt", "w", stdout);
    Init();

    while (1) {
        string operator_id;
        std::cin >> operator_id;

        string operator_type;
        std::cin >> operator_type;

        std::cout << operator_id << " ";

        if (operator_type == "add_user") {
            std::cout << add_user() << "\n";
            continue;
        }
        if (operator_type == "login") {
            std::cout << login() << "\n";
            continue;
        }
        if (operator_type == "logout") {
            std::cout << logout() << "\n";
            continue;
        }
        if (operator_type == "query_profile") {
            query_profile();
            continue;
        }
        if (operator_type == "modify_profile") {
            modify_profile();
            continue;
        }

        if (operator_type == "add_train") {
            std::cout << add_train() << "\n";
            continue;
        }
        if (operator_type == "delete_train") {
            std::cout << delete_train() << "\n";
            continue;
        }
        if (operator_type == "release_train") {
            std::cout << release_train() << "\n";
            continue;
        }
        if (operator_type == "query_train") {
            query_train();
            continue;
        }

        if (operator_type == "query_ticket") {
            query_ticket(get_operator_time(operator_id));
            continue;
        }
        if (operator_type == "query_transfer") {
            query_transfer();
            continue;
        }

        if (operator_type == "buy_ticket") {
            buy_ticket(get_operator_time(operator_id));
            continue;
        }
        if (operator_type == "query_order") {
            query_order();
            continue;
        }
        if (operator_type == "refund_ticket") {
            std::cout << refund_ticket() << std::endl;
            continue;
        }

        if (operator_type == "clean") {
            ALL_CLEAN();
            std::cout << "0\n";
            continue;
        }
        if (operator_type == "exit") {
            std::cout << "bye\n";
            break;
        }
    }
}

 // int main() {
 //     int n; std::cin >> n;
 //     FileStore <chars, int> T;
 //     T.Init("FileStore");
 //
 //     // std::cerr << sizeof(T.qweqw);
 //
 //     chars index;
 //     for (int i = 1; i <= n; i++) {
 //         // std::cerr << "i = " <<i <<std::endl;
 //         std::string op; std::cin >> op;
 //         // std::cerr << op << std::endl;
 //         if (op == "insert") {
 //             int value; std::cin >> index.a >> value;
 //             T.data_insert(index, value);
 //             continue;
 //         }
 //         if (op == "delete") {
 //             int value; std::cin >> index.a >> value;
 //             T.data_delete(index, value);
 //             continue;
 //         }
 //         if (op == "find") {
 //             std::cin >> index.a;
 //             vector <int> ans = T.data_find(index);
 //             if (!ans.size()) std::cout << "null";
 //                 else {
 //                     for (int j = 0; j < ans.size(); j++) std::cout << ans[j] << " ";
 //                 }
 //             std::cout << std::endl;
 //         }
 //     }
 //    // std::cerr << "about ~ part start.\n";
 //     return 0;
 // }