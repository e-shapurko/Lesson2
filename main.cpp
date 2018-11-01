#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include "ip_filter.h"



int main()
{
    try
    {
        V_STR ip_list;
        prepare_data_from_file(ip_list);
        ip_filter ipfilter(ip_list);

        VV_INT ranges;
        auto find_str = "*.*.*.*";
        ipfilter.write_to_console(find_str, -1);
        find_str = "1.*.*.*";
        ipfilter.write_to_console(find_str, -1);
        find_str = "46.70.*.*";
        ipfilter.write_to_console(find_str, -1);
        find_str = "?46.?46.?46.?46";
        ipfilter.write_to_console(find_str, -1);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
