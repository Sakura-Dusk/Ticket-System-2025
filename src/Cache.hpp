//
// Created by Sakura on 25-5-17.
//

#ifndef CACHE_H
#define CACHE_H

#include "Hash.hpp"

template <typename datatype, int MAX_LEN = 128>
class CACHE {
    public:
        struct Data {
            datatype data;
            int id; bool is_change;
            Data(const datatype& data, const int& id) : data(data), id(id), is_change(false) {}
            void update_Data(const datatype& new_data) {
                data = new_data; is_change = 1;
            }
        };

        struct chain {
            chain *pre, *nxt;
            Data data;
            chain(Data data) : data(data), pre(nullptr), nxt(nullptr) {}
        };

    private:
        int Size;
        chain *head, *tail;
        Hash<chain> mp;

    public:
        CACHE() : Size(0), head(nullptr), tail(nullptr) {}
        ~CACHE() {
            // std::cerr << "~CACHE start\n";
            while (head != nullptr) {
                chain *to = head->nxt;
                delete head;
                head = to;
            }
            // std::cerr << "~CACHE done\n";
        }

        chain* end() {return tail;}
        int size() {return Size;}
        chain* find(int id) {
            return mp.find(id);
        }
        void make_head(chain* x) {
            if (x == head) return ;
            x->pre->nxt = x->nxt;
            if (x != tail) x->nxt->pre = x->pre;
                else tail = x->pre;
            x->pre = nullptr;
            x->nxt = head;
            head->pre = x;
            head = x;
        }

        void erase(chain *x) {
            Size--;
            // std::cerr << "start cache erase\n";
            // if (x == nullptr) std::cerr << "x is nullptr ! \n";
            // if (x == tail) std::cerr << "eraseing tail\n";
            mp.erase(x->data.id);
            if (x == head) head = x->nxt;
            if (x == tail) tail = x->pre;
            if (x->nxt != nullptr) x->nxt->pre = x->pre;
            if (x->pre != nullptr) x->pre->nxt = x->nxt;
            // std::cerr <<"mp.erase done\n";
            // if (x->data.id == 4080) {
                // std::cerr << "I delete 4080 !\n";
                // std::cerr << x->data.data.id << std::endl;
            // }
            delete x;
            // std::cerr << "end cache erase\n";
        }

        void insert(const int &id, const datatype &data) {
            Size++;
            chain *now = new chain(Data(data, id));
            mp.insert(id, now);
            if (head == nullptr) head = tail = now;
                else {
                    head->pre = now;
                    now->nxt = head;
                    head = now;
                }
        }

        void pop_last() {
            erase(tail);
        }
};

#endif //CACHE_H
