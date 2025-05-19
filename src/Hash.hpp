//
// Created by Sakura on 25-5-17.
//

#ifndef HASH_H
#define HASH_H

#include "vector.hpp"

constexpr int mo = 137;

template <typename valuetype>
class Hash {
    private:
        struct Hash_data {
             int id;
             valuetype *value;
             Hash_data(){}
             Hash_data(const int &id, valuetype* value) : id(id), value(value){}
        };
        vector <Hash_data> f[mo];

    public:
        Hash() {
           for (int i = 0; i < mo; i++) f[i] = vector<Hash_data>();
        }

        void insert(int id, valuetype* value) {
            // std::cerr << " - insert id : " << id << std::endl;
            int id_mo = id % mo;
            // std::cerr << "id_mo = " << id_mo << std::endl;
            f[id_mo].push_back(Hash_data(id, value));
        }

        void erase(int id) {
            int id_mo = id % mo;
            for (int i = 0; i < f[id_mo].size(); i++) {
                if (f[id_mo][i].id == id) {
                    f[id_mo].erase(i);
                    return ;
                }
            }
        }

        valuetype* find(int id) {
            // std::cerr << " - find id : " << id << std::endl;
            int id_mo = id % mo;
            // std::cerr << "id_mo = " << id_mo << std::endl;
            for (int i = 0; i < f[id_mo].size(); i++) {
                // std::cerr << f[id_mo][i].id << std::endl;
                if (f[id_mo][i].id == id) return f[id_mo][i].value;
            }
            return nullptr;
        }
};



#endif //HASH_H
