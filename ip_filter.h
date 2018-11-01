#ifndef IP_FILTER_H
#define IP_FILTER_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include "UseBase.h"



const int LAST_IP_PART = 3;
const int FIRST_IP_PART = 0;

const int FIND_ALL = 15;
const int FIND_VALUE = 12;
const int FIND_ANY = 10;
const int FIND_ANY_OR_VALUE = 8;

class ip_filter
{
private:
    VV_INT ip_sorted_list;
    int find_place_for_insert(VV_INT &where_find,
                              V_INT &what_find,
                              const u_int &from_index,
                              const u_int &to_index,
                              const u_int step);
    void find_ranges(VV_INT &where_find,
                     const V_STR &what_find,
                     VV_INT &ranges,
                     const u_int step,
                     int find_type = FIND_ALL);
    void merge_ranges(VV_INT &ranges, VV_INT &added_ranges);

    std::string convert_ip_vitos(const V_INT &int_ip);
    V_INT convert_vstovi(const V_STR &str_ip);

    void add_ip(VV_INT &data, const INT_ITR &position, V_INT &ip);

    void create_ip_sorted_lsit(V_STR ip_list);

public:
    ip_filter(V_STR ip_list);
    void write_to_console(const std::string &patern_str, const int &dir = 1);
    friend class test_ip_filter;

};

#endif // IP_FILTER_H
