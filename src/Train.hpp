//
// Created by Sakura on 25-5-25.
//

#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "Chars.hpp"
#include "Sort.hpp"

const int stationNum_Maximum_Size = 100;
const int Maximum_Day_Size = 95;

struct day_Time {
    int hh, mm;//hour, minute

    day_Time(int hh = 0, int mm = 0) : hh(hh), mm(mm) {}
};
int get_all_minutes(day_Time x) {
    return x.hh * 60 + x.mm;
}
day_Time minutes_to_day_Time(int x) {
    return day_Time(x / 60, x % 60);
}
int get_minutes(day_Time x, day_Time y) {
    return get_all_minutes(y) - get_all_minutes(x);
}
struct year_Time {
    int mm, dd;//month, day

    year_Time(int mm = 0, int dd = 0) : mm(mm), dd(dd){}
};
int get_all_days(year_Time x) {
    if (x.mm < 6) return -1000000000;
    if (x.mm > 9) return 1000000000;
    return x.dd + ((x.mm >= 7) ? 30 : 0) + ((x.mm >= 8) ? 31 : 0) + ((x.mm >= 9) ? 31 : 0);
}
year_Time days_to_year_Time(int x) {
    if (x <= 30) return year_Time(6, x);
    if (x <= 30 + 31) return year_Time(7, x - 30);
    if (x <= 30 + 31 + 31) return year_Time(8, x - 30 - 31);
    return year_Time(9, x - 30 - 31 - 31);
}
int get_days(year_Time x, year_Time y) {
    return get_all_days(y) - get_all_days(x);
}

struct Time {
    year_Time x;
    day_Time y;

    Time(year_Time x = year_Time(), day_Time y = day_Time()) : x(x), y(y) {}

    Time& operator +=(const int add_time) {
        // write(); putchar('\n');
        int add_minutes = get_all_minutes(y) + add_time;
        int add_days = get_all_days(x) + add_minutes / (24 * 60); add_minutes %= (24 * 60);
        // std::cerr << "add_minutes = " << add_minutes << " , add_days = " << add_days << std::endl;
        x = days_to_year_Time(add_days); y = minutes_to_day_Time(add_minutes);
        // write(); putchar('\n');
        return *this;
    }

    void write() {
        printf("%02d-%02d %02d:%02d", x.mm, x.dd, y.hh, y.mm);
    }
};

int Time_to_Minutes(Time x) {
    return get_all_days(x.x) * 24 * 60 + get_all_minutes(x.y);
}
int get_times(Time x, Time y) {
    return Time_to_Minutes(y) - Time_to_Minutes(x);
}


class Train {
public:
    chars trainID;
    int stationNum;
    chars stations[stationNum_Maximum_Size];
    int seatNum;
    int prices[stationNum_Maximum_Size];
    day_Time startTime;
    int travelTimes[stationNum_Maximum_Size];
    int stopoverTimes[stationNum_Maximum_Size];
    year_Time saleDate_start, saleDate_end;
    char type;

    int ticket_left[Maximum_Day_Size][stationNum_Maximum_Size];

public:
    bool is_release;

    Train(){}
    Train(chars trainID, int stationNum, chars* stations, int seatNum, int* prices, day_Time startTime, int* travelTimes, int* stopoverTimes, year_Time saleDate_start, year_Time saleDate_end, char type)
        : trainID(trainID), stationNum(stationNum), seatNum(seatNum), startTime(startTime), saleDate_start(saleDate_start), saleDate_end(saleDate_end), type(type), is_release(false) {
        for (int i = 0; i < stationNum; i++) this->stations[i] = stations[i];
        for (int i = 0; i < stationNum - 1; i++) this->prices[i] = prices[i];
        for (int i = 0; i < stationNum - 1; i++) this->travelTimes[i] = travelTimes[i];
        for (int i = 1; i < stationNum - 1; i++) this->stopoverTimes[i] = stopoverTimes[i];
    }

    bool operator ==(const Train other) const {
        return trainID == other.trainID;
    }
    bool operator <(const Train other) const {
        return trainID < other.trainID;
    }
};

struct Journey {
    chars trainID;
    int pos, pos_to;
    Time first_time, last_time;
    int need_time, need_money;

    Journey(){}
    Journey(Train &now_train, int pos, int pos_to) : pos(pos), pos_to(pos_to) {
        trainID = now_train.trainID;
        int pass_time = 0;
        for (int j = 0; j < now_train.stationNum; j++)
            if (j == pos) {
                if (j && j < now_train.stationNum - 1) pass_time += now_train.stopoverTimes[j];
                break;
            }
            else {
                pass_time += now_train.travelTimes[j];
                if (j && j < now_train.stationNum - 1) pass_time += now_train.stopoverTimes[j];
            }
        first_time = Time(now_train.saleDate_start, now_train.startTime); first_time += pass_time;
        last_time =  Time(now_train.saleDate_end, now_train.startTime); last_time += pass_time;
        need_time = 0; need_money = 0;
        for (int j = pos; j < now_train.stationNum; j++) {
            if (j == pos_to) {
                break;
            }
            else {
                need_time += now_train.travelTimes[j];
                need_money += now_train.prices[j];
                if (j != pos) need_time += now_train.stopoverTimes[j];
            }
        }
    }

    bool operator ==(const Journey other) const {
        return (trainID == other.trainID) && (pos == other.pos) && (pos_to == other.pos_to);
    }
    bool operator <(const Journey other) const {
        if (trainID != other.trainID) return trainID < other.trainID;
        if (pos != other.pos) return pos < other.pos;
        return pos_to < other.pos;
    }
};

inline MemoryRiver<Train, 1> Train_id;
inline MemoryRiver<Journey, 1> Journey_id;
FileStore<chars, int> Train_List;
FileStore<std::pair<chars, chars>, int> Place_to_Place_List;//pair<station,station> -> trainID
FileStore<chars, chars> Place_to_Train_List;//station -> trainID

void Train_Init() {
    Train_id.initialise("Train_id");
    Journey_id.initialise("Journey_id");
    Train_List.Init("Train_List");
    Place_to_Place_List.Init("Place_to_Place_List");
    Place_to_Train_List.Init("Place_to_Train_List");
}

void Train_ALL_CLEAN() {
    Train_id.clear_all();
    Journey_id.clear_all();
    Train_List.clear_all();
    Place_to_Place_List.clear_all();
    Place_to_Train_List.clear_all();
    Train_Init();
}

int add_train() {
    chars trainID;
    int stationNum;
    chars stations[stationNum_Maximum_Size];
    int seatNum;
    int prices[stationNum_Maximum_Size];
    day_Time startTime;
    int travelTimes[stationNum_Maximum_Size];
    int stopoverTimes[stationNum_Maximum_Size];
    year_Time saleDate_start, saleDate_end;
    char type;

    chars read_op;
    for (int i = 1; i <= 10; i++) {
        std::cin >> read_op.a;
        // std::cerr << i << " " << read_op.a << "\n";
        if (read_op == Chars("-i")) std::cin >> trainID.a;
        if (read_op == Chars("-n")) std::cin >> stationNum;
        if (read_op == Chars("-m")) std::cin >> seatNum;
        if (read_op == Chars("-s")) {
            // std::cerr << "qwq\n";
            std::string str;
            std::cin >> str;
            int len = str.length(), cnt = 0;
            for (int L = 0, R = 0; L < len; L = R + 1) {
                R = L;
                while (R < len && str[R] != '|') R++;
                // std::cerr << "qwq " << L << " " << R << "\n";
                for (int j = L; j < R; j++)
                    stations[cnt].a[j - L] = str[j];
                stations[cnt].a[R - L] = 0;
                cnt++;
            }
        }
        if (read_op == Chars("-p")) {
            // std::cerr << "qwq\n";
            std::string str;
            std::cin >> str;
            int len = str.length(), cnt = 0; prices[cnt] = 0;
            for (int L = 0, R = 0; L < len; L = R + 1) {
                R = L;
                while (R < len && str[R] != '|') R++;
                // std::cerr << "qwq " << L << " " << R << "\n";
                for (int j = L; j < R; j++)
                    prices[cnt] = prices[cnt] * 10 + str[j] - '0';
                // std::cerr << prices[cnt] << std::endl;
                cnt++; prices[cnt] = 0;
            }
        }
        if (read_op == Chars("-x")) {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            startTime.hh = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            startTime.mm = (c - '0') * 10 + getchar() - '0';
        }
        if (read_op == Chars("-t")) {
            std::string str;
            std::cin >> str;
            int len = str.length(), cnt = 0; travelTimes[cnt] = 0;
            for (int L = 0, R = 0; L < len; L = R + 1) {
                R = L;
                while (R < len && str[R] != '|') R++;
                // std::cerr << "qwq " << L << " " << R << "\n";
                for (int j = L; j < R; j++)
                    travelTimes[cnt] = travelTimes[cnt] * 10 + str[j] - '0';
                // std::cerr << travelTimes[cnt] << std::endl;
                cnt++; travelTimes[cnt] = 0;
            }
        }
        if (read_op == Chars("-o")) {
            std::string str;
            std::cin >> str;
            int len = str.length(), cnt = 1; stopoverTimes[cnt] = 0;
            if (len == 1 && str[0] == '_') continue;
            for (int L = 0, R = 0; L < len; L = R + 1) {
                R = L;
                while (R < len && str[R] != '|') R++;
                for (int j = L; j < R; j++)
                    stopoverTimes[cnt] = stopoverTimes[cnt] * 10 + str[j] - '0';
                cnt++; stopoverTimes[cnt] = 0;
            }
        }
        if (read_op == Chars("-d")) {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            saleDate_start.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            saleDate_start.dd = (c - '0') * 10 + getchar() - '0';

            c = getchar(); while (c < '0' || c > '9') c = getchar();
            saleDate_end.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            saleDate_end.dd = (c - '0') * 10 + getchar() - '0';
        }
        if (read_op == Chars("-y")) std::cin >> type;
    }

    // std::cerr << "add_train read done\n";

    if (Train_List.data_find_bool(trainID)) return -1;//trainID already exist
    Train now_train(trainID, stationNum, stations, seatNum, prices, startTime, travelTimes, stopoverTimes, saleDate_start, saleDate_end, type);
    int now_train_id = Train_id.write(now_train);
    Train_List.data_insert(trainID, now_train_id);
    // std::cerr << "Try build, stationNum = " << stationNum << std::endl;
    return 0;
}

int delete_train() {
    // std::cerr << "Try delete_train\n";
    chars trainID;
    chars read_op;
    for (int i = 1; i <= 1; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-i")) std::cin >> trainID.a;
    }
    vector<int> res = Train_List.data_find(trainID);
    if (res.empty()) return -1;//train not found
    Train now_train; Train_id.read(now_train, res[0]);
    if (now_train.is_release) return -1;//train already released
    Train_List.data_delete(trainID, res[0]);
    return 0;
}

int release_train() {
    chars trainID;
    chars read_op;
    for (int i = 1; i <= 1; i++) {
        std::cin >> read_op.a;
        if (read_op == Chars("-i")) std::cin >> trainID.a;
    }
    vector<int> res = Train_List.data_find(trainID);
    if (res.empty()) return -1;//train not found
    Train now_train; Train_id.read(now_train, res[0]);
    if (now_train.is_release) return -1;//train already released
    now_train.is_release = true;

    //part about query_ticket : Place_to_Place_List
    for (int i = 0; i < now_train.stationNum; i++)
        for (int j = i + 1; j < now_train.stationNum; j++) {
            Journey journey(now_train, i, j);
            int journey_id = Journey_id.write(journey);
            Place_to_Place_List.data_insert(std::make_pair(now_train.stations[i], now_train.stations[j]), journey_id);
        }
    //put on the ticket on sell
    int total_days = get_days(now_train.saleDate_start, now_train.saleDate_end) + 1;
    for (int i = 0; i < total_days; i++)
        for (int j = 0; j < now_train.stationNum; j++)
            now_train.ticket_left[i][j] = now_train.seatNum;
    //part about query_transfer : Place_to_Train_List
    for (int i = 0; i < now_train.stationNum; i++)
        Place_to_Train_List.data_insert(now_train.stations[i], trainID);

    Train_id.update(now_train, res[0]);
    return 0;
}

void query_train() {
    chars trainID;
    year_Time Date;

    chars read_op;
    for (int i = 1; i <= 2; i++) {
        std::cin >> read_op.a;
        // std::cerr << i << " " << read_op.a << "\n";
        if (read_op == Chars("-i")) std::cin >> trainID.a;
        if (read_op == Chars("-d")) {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.dd = (c - '0') * 10 + getchar() - '0';
        }
    }

    //TBD
    vector<int> res = Train_List.data_find(trainID);
    if (res.empty()) {
        std::cout << "-1\n";
        return ;
    }//train not found
    Train now_train; Train_id.read(now_train, res[0]);
    if (get_all_days(Date) < get_all_days(now_train.saleDate_start) || get_all_days(Date) > get_all_days(now_train.saleDate_end)) {
        std::cout << "-1\n";
        return ;
    }//out of saleDate range

    std::cout << trainID << " " << now_train.type << std::endl;
    Time now_time(Date, now_train.startTime);
    int day_number = get_days(now_train.saleDate_start, Date);
    int now_cost = 0;
    for (int i = 0; i < now_train.stationNum; i++) {
        std::cout << now_train.stations[i].a << " ";
        if (!i) {
            std::cout << "xx-xx xx:xx -> ";
        }
        else {
            now_time.write();
            std::cout << " -> ";
            // printf("%02d-%02d %02d:%02d -> ", now_time.x.mm, now_time.x.dd, now_time.y.hh, now_time.y.mm);
        }
        if (i && i < now_train.stationNum - 1) now_time += now_train.stopoverTimes[i];
        if (i == now_train.stationNum - 1) {
            std::cout << "xx-xx xx:xx ";
        }
        else {
            now_time.write(); putchar(' ');
            // printf("%02d-%02d %02d:%02d ", now_time.x.mm, now_time.x.dd, now_time.y.hh, now_time.y.mm);
        }
        std::cout << now_cost << " ";
        if (i == now_train.stationNum - 1) {
            std::cout << "x\n";
        }
        else {
            if (now_train.is_release) std::cout << now_train.ticket_left[day_number][i] << std::endl;
                else std::cout << now_train.seatNum << std::endl;
            now_cost += now_train.prices[i];
            now_time += now_train.travelTimes[i];
        }
    }
}

void query_ticket(int operator_time) {
    chars start_station, end_station;
    year_Time Date;
    bool strategy = 0;
    char c = getchar();
    while (c != '\n' && c != '-') c = getchar();
    while (c == '-') {
        c = getchar();
        if (c == 's') std::cin >> start_station.a;
        if (c == 't') std::cin >> end_station.a;
        if (c == 'd') {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.dd = (c - '0') * 10 + getchar() - '0';
        }
        if (c == 'p') {
            chars tmp; std::cin >> tmp.a;
            if (tmp.a[0] == 'c') strategy = 1;
        }
        c = getchar();
        while (c != '\n' && c != '-') c = getchar();
    }

    // std::cerr << "done read part\n";
    // return ;

    vector<std::tuple<Journey, int, int> >ans;
    vector<int>res = Place_to_Place_List.data_find(std::make_pair(start_station, end_station));
    for (int i = 0; i < res.size(); i++) {
        Journey now_Journey; Journey_id.read(now_Journey, res[i]);
        if (get_all_days(Date) < get_all_days(now_Journey.first_time.x) || get_all_days(Date) > get_all_days(now_Journey.last_time.x)) continue;
        ans.push_back(std::make_tuple(now_Journey, now_Journey.need_time, now_Journey.need_money));
    }

    // std::cerr << "find all answers\n";
    // return ;

    vector <int> id; for (int i = 0; i < ans.size(); i++) id.push_back(i);
    if (ans.size()) {
        // std::cerr << "this time strategy : " << strategy << std::endl;
        if (strategy == 0) {
            // std::cerr << "qwq\n";
            sort(id.begin(), id.end(), [&](int x, int y) {
                // std::cerr << "sort about id: " << x << " " << y << std::endl;
                int xtime = std::get<1>(ans[x]), ytime = std::get<1>(ans[y]);
                // std::cerr << "xtime = " << xtime << ", ytime = " << ytime << std::endl;
                if (xtime != ytime) return xtime < ytime;
                return std::get<0>(ans[x]).trainID < std::get<0>(ans[y]).trainID;
            });
        }
        else {
            sort(id.begin(), id.end(), [&](int x, int y) {
                // std::cerr << "sort about id: " << x << " " << y << std::endl;
                int xcost = std::get<2>(ans[x]), ycost = std::get<2>(ans[y]);
                // std::cerr << "xcost = " << xcost << ", ycost = " << ycost << std::endl;
                if (xcost != ycost) return xcost < ycost;
                return std::get<0>(ans[x]).trainID < std::get<0>(ans[y]).trainID;
            });
        }
    }

    // std::cerr << "finish sort\n";
    // for (int i = 0; i < ans.size(); i++) std::cerr << id[i] << " "; std::cerr << std::endl;

    std::cout << ans.size() << std::endl;
    for (int i = 0; i < ans.size(); i++) {
        Journey now_Journey = std::get<0>(ans[id[i]]);

        Train now_train; Train_id.read(now_train, Train_List.data_find(now_Journey.trainID)[0]);
        int day_id = get_days(now_Journey.first_time.x, Date);
        Time start_time = Time(Date, now_Journey.first_time.y);
        Time end_time = start_time; end_time += now_Journey.need_time;

        int seat_number = 1000000000;
        for (int j = now_Journey.pos; j < now_Journey.pos_to; j++) seat_number = std::min(seat_number, now_train.ticket_left[day_id][j]);

        std::cout << now_train.trainID.a << " " << start_station.a << " ";
        start_time.write();
        std::cout << " -> " << end_station.a << " ";
        end_time.write();
        std::cout << " " << now_Journey.need_money << " " << seat_number << std::endl;
    }
}

void query_transfer(int operator_time) {
    chars start_station, end_station;
    year_Time Date;
    bool strategy = 0;
    char c = getchar();
    while (c != '\n' && c != '-') c = getchar();
    while (c == '-') {
        c = getchar();
        if (c == 's') std::cin >> start_station.a;
        if (c == 't') std::cin >> end_station.a;
        if (c == 'd') {
            char c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.mm = (c - '0') * 10 + getchar() - '0';
            c = getchar(); while (c < '0' || c > '9') c = getchar();
            Date.dd = (c - '0') * 10 + getchar() - '0';
        }
        if (c == 'p') {
            chars tmp; std::cin >> tmp.a;
            if (tmp.a[0] == 'c') strategy = 1;
        }
        c = getchar();
        while (c != '\n' && c != '-') c = getchar();
    }

    // return ;

    //first find all train that start from start_station
    bool find_solution = 0; std::tuple<Train, Train, int, int, chars, Time, Time, Time, Time, int, int> ans;
    //first train, second train, total_time, total_cost, mid_station, first_train_time, first_train_arrive_time, second_train_time, second_train_arrive_time, first_train_day_id, second_train_day_id
    vector<chars>vec1 = Place_to_Train_List.data_find(start_station);
    for (int o = 0; o < vec1.size(); o++) {
        Train first_train; Train_id.read(first_train, Train_List.data_find(vec1[o])[0]);

        int pos_1 = -1;
        int pass_time_ = 0;
        for (int i = 0; i < first_train.stationNum; i++)
            if (first_train.stations[i] == start_station) {
                if (i && i < first_train.stationNum - 1) pass_time_ += first_train.stopoverTimes[i];
                pos_1 = i; break;
            }
            else {
                pass_time_ += first_train.travelTimes[i];
                if (i && i < first_train.stationNum - 1) pass_time_ += first_train.stopoverTimes[i];
            }

        Time first_time_(first_train.saleDate_start, first_train.startTime); first_time_ += pass_time_;
        Time last_time_(first_train.saleDate_end, first_train.startTime); last_time_ += pass_time_;
        if (get_all_days(Date) < get_all_days(first_time_.x) || get_all_days(Date) > get_all_days(last_time_.x)) continue;

        int first_train_day_id = get_days(first_time_.x, Date);
        Time first_train_time(Date, first_time_.y);//when the first_train start
        Time first_train_arrrive_time(Date, first_time_.y);//when the first_train arrive
        int first_train_money = 0;//how much the first_train cost

        for (int pos_1_to = pos_1 + 1; pos_1_to < first_train.stationNum; pos_1_to++) {//first_train : station[pos_1] -> station[pos_1_to]
            first_train_money += first_train.prices[pos_1_to - 1];
            first_train_arrrive_time += first_train.travelTimes[pos_1_to - 1];
            if (pos_1_to - 1 != pos_1) first_train_arrrive_time += first_train.stopoverTimes[pos_1_to - 1];

            //code from query_ticket
            chars mid_station = first_train.stations[pos_1_to];
            vector<int>res = Place_to_Place_List.data_find(std::make_pair(mid_station, end_station));
            for (int i = 0; i < res.size(); i++) {
                Journey now_Journey; Journey_id.read(now_Journey, res[i]);
                chars train_ID = now_Journey.trainID;
                Train now_train; Train_id.read(now_train, Train_List.data_find(train_ID)[0]);
                if (now_train.trainID == first_train.trainID) continue;

                Time first_time = now_Journey.first_time, last_time = now_Journey.last_time;
                // if (Time_to_Minutes(first_train_arrrive_time) > Time_to_Minutes(last_time)) continue;
                Time second_train_time(first_time.x, first_time.y);
                if (get_all_minutes(first_time.y) >= get_all_minutes(first_train_arrrive_time.y)) {
                    if (get_all_days(first_train_arrrive_time.x) > get_all_days(second_train_time.x)) second_train_time.x = first_train_arrrive_time.x;
                    // second_train_time.x = std::max(second_train_time.x, first_train_arrrive_time.x);
                }
                else {
                    if (get_all_days(first_train_arrrive_time.x) + 1 > get_all_days(second_train_time.x)) second_train_time.x = days_to_year_Time(get_all_days(first_train_arrrive_time.x) + 1);
                    // second_train_time.x = std::max(second_train_time.x, days_to_year_Time(get_all_days(first_train_arrrive_time.x) + 1));
                }
                if (Time_to_Minutes(second_train_time) > Time_to_Minutes(last_time)) continue;
                int second_train_day_id = get_days(first_time.x, second_train_time.x);
                int need_time = now_Journey.need_time, need_money = now_Journey.need_money;
                Time second_train_arrive_time(second_train_time.x, second_train_time.y); second_train_arrive_time += need_time;

                int total_money = first_train_money + need_money;
                int total_time = get_times(first_train_time, second_train_arrive_time);
                std::tuple<Train, Train, int, int, chars, Time, Time, Time, Time, int, int> maybe_ans(first_train, now_train, total_time, total_money, mid_station, first_train_time, first_train_arrrive_time, second_train_time, second_train_arrive_time, first_train_day_id, second_train_day_id);

                if (find_solution) {
                    if (strategy == 0) {
                        int timex = std::get<2>(ans), timey = std::get<2>(maybe_ans);
                        if (timex != timey) {
                            if (timex > timey) ans = maybe_ans;
                        }
                        else {
                            int pricex = std::get<3>(ans), pricey = std::get<3>(maybe_ans);
                            if (pricex != pricey) {
                                if (pricex > pricey) ans = maybe_ans;
                            }
                            else {
                                chars first_trainx = std::get<0>(ans).trainID, first_trainy = std::get<0>(maybe_ans).trainID;
                                if (first_trainx != first_trainy) {
                                    if (first_trainx > first_trainy) ans = maybe_ans;
                                }
                                else {
                                    if (std::get<1>(ans).trainID > std::get<1>(maybe_ans).trainID) ans = maybe_ans;
                                }
                            }
                        }
                    }
                    else {
                        int pricex = std::get<3>(ans), pricey = std::get<3>(maybe_ans);
                        if (pricex != pricey) {
                            if (pricex > pricey) ans = maybe_ans;
                        }
                        else {
                            int timex = std::get<2>(ans), timey = std::get<2>(maybe_ans);
                            if (timex != timey) {
                                if (timex > timey) ans = maybe_ans;
                            }
                            else {
                                chars first_trainx = std::get<0>(ans).trainID, first_trainy = std::get<0>(maybe_ans).trainID;
                                if (first_trainx != first_trainy) {
                                    if (first_trainx > first_trainy) ans = maybe_ans;
                                }
                                else {
                                    if (std::get<1>(ans).trainID > std::get<1>(maybe_ans).trainID) ans = maybe_ans;
                                }
                            }
                        }
                    }
                }
                else {
                    ans = maybe_ans;
                    find_solution = true;
                }
                // ans.push_back(std::make_tuple(now_train, need_time, need_money));
            }
        }
    }

    if (!find_solution) {
        std::cout << 0 << std::endl;
        return ;
    }

    Train first_train = std::get<0>(ans), second_train = std::get<1>(ans);
    chars mid_station = std::get<4>(ans);
    Time first_train_time = std::get<5>(ans), first_train_arrive_time = std::get<6>(ans);
    Time second_train_time = std::get<7>(ans), second_train_arrive_time = std::get<8>(ans);
    int first_train_day_id = std::get<9>(ans), second_train_day_id = std::get<10>(ans);

    int first_train_need_money = 0, first_train_seat_number = 1000000000;
    int second_train_need_money = 0, second_train_seat_number = 1000000000;
    int pos = -1;
    for (int i = 0; i < first_train.stationNum; i++) if (start_station == first_train.stations[i]) {pos = i; break;}
    for (int i = pos; i < first_train.stationNum; i++)
        if (first_train.stations[i] == mid_station) break;
        else {
            first_train_need_money += first_train.prices[i];
            first_train_seat_number = std::min(first_train_seat_number, first_train.ticket_left[first_train_day_id][i]);
        }
    pos = -1;
    for (int i = 0; i < second_train.stationNum; i++) if (mid_station == second_train.stations[i]) {pos = i; break;}
    for (int i = pos; i < second_train.stationNum; i++)
        if (second_train.stations[i] == end_station) break;
        else {
            second_train_need_money += second_train.prices[i];
            second_train_seat_number = std::min(second_train_seat_number, second_train.ticket_left[second_train_day_id][i]);
        }

    std::cout << first_train.trainID.a << " " << start_station.a << " ";
    first_train_time.write();
    std::cout << " -> " << mid_station.a << " ";
    first_train_arrive_time.write();
    std::cout << " " << first_train_need_money << " " << first_train_seat_number << std::endl;

    std::cout << second_train.trainID.a << " " << mid_station.a << " ";
    second_train_time.write();
    std::cout << " -> " << end_station.a << " ";
    second_train_arrive_time.write();
    std::cout << " " << second_train_need_money << " " << second_train_seat_number << std::endl;
}

#endif //TRAIN_HPP
