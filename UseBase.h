#ifndef USEBASE_H
#define USEBASE_H
#pragma once

#include <string>
#include <vector>

typedef std::vector<std::vector<int>>::iterator INT_ITR;
typedef std::vector<std::vector<int>>::reverse_iterator INT_REV_ITR;
typedef std::vector<std::vector<int>> VV_INT;
typedef std::vector<std::string> V_STR;
typedef std::vector<int> V_INT;
typedef unsigned int u_int;

#ifndef FALSE
#define FALSE                   0
#endif

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE    -1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif



inline std::vector<std::string> split(const std::string &str, const char d)
{
    V_STR r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

inline bool prepare_data_from_file(OUT V_STR &data)
{
    std::ifstream file;
    std::string line;
    file.open("./ip_filter.tsv");
    while(std::getline(file, line))
    {
        {
            V_STR v = split(line, '\t');
            data.push_back(v[0]);
        }
    }
    return false;
}



#endif // USEBASE_H


