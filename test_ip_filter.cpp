#define BOOST_TEST_MODULE test_ip_filter

#include "ip_filter.h"
#include <cstdlib>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace plug_nsp {
  inline bool plug() {
    return true;
  }
}

V_STR test_ip_list = 
{
"113.162.145.156",
"157.39.22.224",
"79.180.73.190",
"179.210.145.4",
"219.102.120.135",
"67.232.81.208",
"85.254.10.197",
"23.240.215.189",
"1.29.168.152",
"185.69.186.168",
"110.152.103.161",
"185.182.81.131",
"68.45.152.8",
"114.221.179.117",
"185.89.100.249",
"46.70.113.73",
"188.68.0.38",
"119.42.115.97",
"194.106.108.214",
"24.210.239.177",
"166.88.193.116",
"62.205.202.214",
"188.166.87.191",
"103.62.147.2",
"95.181.177.197",
"95.181.177.197",
"149.107.0.27",
"182.186.69.18",
"73.21.168.251",
"113.190.198.73",
"91.15.15.33",
"188.72.96.184",
"50.254.198.161",
"98.219.109.151",
"123.133.114.213",
"89.177.68.136",
"89.177.68.136",
"65.208.151.112",
"183.198.27.25",
"187.189.75.118",
"188.212.33.157",
"94.240.192.2",
"69.160.110.157",
"81.198.169.232",
"185.89.100.47",
"46.182.19.219",
"183.88.197.193",
"49.74.228.23",
"124.224.214.76",
"107.185.11.105"
};


class test_ip_filter
{
private:
    ip_filter m_ipfilter;
public:
    test_ip_filter(ip_filter &ipfilter):m_ipfilter(ipfilter)
    {
    }

    int get_ip_count(VV_INT &ranges)
    {
        int cnt = 0;
        for (int i = 0; i < ranges.size(); i++)
        {
            for (int ip_it = ranges[i][0]; ip_it <= ranges[i][1] ; ip_it++)
            {
                cnt++;
            }
        }
        return cnt;
    }

    void test_find_ranges(V_STR &test_ip_find, VV_INT &ranges)
    {
        m_ipfilter.find_ranges(m_ipfilter.ip_sorted_list, test_ip_find, ranges, 0);
    }
};


BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)

BOOST_AUTO_TEST_CASE(ip_filter_test_star_find_ranges) // test *
{
	
	ip_filter ipfilter(test_ip_list);
	test_ip_filter tif_class(ipfilter);
	V_STR test_ip_find = {"*","177","*","*"};
	VV_INT ranges;

	tif_class.test_find_ranges(test_ip_find, ranges);
    BOOST_CHECK(tif_class.get_ip_count(ranges) == 2);

}

BOOST_AUTO_TEST_CASE(ip_filter_test_all_star_find_ranges) // test *
{
	
	ip_filter ipfilter(test_ip_list);
	test_ip_filter tif_class(ipfilter);
	V_STR test_ip_find = {"*","*","*","*"};
	VV_INT ranges;

	tif_class.test_find_ranges(test_ip_find, ranges);
    BOOST_CHECK(tif_class.get_ip_count(ranges) == 50);

}

BOOST_AUTO_TEST_CASE(ip_filter_test_any_find_ranges) // test ?
{
	
	ip_filter ipfilter(test_ip_list);
	test_ip_filter tif_class(ipfilter);
	V_STR test_ip_find = {"?49","*","*","*"};
	VV_INT ranges;

	tif_class.test_find_ranges(test_ip_find, ranges);
    BOOST_CHECK(tif_class.get_ip_count(ranges) == 1);

}

BOOST_AUTO_TEST_CASE(ip_filter_test_value_find_ranges) // test value
{
	
	ip_filter ipfilter(test_ip_list);
	test_ip_filter tif_class(ipfilter);
	V_STR test_ip_find = {"65","208","151","112"};
	VV_INT ranges;

	tif_class.test_find_ranges(test_ip_find, ranges);
    BOOST_CHECK(tif_class.get_ip_count(ranges) == 1);

}

BOOST_AUTO_TEST_CASE(ip_filter_test_all_types_find_ranges) // test value star and any
{
	
	ip_filter ipfilter(test_ip_list);
	test_ip_filter tif_class(ipfilter);
	V_STR test_ip_find = {"65","?177","*","*"};
	VV_INT ranges;

	tif_class.test_find_ranges(test_ip_find, ranges);
    BOOST_CHECK(tif_class.get_ip_count(ranges) == 3);

}


BOOST_AUTO_TEST_SUITE_END()
