#include "ip_filter.h"
#include <math.h>



ip_filter::ip_filter(V_STR ip_list)
{
    create_ip_sorted_lsit(ip_list);
}


//  :)
void ip_filter::write_to_console(const std::string &patern_str, const int &dir /*направление сортировки и шаг*/)
{
    VV_INT ranges;
    int start;
    int stop;
    int start_range;
    int stop_range;
    int sign_part = round(dir/abs(dir));

    if (dir == 0)
        return;

    find_ranges(ip_sorted_list, split(patern_str, '.'), ranges, 0);

    // чтоб цикл получился двунаправленным
    start_range = (dir > 0) ? 0 : (ranges.size() - 1);
    stop_range = (dir > 0) ? (ranges.size() - 1) : 0;

    for (int i = start_range; (i * sign_part) <= (stop_range * sign_part); i += dir)
    {
        start = (dir > 0) ? ranges[i][0] : ranges[i][1];
        stop = (dir > 0) ? ranges[i][1] : ranges[i][0];

        for (int ip_it = start; ((sign_part * ip_it) <= (sign_part * stop)) ; ip_it += dir)
        {
            std::cout << convert_ip_vitos(ip_sorted_list[ip_it]) << std::endl;
        }
    }
}

int ip_filter::find_place_for_insert(VV_INT &where_find, V_INT &what_find, const u_int &from_index, const u_int &to_index, const u_int step)
{
    unsigned int start = from_index;
    unsigned int stop = to_index;
    bool b_start = false;
    bool b_check_equal = false;

    if (where_find.size() == 0)
    {
        return 0;
    }

    for (u_int i = from_index; i <= to_index; i++)
    {
        u_int what_find_ip_part = what_find[step];
        u_int where_find_ip_part = where_find[i][step];

        if ((what_find_ip_part > where_find_ip_part) && (i == to_index))
        {
            return to_index + 1;
        }
        else if (what_find_ip_part > where_find_ip_part)
        {
            start = stop = i;
        }
        else if (where_find_ip_part == what_find_ip_part)
        {
            b_check_equal = true;
            if (b_start == false)
            {
                b_start = true;
                start = i;
            }
            stop = i;
            if ((i == to_index) and (step != LAST_IP_PART))
            {
                return find_place_for_insert(where_find, what_find, start, stop, step + 1);
            }

        }
        else if (what_find_ip_part < where_find_ip_part)
        {
            if (step == LAST_IP_PART)
            {
                return i;
            }
            if (b_check_equal == false)
                return i;
            else
                return find_place_for_insert(where_find, what_find, start, stop, step + 1);
        }
    }
    return start;
}

void ip_filter::find_ranges(VV_INT &where_find, const V_STR &what_find, VV_INT &ranges, const u_int ip_part, int find_type)
{
    auto b_start = false;
    auto b_check_equal = false;

    if (where_find.size() < 1)
        return;

    VV_INT ranges_for_find;
    auto what_find_str = what_find[ip_part];
    if ((ip_part == FIRST_IP_PART) && (ranges.size() == 0))
    {
        ranges.push_back({0, (int)where_find.size() - 1}); //задание диапазона поиска если он не задан изначально;
    }

    if ((what_find_str == "*") && (ip_part != LAST_IP_PART))
    {
        find_type &= FIND_ALL;
        find_ranges(where_find, what_find, ranges, ip_part + 1, find_type);
        return;
    }
    else if ((what_find_str == "*") && (ip_part == LAST_IP_PART))
    {
        find_type &= FIND_ALL;
        return;
    }
    else if(what_find_str.find_first_of("?") != std::string::npos)
    {
        find_type &= FIND_ANY; // приоретный флаг - затирает FIND_ALL
        what_find_str.replace(0, 1, "");
        ranges_for_find.push_back({0, (int)where_find.size() - 1});
    }
    else
    {
        find_type &= FIND_VALUE; // приоретный флаг - затирает FIND_ALL
        ranges_for_find = ranges;
    }


    VV_INT temp_ranges;
    int start;
    int stop;
    auto what_find_ip_part = std::stoi(what_find_str);


    for (int i = 0; i < ranges_for_find.size(); i++)
    {
        start   = ranges_for_find[i][0];
        stop    = ranges_for_find[i][1];
        b_start = false;
        b_check_equal = false;

        for (auto lim = ranges_for_find[i][0]; lim <= ranges_for_find[i][1]; lim++)
        {
            if (where_find[lim][ip_part] == what_find_ip_part)
            {
                b_check_equal = true;
                if (b_start == false)
                {
                    b_start = true;
                    start = lim;
                }
                stop = lim;
                if (lim == ranges_for_find[i][1])
                {
                    temp_ranges.push_back({start, stop});
                }
            }
            else
            {
                if (b_check_equal == true)
                {
                    b_check_equal = false;
                    b_start = false;
                    temp_ranges.push_back({start, stop});
                }
            }
        }
    }

    if (
            ((find_type == FIND_ANY) || (find_type == FIND_ANY_OR_VALUE))
            &&
            (ip_part != FIRST_IP_PART) //если копашимся не с первым байтом ip адреса
       )
        merge_ranges(ranges, temp_ranges);
    else
        ranges = temp_ranges;

    if ((ranges.size() > 0) && (ip_part != LAST_IP_PART))
        find_ranges(where_find, what_find, ranges, ip_part + 1, find_type);
    return;
}

void ip_filter::merge_ranges(VV_INT &ranges, VV_INT &added_ranges)
{
    VV_INT temp_ranges;
    VV_INT result_ranges;
    V_INT range;
    V_INT added_range;

    bool ranges_enabled;
    bool added_ranges_enabled;
    INT_ITR change_it;
    INT_ITR ranges_it = ranges.begin();
    INT_ITR added_ranges_it = added_ranges.begin();

    //сортировка по значению начала диапазона {{ot 3 - do 5}{4 - 19}{8 - 9}}
    while (1)
    {
        ranges_enabled = (ranges_it != ranges.end());
        added_ranges_enabled = (added_ranges_it != added_ranges.end());

        if ((added_ranges_enabled == true) and (ranges_enabled == false))
        // если закончился диапазаон с которым мерджимся
        {
            added_range = *added_ranges_it;
            temp_ranges.push_back(added_range);
            added_ranges_it++;
            continue;
        }
        else if ((added_ranges_enabled == false) and (ranges_enabled == true))
        // если закончился диапазан который мерджим
        {
            range = *ranges_it;
            temp_ranges.push_back(range);
            ranges_it++;
            continue;
        }
        else if ((added_ranges_enabled == false) and (ranges_enabled == false))
        // если всё закончилось
        {
            break;
        }

        added_range = *added_ranges_it;
        range = *ranges_it;
        if (added_range[0] <= range[0])
        {
            temp_ranges.push_back(added_range);
            added_ranges_it++;
            continue;
        }
        else
        {
            temp_ranges.push_back(range);
            ranges_it++;
            continue;
        }
    }

    auto finding_new_range = true; //находимся в состоянии поиска максимумов и минимумов нового диапазона
    auto last_lap = false; //последняя итерация
    int start;
    int stop;

    //нахождение пересечений диапазонов и их схлопывание.

//    for(u_int i = 0; i < temp_ranges.size();)
    for(VV_INT::iterator range = temp_ranges.begin(); range != temp_ranges.end();)
    {
        last_lap = (range == (temp_ranges.end() - 1));
        if (finding_new_range == true)
        {
            start = (*range)[0];
            stop = (*range)[1];
            if (last_lap == true)
                result_ranges.push_back({start, stop});
            else
                finding_new_range = false;
            range++;
            continue;
        }
        else
        {
            if ((*range)[0] == stop)
            {
                stop = (*range)[1];
                range++;
                continue;
            }
            else if ((*range)[0] > stop)
            {
                result_ranges.push_back({start, stop});
                finding_new_range = true;
                continue;
            }
            else if ((*range)[0] < stop)
            {
                if ((*range)[1] == stop)
                {
                    range++;
                    continue;
                }
                else if ((*range)[1] > stop)
                {
                    stop = (*range)[1];
                    range++;
                    continue;
                }
                else if ((*range)[1] < stop)
                {
                    range++;
                    continue;
                }
            }
        }
    }
    ranges = result_ranges;
}

std::string ip_filter::convert_ip_vitos(const V_INT &int_ip)
{
    std::string res = "";
    auto delimetr = "";
    for (auto i = 0; i < 4; i++)
    {
        res = res + delimetr + std::to_string(int_ip[i]);
        delimetr = ".";
    }
    return res;
}

V_INT ip_filter::convert_vstovi(const V_STR &str_ip)
{
    V_INT res;
    for (int i = 0; i < 4; i++)
    {
        std::stoi(str_ip[i]);
        res.push_back(std::stoi(str_ip[i]));
    }
    return res;
}

void ip_filter::add_ip(VV_INT &data, const INT_ITR &position, V_INT &ip)
{
    try
    {
        data.insert(position, ip);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ip_filter::create_ip_sorted_lsit(V_STR ip_list)
{
    V_INT ip_int_list;
    for(V_STR::iterator ip = ip_list.begin(); ip != ip_list.end(); ip++)
    {
        ip_int_list = convert_vstovi(split(*ip, '.'));
        INT_ITR ins_it = ip_sorted_list.begin();
        auto place = find_place_for_insert(ip_sorted_list, ip_int_list, 0, ip_sorted_list.size() - 1, 0);
        std::advance(ins_it, place);
        add_ip(ip_sorted_list, ins_it, ip_int_list);
    }
}


