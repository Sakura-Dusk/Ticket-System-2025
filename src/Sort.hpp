//
// Created by Sakura on 25-5-25.
//

#ifndef SORT_HPP
#define SORT_HPP

namespace SJTU {
    template<typename Iterator, typename Compare>
    void merge(const Iterator& begin, const Iterator& mid, const Iterator& end, Compare cmp) {
        auto size = end - begin;
        auto merged = new int [size + 1];

        // Iterator tmp = begin;
        // std::cerr << "left : ";
        // while (tmp != mid) {
        //     std::cerr << *tmp << " ";
        //     ++tmp;
        // }
        // std::cerr << std::endl;
        // tmp = mid;
        // std::cerr << "right : ";
        // while (tmp != end) {
        //     std::cerr << *tmp << " ";
        //     ++tmp;
        // }
        // std::cerr << std::endl;

        auto pos = merged;
        Iterator anow = begin, bnow = mid;
        while (anow != mid && bnow != end) {
            if (cmp(*anow, *bnow)) {
                *pos = std::move(*anow);
                ++pos; ++anow;
            }
            else {
                *pos = std::move(*bnow);
                ++pos; ++bnow;
            }
        }
        while (anow != mid) {
            *pos = std::move(*anow);
            ++pos; ++anow;
        }
        while (bnow != end) {
            *pos = std::move(*bnow);
            ++pos; ++bnow;
        }

        pos = merged;
        for (auto i = begin; i != end; i++) {
            *i = std::move(*pos);
            ++pos;
        }
        delete[] merged;

        // tmp = begin;
        // std::cerr << "after sort : ";
        // while (tmp != end) {
        //     std::cerr << *tmp << " ";
        //     ++tmp;
        // }
        // std::cerr << std::endl;
    }

    template<typename Iterator, typename Compare>
    void sort(const Iterator& begin, const Iterator& end, Compare cmp) {
        if (begin + 1 == end) return ;
        auto size = end - begin;
        // std::cerr << "size = " << size << std::endl;
        Iterator mid = begin + (size / 2);
        SJTU::sort(begin, mid, cmp);
        SJTU::sort(mid, end, cmp);
        merge(begin, mid, end, cmp);
    }
}

#endif //SORT_HPP
