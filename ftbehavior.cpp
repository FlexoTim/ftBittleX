#include "ftBittleX.h"

namespace utPetoi {

	namespace ftBittleX {

		// ====================================================
		// behavior SKILL command tests
		// ====================================================
		using ftfBittleXBehavior = ftfBittleX;

#ifdef DISABLED_CODE
#endif	// DISABLED_CODE
		// angry (violent motion)
		TEST_F(ftfBittleXBehavior, angry) {
			EXPECT_TRUE(on_angry());
		}

		TEST_F(ftfBittleXBehavior, DISABLED_backflip) {
			EXPECT_TRUE(on_backflip());
		}

		// boxing (violent motion)
		TEST_F(ftfBittleXBehavior, boxing) {
			EXPECT_TRUE(on_boxing());
		}

		// cheers
		TEST_F(ftfBittleXBehavior, cheers) {
			EXPECT_TRUE(on_cheers());
		}

		// check
		TEST_F(ftfBittleXBehavior, check) {
			EXPECT_TRUE(on_check());
		}
		// come here
		TEST_F(ftfBittleXBehavior, come_here) {
			EXPECT_TRUE(on_come_here());
		}
		// dig
		TEST_F(ftfBittleXBehavior, dig) {
			EXPECT_TRUE(on_dig());
		}

		TEST_F(ftfBittleXBehavior, DISABLED_frontflip) {
			EXPECT_TRUE(on_frontflip());
		}

		// high five
		TEST_F(ftfBittleXBehavior, high_five) {
			EXPECT_TRUE(on_high_five());
		}
		// good boy
		TEST_F(ftfBittleXBehavior, good_boy) {
			EXPECT_TRUE(on_good_boy());
		}

		TEST_F(ftfBittleXBehavior, handstand) {
			cmd_skill command{ "hds", "handstand" };
			EXPECT_TRUE(on_command(command, 4000));
			//EXPECT_TRUE(on_good_boy());
		}

		TEST_F(ftfBittleXBehavior, hug) {
			EXPECT_TRUE(on_hug());
		}
		TEST_F(ftfBittleXBehavior, hi) {
			EXPECT_TRUE(on_hi());
		}
		TEST_F(ftfBittleXBehavior, hand_shake) {
			EXPECT_TRUE(on_hand_shake());
		}
		TEST_F(ftfBittleXBehavior, hands_up) {
			EXPECT_TRUE(on_hands_up());
		}

		TEST_F(ftfBittleXBehavior, jump) {
			EXPECT_TRUE(on_jump());
		}

		TEST_F(ftfBittleXBehavior, kick) {
			EXPECT_TRUE(on_kick());
		}

		TEST_F(ftfBittleXBehavior, leapover) {
			EXPECT_TRUE(on_leapover());
		}

		TEST_F(ftfBittleXBehavior, moon_walk) {
			EXPECT_TRUE(on_moon_walk());
		}
		TEST_F(ftfBittleXBehavior, nod) {
			EXPECT_TRUE(on_nod());
		}

		TEST_F(ftfBittleXBehavior, play_dead) {
			EXPECT_TRUE(on_play_dead());
		}

		TEST_F(ftfBittleXBehavior, pee) {
			EXPECT_TRUE(on_pee());
		}
		TEST_F(ftfBittleXBehavior, push_ups) {
			EXPECT_TRUE(on_pushups());
		}
		TEST_F(ftfBittleXBehavior, pushups_one_hand) {
			EXPECT_TRUE(on_pushups_one_hand());
		}

		TEST_F(ftfBittleXBehavior, recover) {
			EXPECT_TRUE(on_recover());
		}

		TEST_F(ftfBittleXBehavior, roll) {
			EXPECT_TRUE(on_roll());
		}

		TEST_F(ftfBittleXBehavior, scratch) {
			EXPECT_TRUE(on_scratch());
		}
		TEST_F(ftfBittleXBehavior, sniff) {
			EXPECT_TRUE(on_sniff());
		}
		// (violent motion)
		TEST_F(ftfBittleXBehavior, table) {
			EXPECT_TRUE(on_table());
		}

		TEST_F(ftfBittleXBehavior, test) {
			EXPECT_TRUE(on_test());
		}

		// (violent motion)
		TEST_F(ftfBittleXBehavior, wave_head) {
			EXPECT_TRUE(on_wave_head());
		}

		TEST_F(ftfBittleXBehavior, zero) {
			EXPECT_TRUE(on_zero_behavior());
		}

		TEST_F(ftfBittleXBehavior, Behavior_Data) {
			using std::this_thread::sleep_for;

			ASSERT_TRUE(on_pushups());	// standard, built-in behavior
			sleep_for(milliseconds(50));	// ~servo lag time
			vector <int8_t> expect_joints{};
			ASSERT_TRUE(on_joint(expect_joints));
			vector <joint_t> pose{
				{HEAD, expect_joints[HEAD]}
				, {LARM, expect_joints[LARM]}
				, {RARM, expect_joints[RARM]}
				, {RHIP, expect_joints[RHIP]}
				, {LHIP, expect_joints[LHIP]}
				, {LFOREARM, expect_joints[LFOREARM]}
				, {RFOREARM, expect_joints[RFOREARM]}
				, {RLEG, expect_joints[RLEG]}
				, {LLEG, expect_joints[LLEG]}
			};
			ASSERT_TRUE(on_zero());

			ASSERT_TRUE(on_behavior_data());
			EXPECT_TRUE(on_verify(pose));
		}
		TEST_F(ftfBittleXBehavior, Behavior_Data_User) {
			const int8_t ang[] = {	// anger
				-7, 0, 0, 1,
				3, 4, 3,
				0,  0,  0,  0,  0,  0,  0,  0, 30, 30, 30, 30, 30, 30, 30, 30,	32, 0, 0, 0,
				-50,  0, 45,  0, -5, -5, 20, 20, -19, 47, 71, 90, -16, -55, 41, 47,	16, 0, 0, 0,
				-50,  0, 45,  0, -5, -5, 20, 20, -70, 60, 85, 19, 76, -68, 41, -31,	32, 0, 0, 0,
				-20,  0, 45,  0, -5, -5, 20, 20,-109, 65, 97, 14, 68, -77, 27, -11,	48, 0, 0, 0,
				-84,  0, 45,  0, -5, -5, 20, 20, -76, 65, 97, 14, 90, -60, 27, -11,	64, 4, 0, 0,
				38, -80, 45,  0, -3, -3,  3,  3, 70, 78, 70, 22, -55, -8, -35, -3,	16, 6, 0, 0,
				0, -80,  0,  0,  0,  0,  0,  0, 30, 30, 30, 30, 30, 30, 30, 30,	16, 0, 0, 0,
			};

			ASSERT_TRUE(on_zero());
			EXPECT_TRUE(on_skill_data(ang, sizeof(ang)));
			// todo: verify pose
		}
	}	// namespace ftBittleX

}	// namespace utPetoi
