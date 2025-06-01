//
// Created by Sakura on 25-5-26.
//

#ifndef TICKET_HPP
#define TICKET_HPP

#include "User.hpp"
#include "Train.hpp"

class Ticket {
    int operator_time;
    chars<20> username;
    chars<20> trainID;
    chars<30> from_place, to_place;
    int number_of_train;
    Time leaving_time, arriving_time;
    int number, price;

public:
    int status;//0:pending 1:success -1:refunded

    Ticket(){}
    Ticket(int operator_time, chars<20> username, chars<20> trainID, chars<30> from_place, chars<30> to_place, int number_of_train, Time leaving_time, Time arriving_time, int number, int price, int status) : operator_time(operator_time), username(username), trainID(trainID), from_place(from_place), to_place(to_place), number_of_train(number_of_train), leaving_time(leaving_time), arriving_time(arriving_time), number(number), price(price), status(status) {}

    bool operator ==(const Ticket other) const {
        return (operator_time == other.operator_time) && (status == other.status);
    }
    bool operator !=(const Ticket other) const {
        return !(*this == other);
    }
    bool operator <(const Ticket other) const {
        if (operator_time != other.operator_time) return operator_time < other.operator_time;
        return status < other.status;
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

    void write() {
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

    std::pair<chars<20>, int> train_data_get() {
        return std::make_pair(trainID, number_of_train);
    }

    chars<20> username_get() {
        return username;
    }

    void give_back_seat() {
        int now_train_id = Train_List.data_find(trainID)[0];
        Train now_train; Train_id.read(now_train, now_train_id);
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
        Train_id.update(now_train, now_train_id);
    }

    int check_maximum_ticket() {
        int maximum_ticket = 1000000000;
        Train now_train; Train_id.read(now_train, Train_List.data_find(trainID)[0]);
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
        int now_train_id = Train_List.data_find(trainID)[0];
        Train now_train; Train_id.read(now_train, now_train_id);
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
        Train_id.update(now_train, now_train_id);
    }
};

MemoryRiver<Ticket, 1> Ticket_id;
FileStore<std::pair<chars<20>, int>, int> Pending_Train_Ticket_List;//make_pair(trainID, number of train) -> Pending Ticket about this train
FileStore<chars<20>, int> User_Ticket_List;//userID -> this user's Ticket

void Ticket_Init() {
    Ticket_id.initialise("Ticket_id");
    Pending_Train_Ticket_List.Init("Pending_Train_Ticket_List");
    User_Ticket_List.Init("User_Ticket_List");
}

void Ticket_ALL_CLEAN() {
    Ticket_id.clear_all();
    Pending_Train_Ticket_List.clear_all();
    User_Ticket_List.clear_all();
    Ticket_Init();
}

void buy_ticket(int operator_time) {
    chars<20> username, trainID; year_Time Date;
    int number;
    chars<30> start_station, end_station;
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

    vector<int>res0 = User_List.data_find(username);
    if (res0.empty()) {//user not found
        std::cout << "-1\n"; return ;
    }
    User now_user; User_id.read(now_user, res0[0]);
    if (!User_Login.data_find_bool(username) || !User_Login.data_find(username)[0]) {//user not login
        std::cout << "-1\n"; return ;
    }

    vector<int>res1 = Train_List.data_find(trainID);
    if (res1.empty()) {//train not found
        std::cout << "-1\n"; return ;
    }
    int now_train_id = res1[0];
    Train now_train; Train_id.read(now_train, now_train_id);
    if (now_train.is_release == false) {//train not release
        std::cout << "-1\n"; return ;
    }

    if (now_train.seatNum < number) {//no enough seat
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
        Train_id.update(now_train, now_train_id);

        Ticket now_ticket(operator_time, username, trainID, start_station, end_station, number_of_train, leaving_time, arriving_time, number, price, 1);
        // std::cerr << "ready to insert ticket\n";
        int now_ticket_id = Ticket_id.write(now_ticket);
        User_Ticket_List.data_insert(username, now_ticket_id);

        std::cout << 1ll * price * number << std::endl;
        return ;
    }

    if (strategy == 0) {//no enough ticket now and don't want to wait
        std::cout << "-1\n"; return ;
    }

    //Go to pending queue
    Ticket now_ticket(operator_time, username, trainID, start_station, end_station, number_of_train, leaving_time, arriving_time, number, price, 0);
    int now_ticket_id = Ticket_id.write(now_ticket);
    Pending_Train_Ticket_List.data_insert(std::make_pair(trainID, number_of_train), now_ticket_id);
    User_Ticket_List.data_insert(username, now_ticket_id);
    std::cout << "queue\n";
}

void query_order() {
    chars<20> username;
    chars read_op;
    for (int i = 1; i <= 1; i++) {
        std::cin >> read_op.a;
        // std::cerr << i << " " << read_op.a << "\n";
        if (read_op == Chars("-u")) std::cin >> username.a;
    }

    vector<int>vec0 = User_List.data_find(username);
    if (vec0.empty()) {//user not found
        std::cout << "-1\n"; return ;
    }
    User now_user; User_id.read(now_user, vec0[0]);
    if (!User_Login.data_find_bool(username) || User_Login.data_find(username)[0] == false) {//user not login
        std::cout << "-1\n"; return ;
    }

    vector<int>vec1 = User_Ticket_List.data_find(username);
    std::cout << vec1.size() << std::endl;
    for (int i = (int)vec1.size() - 1; i >= 0; i--) {
        Ticket now_ticket; Ticket_id.read(now_ticket, vec1[i]);
        now_ticket.write(); putchar('\n');
    }
}

int refund_ticket() {
    chars<20> username;
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

    vector<int>vec0 = User_List.data_find(username);
    if (vec0.empty()) return -1;//user not found
    User now_user; User_id.read(now_user, vec0[0]);
    if (!User_Login.data_find_bool(username) || User_Login.data_find(username)[0] == false) return -1;//user not login

    vector<int>vec1 = User_Ticket_List.data_find(username);
    if (vec1.size() < number) return -1;//no enough ticket
    int now_ticket_id = vec1[vec1.size() - number];
    Ticket now_ticket; Ticket_id.read(now_ticket, now_ticket_id);

    if (now_ticket.status == -1) return -1;//ticket already refunded
    if (now_ticket.status == 0) {//pending -> refund
        // User_Ticket_List.data_delete(username, now_ticket);
        Pending_Train_Ticket_List.data_delete(now_ticket.train_data_get(), now_ticket_id);
        now_ticket.status = -1;
        // User_Ticket_List.data_insert(username, now_ticket);
        Ticket_id.update(now_ticket, now_ticket_id);
        return 0;
    }
    if (now_ticket.status == 1) {
        // User_Ticket_List.data_delete(username, now_ticket);
        now_ticket.status = -1;
        // User_Ticket_List.data_insert(username, now_ticket);
        Ticket_id.update(now_ticket, now_ticket_id);

        now_ticket.give_back_seat();

        //check the pending queue
        std::pair<chars<20>, int> train_data = now_ticket.train_data_get();
        vector<int>pending_queue = Pending_Train_Ticket_List.data_find(train_data);
        for (int i = 0; i < pending_queue.size(); i++) {//every pending ticket
            int pending_ticket_id = pending_queue[i];
            Ticket pending_ticket; Ticket_id.read(pending_ticket, pending_ticket_id);
            // std::cerr << "pending_ticket = ";
            // pending_ticket.write(); putchar('\n');
            int maximum_ticket = pending_ticket.check_maximum_ticket();
            // std::cerr << "maximum_ticket = " << maximum_ticket << std::endl;
            if (pending_ticket.check_number(maximum_ticket)) {
                pending_ticket.turn_to_success();

                //update the status
                chars pending_ticket_username = pending_ticket.username_get();
                Pending_Train_Ticket_List.data_delete(train_data, pending_ticket_id);
                // User_Ticket_List.data_delete(pending_ticket_username, pending_ticket);
                pending_ticket.status = 1;
                // User_Ticket_List.data_insert(pending_ticket_username, pending_ticket);
                Ticket_id.update(pending_ticket, pending_ticket_id);
            }
        }

        return 0;
    }
}

#endif //TICKET_HPP
