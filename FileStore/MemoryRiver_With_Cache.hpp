//
// Created by Sakura on 25-5-17.
//

#ifndef MEMORYRIVER_WITH_CACHE_H
#define MEMORYRIVER_WITH_CACHE_H

#include "Cache.hpp"
#include "MemoryRiver.hpp"

template<class T, int info_len = 2, int MAX_LEN = 128>
class MEMORYRIVER_WITH_CACHE {
    private:
        MemoryRiver<T, info_len> memory_river;
        CACHE<T, MAX_LEN> cache;

        void pop_last() {
            auto last = cache.end();
            // if (last == nullptr) {
                // std::cerr << "last become nullptr !\n";
            // }
            // else {
            //     std::cerr << "last->data.id = " << last->data.id << " , last->data.is_change = " << last->data.is_change << std::endl;
            // }
            if (last->data.is_change) {
                memory_river.update(last->data.data, last->data.id);
            }
            cache.pop_last();
        }

    public:
        MEMORYRIVER_WITH_CACHE(const string& filename = "") {}
        ~MEMORYRIVER_WITH_CACHE() {
            // std::cerr << "~MEMORYRIVER_WITH_CACHE start\n";
            // std::cerr << "cache.size = " << cache.size() << std::endl;
            while (cache.size()) {
                pop_last();
                // std::cerr << "cache.size = " << cache.size() << std::endl;
            }
            // std::cerr << "~MEMORYRIVER_WITH_CACHE done\n";
        }

        void initialise(string FN = "") {
            memory_river.initialise(FN);
        }
        void get_info(int &tmp, int n) {
            memory_river.get_info(tmp, n);
        }
        void write_info(int tmp, int n) {
            memory_river.write_info(tmp, n);
        }

        int write(T &t) {
            int id = memory_river.write(t);
            // std::cerr << "write before cache size : " << cache.size() << std::endl;
            // std::cerr << "write about : " << id << std::endl;
            if (cache.size() == MAX_LEN) pop_last();
            cache.insert(id, t);
            // std::cerr << "now cache size : " << cache.size() << std::endl;
            return id;
        }

        void update(T &t, const int index) {
            // std::cerr << "update before cache size : " << cache.size() << std::endl;
            // std::cerr << "update about : " << t.id << std::endl;
            auto now = cache.find(index);
            if (now == nullptr) {
                // std::cerr << "not in cache\n";
                memory_river.update(t, index);
                if (cache.size() == MAX_LEN) pop_last();
                cache.insert(index, t);
            }
            else {
                // std::cerr << "in cache\n";
                now->data.update_Data(t);
                cache.make_head(now);
            }
            // std::cerr << "now cache size : " << cache.size() << std::endl;
        }

        void read(T &t, const int index) {
            // std::cerr << "read before cache size : " << cache.size() << std::endl;
            // std::cerr << "read about : " << index << std::endl;
            auto now = cache.find(index);
            if (now == nullptr) {
                // std::cerr << "not in cache\n";
                memory_river.read(t, index);
                // std::cerr << "t.id = " << t.id << std::endl;
                if (cache.size() == MAX_LEN) pop_last();
                cache.insert(index, t);
            }
            else {
                // std::cerr << "in cache\n";
                t = now->data.data;
                cache.make_head(now);
            }
            // std::cerr << "now cache size : " << cache.size() << std::endl;
        }

        void Delete(int index) {
            /* your code here */
        }
};

#endif //MEMORYRIVER_WITH_CACHE_H
