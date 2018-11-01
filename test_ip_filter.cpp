#define BOOST_TEST_MODULE test_ip_filter

#include "ip_filter.h"

#include <boost/test/unit_test.hpp>

namespace compare_nsp {
  inline bool plug() {
    return true;
  }
}

BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)

BOOST_AUTO_TEST_CASE(ip_filter_test_version)
{
    BOOST_CHECK(plug() == true);
}

BOOST_AUTO_TEST_SUITE_END()
