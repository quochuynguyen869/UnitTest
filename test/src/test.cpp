#include "gtest/gtest.h"
#include <iostream>
#include <assert.h>
#include "test_app.c"

using namespace std;



TEST(Stack, empty){

	EXPECT_EQ(1, 1);

}



int main(int argc, char **argv){

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

	return 0;
}
