#include <vector>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m3.h"
#include "m4.h"
#include "tour_verify.h"

SUITE(Simple) {
	TEST(Test_Simple_0) {
		std::vector<unsigned> delivery_ids = {26713, 111288, 159340};
		std::vector<unsigned> depot_ids = {9085, 122230};

		std::vector<unsigned> tour;
		{
			UNITTEST_TIME_CONSTRAINT(30000);
			tour = traveling_salesman(delivery_ids, depot_ids);
		}

		bool tour_is_legal = ece297test::tour_is_legal(delivery_ids, depot_ids, tour);
		CHECK(tour_is_legal == true);

		if(tour_is_legal) {
			double tour_cost = compute_path_travel_time(tour);
			std::cout << "Test_Simple_0 Cost: " << tour_cost << std::endl;
		} else {
			std::cout << "Test_Simple_0 Cost: N/A (illegal)" << std::endl;
		}
	}
}
