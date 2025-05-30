//
// Created by Sakura on 25-5-26.
//

#ifndef TICKET_HPP
#define TICKET_HPP

#include "User.hpp"
#include "Train.hpp"

class Ticket {
    int operator_time;
    chars username;
    chars trainID, from_place, to_place;
    int number_of_train;
    Time leaving_time, arriving_time;
    int number, price;

public:
    // int status;//0:pending 1:success -1:refunded

    Ticket(){}
    Ticket(int operator_time, chars username, chars trainID, chars from_place, chars to_place, int number_of_train, Time leaving_time, Time arriving_time, int number, int price) : operator_time(operator_time), username(username), trainID(trainID), from_place(from_place), to_place(to_place), number_of_train(number_of_train), leaving_time(leaving_time), arriving_time(arriving_time), number(number), price(price) {}

    bool operator ==(const Ticket other) const {
        return (operator_time == other.operator_time);
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

    void write(int status) {
        putchar('[');
        if (status == 0) std::cout << "pending";
        if (status == -1) std::cout << "refunded";
        if (status == 1) std::cout << "success";
        putchar(']');

        std::cout << " " << trainID.a << " " << from_place.a << " ";
        leaving_time.write();
        std::cout << " -> " << to_place.a << " ";
        arriving_time.write();
        std::cout << " " << price << " " << number;
    }

    std::pair<chars, int> train_data_get() {
        return std::make_pair(trainID, number_of_train);
    }

    chars username_get() {
        return username;
    }

    void give_back_seat() {
        Train now_train = Train_List.data_find(trainID)[0];
        int pos = -1;
        for (int i = 0; i < now_train.stationNum; i++)
            if (now_train.stations[i] == from_place) {pos = i; break;}
        int pos_to = -1;
        for (int i = pos + 1; i < now_train.stationNum; i++)
            if (now_train.stations[i] == to_place) {pos_to = i; break;}

        //give back the seats
        // Train_List.data_delete(trainID, now_train);
        for (int i = pos; i < pos_to; i++) now_train.ticket_left[number_of_train][i] += number;
        // Train_List.data_insert(trainID, now_train);
        Train_List.data_update(trainID, now_train);
    }

    int check_maximum_ticket() {
        int maximum_ticket = 1000000000;
        Train now_train = Train_List.data_find(trainID)[0];
        int pos = -1;
        for (int i = 0; i < now_train.stationNum; i++)
            if (now_train.stations[i] == from_place) {pos = i; break;}
        int pos_to = -1;
        for (int i = pos + 1; i < now_train.stationNum; i++)
            if (now_train.stations[i] == to_place) {pos_to = i; break;}

        for (int i = pos; i < pos_to; i++) maximum_ticket = std::min(maximum_ticket, now_train.ticket_left[number_of_train][i]);
        return maximum_ticket;
    }

    bool check_number(int num) {
        return num >= number;
    }

    void turn_to_success() {
        Train now_train = Train_List.data_find(trainID)[0];
        int pos = -1;
        for (int i = 0; i < now_train.stationNum; i++)
            if (now_train.stations[i] == from_place) {pos = i; break;}
        int pos_to = -1;
        for (int i = pos + 1; i < now_train.stationNum; i++)
            if (now_train.stations[i] == to_place) {pos_to = i; break;}

        //take the tickets
        // Train_List.data_delete(trainID, now_train);
        for (int i = pos; i < pos_to; i++) now_train.ticket_left[number_of_train][i] -= number;
        // Train_List.data_insert(trainID, now_train);
        Train_List.data_update(trainID, now_train);
    }
};

FileStore<std::pair<chars, int>, Ticket> Pending_Train_Ticket_List;//make_pair(trainID, number of train) -> Pending Ticket about this train
FileStore<chars, Ticket> User_Ticket_List;//userID -> this user's Ticket
FileStore<Ticket, int> Ticket_status;

void Ticket_Init() {
    Pending_Train_Ticket_List.Init("Pending_Train_Ticket_List");
    User_Ticket_List.Init("User_Ticket_List");
    Ticket_status.Init("Ticket_status");
}

void Ticket_ALL_CLEAN() {
    Pending_Train_Ticket_List.clear_all();
    User_Ticket_List.clear_all();
    Ticket_status.clear_all();
    Ticket_Init();
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
    if (!User_Login.data_find_bool(username) || !User_Login.data_find(username)[0]) {//user not login
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
        std::cout <<"-1\n"; return ;//can't find this kind of train in this date
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
        // Train_List.data_delete(trainID, now_train);//update the train's ticket_left
        for (int i = start_pos; i < end_pos; i++) {
            now_train.ticket_left[number_of_train][i] -= number;
        }
        // Train_List.data_insert(trainID, now_train);
        Train_List.data_update(trainID, now_train);

        Ticket now_ticket(operator_time, username, trainID, start_station, end_station, number_of_train, leaving_time, arriving_time, number, price);
        // std::cerr << "ready to insert ticket\n";
        User_Ticket_List.data_insert(username, now_ticket);
        Ticket_status.data_insert(now_ticket, 1);

        std::cout << 1ll * price * number << std::endl;
        return ;
    }

    if (strategy == 0) {//no enough ticket now and don't want to wait
        std::cout << "-1\n"; return ;
    }

    //Go to pending queue
    Ticket now_ticket(operator_time, username, trainID, start_station, end_station, number_of_train, leaving_time, arriving_time, number, price);
    Pending_Train_Ticket_List.data_insert(std::make_pair(trainID, number_of_train), now_ticket);
    User_Ticket_List.data_insert(username, now_ticket);
    Ticket_status.data_insert(now_ticket, 0);
    std::cout << "queue\n";
}

void query_order() {
    chars username;
    chars read_op;
    for (int i = 1; i <= 1; i++) {
        std::cin >> read_op.a;
        // std::cerr << i << " " << read_op.a << "\n";
        if (read_op == Chars("-u")) std::cin >> username.a;
    }

    vector<User>vec0 = User_List.data_find(username);
    if (vec0.empty()) {//user not found
        std::cout << "-1\n"; return ;
    }
    User now_user = vec0[0];
    if (!User_Login.data_find_bool(username) || User_Login.data_find(username)[0] == false) {//user not login
        std::cout << "-1\n"; return ;
    }

    vector<Ticket>vec1 = User_Ticket_List.data_find(username);
    std::cout << vec1.size() << std::endl;
    for (int i = (int)vec1.size() - 1; i >= 0; i--) {
        Ticket now_ticket = vec1[i];
        now_ticket.write(Ticket_status.data_find(now_ticket)[0]); putchar('\n');
    }
}

int refund_ticket(int operator_time) {
    chars username;
    int number = 1;

    char c = getchar();
    while (c != '\n' && c != '-') c = getchar();
    while (c == '-') {
        c = getchar();
        if (c == 'u') std::cin >> username.a;
        if (c == 'n') std::cin >> number;
        c = getchar();
        while (c != '\n' && c != '-') c = getchar();
    }

    vector<User>vec0 = User_List.data_find(username);
    if (vec0.empty()) return -1;//user not found
    User now_user = vec0[0];
    if (!User_Login.data_find_bool(username) || User_Login.data_find(username)[0] == false) return -1;//user not login

    vector<Ticket>vec1 = User_Ticket_List.data_find(username);
    vector <int> id; for (int i = 0; i < vec1.size(); i++) id.push_back(i);
    SJTU::sort(id.begin(), id.end(), [&](int x, int y) {
        return vec1[x] < vec1[y];
    });
    // SJTU::sort(vec1.begin(), vec1.end(), std::less<Ticket>());
    if (vec1.size() < number) return -1;//no enough ticket
    Ticket now_ticket = vec1[id[vec1.size() - number]];

    // if (operator_time == 12802) {
    //     now_ticket.write();
    //     std::cout << "\n";
    // }

    int now_ticket_status = Ticket_status.data_find(now_ticket)[0];
    if (now_ticket_status == -1) return -1;//ticket already refunded
    if (now_ticket_status == 0) {//pending -> refund
        // User_Ticket_List.data_delete(username, now_ticket);
        // Pending_Train_Ticket_List.data_delete(now_ticket.train_data_get(), now_ticket);
        // User_Ticket_List.data_update_special(username, now_ticket, -1);
        now_ticket_status = -1;
        // User_Ticket_List.data_insert(username, now_ticket);
        Ticket_status.data_update(now_ticket, now_ticket_status);
        return 0;
    }
    if (now_ticket_status == 1) {
        // User_Ticket_List.data_delete(username, now_ticket);
        // User_Ticket_List.data_update_special(username, now_ticket, -1);
        now_ticket_status = -1;
        // User_Ticket_List.data_insert(username, now_ticket);
        Ticket_status.data_update(now_ticket, now_ticket_status);

        now_ticket.give_back_seat();

        //check the pending queue
        std::pair<chars, int> train_data = now_ticket.train_data_get();
        vector<Ticket>pending_queue = Pending_Train_Ticket_List.data_find(train_data);
        for (int i = 0; i < pending_queue.size(); i++) {//every pending ticket
            Ticket pending_ticket = pending_queue[i];
            int pending_ticket_status = Ticket_status.data_find(pending_ticket)[0];
            if (pending_ticket_status != 0) continue;
            // std::cerr << "pending_ticket = ";
            // pending_ticket.write(); putchar('\n');
            int maximum_ticket = pending_ticket.check_maximum_ticket();
            // std::cerr << "maximum_ticket = " << maximum_ticket << std::endl;
            if (pending_ticket.check_number(maximum_ticket)) {
                pending_ticket.turn_to_success();

                //update the status
                chars pending_ticket_username = pending_ticket.username_get();
                // Pending_Train_Ticket_List.data_delete(train_data, pending_ticket);
                // User_Ticket_List.data_delete(pending_ticket_username, pending_ticket);
                // User_Ticket_List.data_update_special(pending_ticket_username, pending_ticket, 1);
                pending_ticket_status = 1;
                // User_Ticket_List.data_insert(pending_ticket_username, pending_ticket);
                Ticket_status.data_update(pending_ticket, pending_ticket_status);
            }
        }

        return 0;
    }
}

#endif //TICKET_HPP
