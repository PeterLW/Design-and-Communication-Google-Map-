#include <vector>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m3.h"
#include "m4.h"
#include "tour_verify.h"

SUITE(Extreme) {
	TEST(Test_Extreme_0) {
		std::vector<unsigned> delivery_ids = {334, 4904, 6868, 9693, 11584, 16073, 19450, 22197, 23666, 24352, 24756, 24837, 26373, 28349, 28760, 29864, 33378, 44271, 45997, 53528, 54101, 59155, 59257, 59823, 63019, 67387, 68370, 69112, 71050, 71395, 75438, 85427, 86482, 97085, 103839, 105650, 109689, 119610, 122014, 129738, 137740, 140247, 141616, 143665, 143872, 144521, 152618, 157705, 159225, 163352};
		std::vector<unsigned> depot_ids = {6604, 15321, 30063, 57131, 77774, 97918, 102219, 103141, 160373, 160522};

		std::vector<unsigned> tour;
		{
			UNITTEST_TIME_CONSTRAINT(30000);
			tour = traveling_salesman(delivery_ids, depot_ids);
		}

		bool tour_is_legal = ece297test::tour_is_legal(delivery_ids, depot_ids, tour);
		CHECK(tour_is_legal == true);

		if(tour_is_legal) {
			double tour_cost = compute_path_travel_time(tour);
			std::cout << "Test_Extreme_0 Cost: " << tour_cost << std::endl;
		} else {
			std::cout << "Test_Extreme_0 Cost: N/A (illegal)" << std::endl;
		}
	}
}
