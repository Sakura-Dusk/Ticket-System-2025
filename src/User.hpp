//
// Created by Sakura on 25-5-24.
//

#ifndef USER_HPP
#define USER_HPP

#include "FileStore-B+.hpp"
#include "vector.hpp"
#include "Chars.hpp"

class User {//store data about User
public:
    int id;
    chars username, password, name, mailAddr;
    int privilege;

public:
    User() {
        privilege = 0; id = 0;
        username = chars(); password = chars(); name = chars(); mailAddr = chars();
    }
    User(const chars &username, const chars &password, const chars &name, const chars &mailAddr, const int &privilege) : id(0), username(username), password(password), name(name), mailAddr(mailAddr), privilege(privilege) {}
    [[nodiscard]] int Privilege() const {return privilege;}
    [[nodiscard]] chars Username() const {return username;}
    [[nodiscard]] chars Password() const {return password;}
    [[nodiscard]] chars Name() const {return name;}
    [[nodiscard]] chars MailAddr() const {return mailAddr;}

    bool operator ==(const User& other) const {
        return username == other.username;
    }
    bool operator <(const User& other) const {
        return username < other.username;
    }
};

inline MemoryRiver<User, 1> User_id;
inline FileStore<chars, int> User_List;
inline FileStore<chars, bool> User_Login;
inline MemoryRiver<chars, 1> User_root;
inline chars root_username;
inline bool exist_root;

inline void User_Init() {
    User_id.initialise("User_id");
    User_List.Init("User_List");
    User_Login.Init("User_Login");
    User_root.initialise("User_root");
    int op;
    User_root.get_info(op, 1);
    if (!op) {
        root_username = chars();
        exist_root = false;
    }
    else {
        User_root.read(root_username, op - 1);
        exist_root = true;
    }
}

inline void User_once_clean() {
    User_Login.clear_all();
}

inline void User_ALL_CLEAN() {
    User_id.clear_all();
    User_Login.clear_all();
    User_List.clear_all();
    User_root.clear_all();
    root_username = chars(); exist_root = false;
    User_Init();
}

inline int add_user() {
    chars cur_username, username, password, name, mailAddr;
    int privilege;
    chars read_op;
    for (int i = 1; i <= 6; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-c")) std::cin >> cur_username.a;
        if (read_op == Chars("-u")) std::cin >> username.a;
        if (read_op == Chars("-p")) std::cin >> password.a;
        if (read_op == Chars("-n")) std::cin >> name.a;
        if (read_op == Chars("-m")) std::cin >> mailAddr.a;
        if (read_op == Chars("-g")) std::cin >> privilege;
    }

    if (!exist_root) {
        privilege = 10;
        User root(username, password, name, mailAddr, privilege);
        int root_id = User_id.write(root);
        User_List.data_insert(username, root_id);
        // std::cerr << "qwq\n";
        root_username = username; exist_root = true;
        const int id = User_root.write(username) + 1;
        User_root.write_info(id, 1);
        User_Login.data_insert(username, false);//not login yet

        return 0;
    }

    vector<int>res = User_List.data_find(cur_username); if (res.empty()) return -1;//can't find cur_user
    User cur_user; User_id.read(cur_user, res[0]);
    const int cur_user_privilege = cur_user.Privilege();
    if (cur_user_privilege <= privilege) return -1;//privilege not ok
    if (!User_Login.data_find_bool(cur_username) || User_Login.data_find(cur_username)[0] == 0) return -1;//cur_user not login
    if (User_List.data_find_bool(username)) return -1;//new user already inside

    User now_user(username, password, name, mailAddr, privilege);
    int now_user_id = User_id.write(now_user);
    User_List.data_insert(username, now_user_id);
    User_Login.data_insert(username, false);//not login yet
    return 0;
}

inline int login() {
    chars username, password;
    chars read_op;
    for (int i = 1; i <= 2; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-u")) std::cin >> username.a;
        if (read_op == Chars("-p")) std::cin >> password.a;
    }

    vector<int>res = User_List.data_find(username); if (res.empty()) return -1;//can't find user
    if (User_Login.data_find_bool(username) && User_Login.data_find(username)[0]) return -1;//already login
    User cur_user; User_id.read(cur_user, res[0]);
    const chars real_password = cur_user.Password();
    if (real_password != password) return -1;//password wrong
    User_Login.data_delete(username, false);
    User_Login.data_insert(username, true);
    return 0;
}

inline int logout() {
    chars username;
    chars read_op;
    for (int i = 1; i <= 1; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-u")) std::cin >> username.a;
    }

    vector<int>res = User_List.data_find(username); if (res.empty()) return -1;//can't find user
    if (!User_Login.data_find_bool(username) || !User_Login.data_find(username)[0]) return -1;//not login
    User_Login.data_delete(username, true);
    User_Login.data_insert(username, false);
    return 0;
}

inline void query_profile(int operator_time) {
    chars cur_username, username;
    chars read_op;
    for (int i = 1; i <= 2; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-c")) std::cin >> cur_username.a;
        if (read_op == Chars("-u")) std::cin >> username.a;
    }

    vector<int>res = User_List.data_find(cur_username);
    if (res.empty()) {
        std::cout << "-1\n"; return ;
    }//can't find cur_user
    if (!User_Login.data_find_bool(cur_username) || !User_Login.data_find(cur_username)[0]) {
        std::cout << "-1\n"; return ;
    }//cur_user not login
    User cur_user; User_id.read(cur_user, res[0]);
    const int cur_privilege = cur_user.Privilege();
    res = User_List.data_find(username);
    if (res.empty()) {
        std::cout << "-1\n"; return ;
    }//can't find user
    User user; User_id.read(user, res[0]);
    const int privilege = user.Privilege();
    if (cur_privilege < privilege || (cur_privilege == privilege && cur_username != username)) {
        std::cout << "-1\n"; return ;
    }//cur_user privilege lower than user privilege
    std::cout << user.Username().a << " " << user.Name().a << " " << user.MailAddr().a << " " << user.Privilege() << std::endl;
}

inline void modify_profile() {
    chars cur_username, username, new_password, new_name, new_mailAddr;
    int new_privilege;
    bool in[6] = {false};
    char c = static_cast<char>(getchar());
    while (c != '\n' && c != '-') c = static_cast<char>(getchar());
    while (c == '-') {
        c = static_cast<char>(getchar());
        if (c == 'c') std::cin >> cur_username.a, in[0] = true;
        if (c == 'u') std::cin >> username.a, in[1] = true;
        if (c == 'p') std::cin >> new_password.a, in[2] = true;
        if (c == 'n') std::cin >> new_name.a, in[3] = true;
        if (c == 'm') std::cin >> new_mailAddr.a, in[4] = true;
        if (c == 'g') std::cin >> new_privilege, in[5] = true;
        c = static_cast<char>(getchar());
        while (c != '\n' && c != '-') c = static_cast<char>(getchar());
    }

    vector<int>res = User_List.data_find(cur_username);
    if (res.empty()) {
        // std::cout << "can't find cur_user\n";
        std::cout << "-1\n"; return ;
    }//can't find cur_user
    if (!User_Login.data_find_bool(cur_username) || !User_Login.data_find(cur_username)[0]) {
        // std::cout << "cur_user not login\n";
        std::cout << "-1\n"; return ;
    }//cur_user not login
    User cur_user; User_id.read(cur_user, res[0]);
    int cur_privilege = cur_user.Privilege();
    res = User_List.data_find(username);
    if (res.empty()) {
        // std::cout << "can't find user\n";
        std::cout << "-1\n"; return ;
    }//can't find user
    User user; User_id.read(user, res[0]);
    int privilege = user.Privilege();
    if (cur_privilege < privilege || (cur_privilege == privilege && cur_username != username)) {
        // std::cout << "cur_user privilege lower than user privilege\n";
        std::cout << "-1\n"; return ;
    }//cur_user privilege lower than user privilege

    if (in[5] && new_privilege >= cur_privilege) {
        // std::cout << "new_privilege >= cur_user privilege\n";
        std::cout << "-1\n"; return ;
    }//new_privilege >= cur_user privilege

    if (in[2]) {
        user.password = new_password;
    }
    if (in[3]) {
        user.name = new_name;
    }
    if (in[4]) {
        user.mailAddr = new_mailAddr;
    }
    if (in[5]) {
        user.privilege = new_privilege;
    }
    // User_List.data_insert(username, now);
    User_id.update(user, res[0]);

    std::cout << user.Username().a << " " << user.Name().a << " " << user.MailAddr().a << " " << user.Privilege() << std::endl;
}

#endif //USER_HPP
