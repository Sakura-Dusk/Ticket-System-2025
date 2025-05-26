//
// Created by Sakura on 25-5-26.
//

#ifndef TICKET_HPP
#define TICKET_HPP

#include "User.hpp"
#include "Train.hpp"

class Ticket {
    int operator_time;
    chars trainID;
    Time leaving_time, arriving_time;
    int number, price;
    int status;//0:pending 1:success -1:refunded

public:
    Ticket(){}
    Ticket(int operator_time, chars trainID, Time leaving_time, Time arriving_time, int number, int price, int status) : operator_time(operator_time), trainID(trainID), leaving_time(leaving_time), arriving_time(arriving_time), number(number), price(price), status(status) {}

    bool operator ==(const Ticket other) const {
        return operator_time != other.operator_time;
    }
    bool operator !=(const Ticket other) const {
        return !(*this == other);
    }
    bool operator <(const Ticket other) const {
        return operator_time < other.operator_time;
    }
    bool operator <=(const Ticket other) const {
        return (*this == other) || (*this < other);
    }
    bool operator >(const Ticket other) const {
        return !(*this <= other);
    }
    bool operator >=(const Ticket other) const {
        return !(*this < other);
    }
};

FileStore<std::pair<chars, int>, Ticket> Pending_Train_Ticket_List;//make_pair(trainID, number of train) -> Pending Ticket about this train
FileStore<chars, Ticket> User_Ticket_List;//userID -> this user's Ticket

void Ticket_Init() {
    Pending_Train_Ticket_List.Init("Pending_Train_Ticket_List");
    User_Ticket_List.Init("User_Ticket_List");
}

void Ticket_ALL_CLEAN() {
    Pending_Train_Ticket_List.clear_all();
    User_Ticket_List.clear_all();
}

void buy_ticket(int operator_time) {
    chars username, trainID; year_Time Date;
    int number;
    chars start_station, end_station;
    bool strategy = false;

    char c = getchar();
    while (c != '\n' && c != '-') c = getchar();
    while (c == '-') {
        c = getchar();
        if (c == 'u') std::cin >> username.a;
        if (c == 'i') std::cin >> trainID.a;
        if (c == 'd') {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.dd = (c - '0') * 10 + getchar() - '0';
        }
        if (c == 'n') std::cin >> number;
        if (c == 'f') std::cin >> start_station.a;
        if (c == 't') std::cin >> end_station.a;
        if (c == 'q') {
            chars tmp; std::cin >> tmp.a;
            if (tmp.a[0] == 't') strategy = true;
        }
        c = getchar();
        while (c != '\n' && c != '-') c = getchar();
    }

    vector<User>res0 = User_List.data_find(username);
    if (res0.empty()) {//user not found
        std::cout << "-1\n"; return ;
    }
    User now_user = res0[0];
    if (!User_Login.data_find(username)[0]) {//user not login
        std::cout << "-1\n"; return ;
    }

    vector<Train>res1 = Train_List.data_find(trainID);
    if (res1.empty()) {//train not found
        std::cout << "-1\n"; return ;
    }
    Train now_train = res1[0];
    if (now_train.is_release == false) {//train not release
        std::cout << "-1\n"; return ;
    }

    int start_pos = -1;
    for (int i = 0; i < now_train.stationNum; i++)
        if (start_station == now_train.stations[i]) {start_pos = i; break;}
    if (start_pos == -1) {//start_station not found
        std::cout << "-1\n"; return ;
    }
    int end_pos = -1;
    for (int i = start_pos + 1; i < now_train.stationNum; i++)
        if (end_station == now_train.stations[i]) {end_pos = i; break;}
    if (end_pos == -1) {//end_station not found
        std::cout << "-1\n"; return ;
    }

    // std::cerr << "start_pos = " << start_pos << " , end_pos = " << end_pos << std::endl;

    int pass_time = 0;
    for (int i = 0; i < start_pos; i++) {
        pass_time += now_train.travelTimes[i];
        if (i) pass_time += now_train.stopoverTimes[i];
    }
    if (start_pos) pass_time += now_train.stopoverTimes[start_pos];
    Time first_train_time(now_train.saleDate_start, now_train.startTime); first_train_time += pass_time;
    Time last_train_time(now_train.saleDate_end, now_train.startTime); last_train_time += pass_time;
    if (get_all_days(Date) < get_all_days(first_train_time.x) || get_all_days(Date) > get_all_days(last_train_time.x)) {
        std::cout <<"-1\n"; return ;//no this kind of train in this date
    }
    int number_of_train = get_days(first_train_time.x, Date);
    int have_minimum_ticket = 1000000000, price = 0;
    for (int i = start_pos; i < end_pos; i++) {
        have_minimum_ticket = std::min(have_minimum_ticket, now_train.ticket_left[number_of_train][i]);
        price += now_train.prices[i];
    }

    // std::cerr << "number_of_train = " << number_of_train << " , have_minimum_ticket = " << have_minimum_ticket << " , price = " << price << std::endl;

    Time leaving_time(Date, first_train_time.y);
    int travel_time = 0;
    for (int i = start_pos; i < end_pos; i++) {
        travel_time += now_train.travelTimes[i];
        if (i != start_pos) travel_time += now_train.stopoverTimes[i];
    }
    Time arriving_time(Date, first_train_time.y); arriving_time += travel_time;

    // leaving_time.write(); std::cout << std::endl;
    // arriving_time.write(); std::cout << std::endl;

    if (have_minimum_ticket >= number) {//successfully buy the ticket
        Train_List.data_delete(trainID, now_train);//update the train's ticket_left
        for (int i = start_pos; i < end_pos; i++) {
            now_train.ticket_left[number_of_train][i] -= number;
        }
        Train_List.data_insert(trainID, now_train);

        Ticket now_ticket(operator_time, trainID, leaving_time, arriving_time, number, price, 1);
        // std::cerr << "ready to insert ticket\n";
        User_Ticket_List.data_insert(username, now_ticket);

        std::cout << 1ll * price * number << std::endl;
        return ;
    }

    if (strategy == 0) {//no enough ticket now and don't want to wait
        std::cout << "-1\n"; return ;
    }

    //To to pending queue
    Ticket now_ticket(operator_time, trainID, leaving_time, arriving_time, number, price, 0);
    Pending_Train_Ticket_List.data_insert(std::make_pair(trainID, number_of_train), now_ticket);
    User_Ticket_List.data_insert(username, now_ticket);
    std::cout << "queue\n";
}

#endif //TICKET_HPP
