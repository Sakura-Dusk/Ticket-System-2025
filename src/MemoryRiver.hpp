#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <iostream>
#include <filesystem>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    ~MemoryRiver() {
        // std::cerr << "close initialise\n";
        file.close();
    }

    void initialise(string FN = "") {
        // std::cerr << FN << std::endl;
        if (FN != "") file_name = FN;
        if (std::filesystem::exists(file_name)) {
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            file.seekp(info_len * sizeof(int), std::ios_base::beg);
            file.close();
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            return ;
        }
        // std::cerr << "new initialise :" << FN << "\n";
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        /* your code here */
        // file.open(file_name, std::ios::in);
        // std::cerr << file.good() << std::endl;
        // std::cerr << file.tellg() << std::endl;
        file.seekg((n - 1) * sizeof(int), std::ios_base::beg);
        // std::cerr << file.good() << std::endl;
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        // std::cerr << file.good() << std::endl;
        // file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        /* your code here */
        // file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp((n - 1) * sizeof(int), std::ios_base::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        // std::cerr << file.good() << std::endl;
        // file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        /* your code here */
        // file.open(file_name, std::ios::app | std::ios::binary);
        file.seekp(0, std::ios::end);
        int index = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        // file.close();
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        /* your code here */
        // file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(index, std::ios_base::beg);
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        // file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        /* your code here */
        // file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(index, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        // file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        /* your code here */
    }

    void clear_all() {
        int len = file_name.size();
        char* str = new char[len + 1];
        for (int i = 0; i < len; i++) str[i] = file_name[i];
        std::remove(str);
        delete[] str;
        // file.close();
        // file.open(file_name, std::ios::out);
        // file.close();
        // file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
};


#endif //BPT_MEMORYRIVER_HPP