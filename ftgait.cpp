// ====================================================
// gait SKILL command tests
// ====================================================
#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {
		using std::chrono::milliseconds;
		using std::this_thread::sleep_for;

		//using ftfBittleXGait = ftfBittleX;
		struct ftfBittleXGait : public ftfBittleX
		{
			~ftfBittleXGait() {
				// n.b., this is cosmetic, not test code

				// let gait run for 2 seconds
				sleep_for(milliseconds(2000));

				// demo gait pose "freeze frame"
				EXPECT_TRUE(on_pause());
				sleep_for(milliseconds(1000));
			}
		};

		// bound forward bdF
		// todo: causes battery light to flicker
		TEST_F(ftfBittleXGait, DISABLED_bound_forward) {
			EXPECT_TRUE(on_bound_forward());
		}
		// backward bk
		TEST_F(ftfBittleXGait, backward) {
			EXPECT_TRUE(on_backward());
		}
		// backward Left bkL
		TEST_F(ftfBittleXGait, backward_left) {
			EXPECT_TRUE(on_backward_left());
		}
		TEST_F(ftfBittleXGait, backward_right) {
			EXPECT_TRUE(on_backward_right());
		}
		TEST_F(ftfBittleXGait, backward_random) {
			EXPECT_TRUE(on_backward_random());
		}

		// crawl forward crF
		TEST_F(ftfBittleXGait, crawl_forward) {
			EXPECT_TRUE(on_crawl_forward());
		}
		// crawl Left crL
		TEST_F(ftfBittleXGait, crawl_Left) {
			EXPECT_TRUE(on_crawl_left());
		}
		TEST_F(ftfBittleXGait, crawl_right) {
			EXPECT_TRUE(on_crawl_right());
		}
		TEST_F(ftfBittleXGait, crawl_random) {
			EXPECT_TRUE(on_crawl_random());
		}

		// gap forward gpF
		TEST_F(ftfBittleXGait, gap_forward) {
			EXPECT_TRUE(on_gap_forward());
		}
		// gap Left gpL
		TEST_F(ftfBittleXGait, gap_Left) {
			EXPECT_TRUE(on_gap_left());
		}
		TEST_F(ftfBittleXGait, gap_right) {
			EXPECT_TRUE(on_gap_right());
		}
		TEST_F(ftfBittleXGait, gap_random) {
			EXPECT_TRUE(on_gap_random());
		}
		// halloween gait hlw
		TEST_F(ftfBittleXGait, halloween) {
			EXPECT_TRUE(on_halloween());
		}

		// jump forward jpF (violent motion)
		// todo: causes battery light to flicker
		TEST_F(ftfBittleXGait, DISABLED_jump_forward) {
			EXPECT_TRUE(on_jump_forward());
		}

		// push forward phF
		TEST_F(ftfBittleXGait, push_forward) {
			EXPECT_TRUE(on_push_forward());
		}
		// push Left phL
		TEST_F(ftfBittleXGait, push_left) {
			EXPECT_TRUE(on_push_left());
		}
		TEST_F(ftfBittleXGait, push_right) {
			EXPECT_TRUE(on_push_right());
		}
		TEST_F(ftfBittleXGait, push_random) {
			EXPECT_TRUE(on_push_random());
		}
		// trot forward trF
		TEST_F(ftfBittleXGait, trot_forward) {
			EXPECT_TRUE(on_trot_forward());
		}
		// trot Left trL
		TEST_F(ftfBittleXGait, trot_Left) {
			EXPECT_TRUE(on_trot_left());
		}
		TEST_F(ftfBittleXGait, trot_right) {
			EXPECT_TRUE(on_trot_right());
		}
		TEST_F(ftfBittleXGait, trot_random) {
			EXPECT_TRUE(on_trot_random());
		}
		// step at origin vtF
		TEST_F(ftfBittleXGait, step) {
			EXPECT_TRUE(on_step());
		}
		// spring left vtL
		TEST_F(ftfBittleXGait, spring_left) {
			EXPECT_TRUE(on_spring_left());
		}
		TEST_F(ftfBittleXGait, spring_right) {
			EXPECT_TRUE(on_spring_right());
		}
		TEST_F(ftfBittleXGait, spring_random) {
			EXPECT_TRUE(on_spring_random());
		}
		// walk forward wkF
		TEST_F(ftfBittleXGait, walk_forward) {
			EXPECT_TRUE(on_walk_forward());
		}
		// walk left wkL
		TEST_F(ftfBittleXGait, walk_left) {
			EXPECT_TRUE(on_walk_left());
		}
		TEST_F(ftfBittleXGait, walk_right) {
			EXPECT_TRUE(on_walk_right());
		}
		TEST_F(ftfBittleXGait, walk_random) {
			EXPECT_TRUE(on_walk_random());
		}

		TEST_F(ftfBittleXGait, Gait_Data) {
			ASSERT_TRUE(on_gait_data());
		}

		// ftSerial.write FAILED expected: 934 actual: 640
		TEST_F(ftfBittleXGait, Gait_Data_User) {
			const int8_t wkF[] = {
				116, 0, 0, 1,
				18, 54, 58, 52, 7, 13, -2, 9,
				16, 55, 56, 52, 8, 13, -3, 9,
				15, 55, 55, 53, 10, 14, -3, 9,
				14, 56, 52, 54, 12, 14, -3, 10,
				13, 56, 51, 54, 15, 14, -4, 10,
				13, 56, 48, 55, 16, 15, -4, 10,
				15, 57, 46, 55, 17, 15, -4, 11,
				16, 57, 43, 56, 16, 16, -3, 11,
				16, 57, 41, 56, 16, 17, -3, 12,
				16, 58, 38, 57, 16, 17, -2, 12,
				17, 58, 36, 57, 15, 18, -2, 13,
				18, 58, 33, 58, 14, 19, -1, 14,
				19, 59, 31, 58, 13, 19, 1, 14,
				20, 59, 28, 60, 13, 20, 2, 13,
				21, 59, 26, 60, 12, 21, 4, 14,
				22, 59, 24, 60, 12, 22, 6, 14,
				24, 59, 23, 61, 11, 23, 9, 15,
				25, 60, 22, 61, 10, 24, 11, 16,
				26, 60, 21, 61, 10, 24, 12, 16,
				26, 60, 22, 61, 10, 26, 15, 16,
				28, 60, 23, 62, 9, 27, 14, 17,
				28, 58, 23, 62, 9, 30, 16, 18,
				30, 60, 23, 62, 8, 29, 16, 19,
				31, 60, 23, 62, 8, 30, 16, 20,
				31, 61, 25, 63, 8, 30, 13, 20,
				32, 64, 26, 62, 7, 26, 13, 22,
				32, 66, 27, 62, 7, 24, 12, 23,
				34, 67, 28, 63, 7, 23, 12, 24,
				34, 68, 29, 63, 7, 19, 11, 25,
				35, 68, 30, 63, 6, 19, 11, 26,
				36, 69, 31, 63, 6, 15, 11, 27,
				37, 69, 32, 63, 6, 12, 10, 29,
				38, 68, 33, 63, 6, 10, 10, 30,
				38, 68, 34, 63, 7, 8, 10, 31,
				39, 67, 35, 63, 7, 6, 10, 32,
				40, 66, 36, 62, 7, 4, 9, 34,
				41, 65, 36, 62, 7, 3, 9, 35,
				42, 63, 37, 62, 7, 1, 9, 37,
				42, 62, 38, 63, 7, 0, 9, 38,
				43, 60, 39, 65, 7, -1, 9, 37,
				44, 59, 39, 66, 7, -2, 9, 34,
				45, 57, 40, 69, 7, -2, 9, 31,
				46, 55, 41, 70, 7, -3, 9, 27,
				47, 53, 41, 71, 7, -3, 9, 26,
				47, 52, 42, 71, 7, -5, 9, 26,
				47, 50, 43, 71, 7, -5, 9, 23,
				48, 48, 44, 71, 7, -5, 9, 20,
				48, 45, 46, 71, 8, -5, 7, 17,
				49, 43, 47, 71, 8, -5, 7, 14,
				50, 40, 47, 70, 8, -4, 7, 12,
				51, 38, 48, 70, 8, -3, 7, 9,
				51, 35, 48, 69, 8, -3, 8, 8,
				52, 33, 49, 68, 9, -2, 8, 5,
				52, 30, 50, 67, 9, -1, 8, 4,
				53, 28, 51, 66, 9, 1, 8, 2,
				54, 24, 51, 65, 10, 3, 8, 1,
				53, 22, 52, 64, 12, 4, 9, -1,
				54, 18, 52, 63, 12, 7, 9, -2,
				54, 18, 52, 61, 13, 7, 9, -3,
				55, 16, 53, 58, 13, 8, 9, -2,
				55, 15, 54, 56, 14, 10, 10, -3,
				56, 14, 54, 55, 14, 12, 10, -3,
				56, 13, 55, 52, 14, 15, 10, -3,
				56, 13, 55, 51, 15, 16, 11, -4,
				57, 15, 56, 48, 15, 17, 11, -4,
				57, 16, 56, 46, 16, 16, 12, -4,
				57, 16, 57, 43, 17, 16, 12, -3,
				58, 16, 57, 41, 17, 16, 13, -3,
				58, 17, 58, 38, 18, 15, 14, -2,
				58, 18, 58, 36, 19, 14, 14, -2,
				59, 19, 60, 33, 19, 13, 13, -1,
				59, 20, 60, 31, 20, 13, 14, 1,
				59, 21, 60, 28, 21, 12, 14, 2,
				59, 22, 61, 26, 22, 12, 15, 4,
				59, 24, 61, 24, 23, 11, 16, 6,
				60, 25, 61, 23, 24, 10, 16, 9,
				60, 26, 61, 22, 24, 10, 16, 11,
				60, 26, 62, 21, 26, 10, 17, 12,
				60, 28, 62, 22, 27, 9, 18, 15,
				58, 28, 62, 23, 30, 9, 19, 14,
				60, 30, 62, 23, 29, 8, 20, 16,
				60, 31, 63, 23, 30, 8, 20, 16,
				61, 31, 62, 23, 30, 8, 22, 16,
				64, 32, 62, 25, 26, 7, 23, 13,
				66, 32, 63, 26, 24, 7, 24, 13,
				67, 34, 63, 27, 23, 7, 25, 12,
				68, 34, 63, 28, 19, 7, 26, 12,
				68, 35, 63, 29, 19, 6, 27, 11,
				69, 36, 63, 30, 15, 6, 29, 11,
				69, 37, 63, 31, 12, 6, 30, 11,
				68, 38, 63, 32, 10, 6, 31, 10,
				68, 38, 63, 33, 8, 7, 32, 10,
				67, 39, 62, 34, 6, 7, 34, 10,
				66, 40, 62, 35, 4, 7, 35, 10,
				65, 41, 62, 36, 3, 7, 37, 9,
				63, 42, 63, 36, 1, 7, 38, 9,
				62, 42, 65, 37, 0, 7, 37, 9,
				60, 43, 66, 38, -1, 7, 34, 9,
				59, 44, 69, 39, -2, 7, 31, 9,
				57, 45, 70, 39, -2, 7, 27, 9,
				55, 46, 71, 40, -3, 7, 26, 9,
				53, 47, 71, 41, -3, 7, 26, 9,
				52, 47, 71, 41, -5, 7, 23, 9,
				50, 47, 71, 42, -5, 7, 20, 9,
				48, 48, 71, 43, -5, 7, 17, 9,
				45, 48, 71, 44, -5, 8, 14, 9,
				43, 49, 70, 46, -5, 8, 12, 7,
				40, 50, 70, 47, -4, 8, 9, 7,
				38, 51, 69, 47, -3, 8, 8, 7,
				35, 51, 68, 48, -3, 8, 5, 7,
				33, 52, 67, 48, -2, 9, 4, 8,
				30, 52, 66, 49, -1, 9, 2, 8,
				28, 53, 65, 50, 1, 9, 1, 8,
				24, 54, 64, 51, 3, 10, -1, 8,
				22, 53, 63, 51, 4, 12, -2, 8,
				18, 54, 61, 52, 7, 12, -3, 9,
			};

			EXPECT_TRUE(on_skill_data(wkF, sizeof(wkF)));
		}

	}	// namespace ftBittleX
}	// namespace utPetoi
