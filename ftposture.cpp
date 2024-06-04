// ====================================================
// posture SKILL command tests
// 11 postures
// rest is tested indirectly
// ====================================================
#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {

		using ftfBittleXPosture = ftfBittleX;

		TEST_F(ftfBittleXPosture, Balance_skill) {
			EXPECT_TRUE(on_balance());
		}
		TEST_F(ftfBittleXPosture, ButtUp_skill) {
			EXPECT_TRUE(on_buttUp());
		}

		TEST_F(ftfBittleXPosture, Dropped) {
			EXPECT_TRUE(on_dropped());
		}
		TEST_F(ftfBittleXPosture, Lifted) {
			EXPECT_TRUE(on_lifted());
		}

		TEST_F(ftfBittleXPosture, Landing) {
			EXPECT_TRUE(on_landing());
		}
		TEST_F(ftfBittleXPosture, Sit) {
			EXPECT_TRUE(on_sit());
		}
		TEST_F(ftfBittleXPosture, Stretch) {
			EXPECT_TRUE(on_stretch());
		}
		TEST_F(ftfBittleXPosture, Standup) {
			EXPECT_TRUE(on_standup());
		}
		TEST_F(ftfBittleXPosture, Zero) {
			EXPECT_TRUE(on_zero());
		}
		TEST_F(ftfBittleXPosture, Posture_Data) {
			using std::this_thread::sleep_for;

			// read stretch joints
			ASSERT_TRUE(on_stretch());
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

			ASSERT_TRUE(on_zero());		// reference pose

			ASSERT_TRUE(on_posture_data(pose));
			sleep_for(milliseconds(50));	// ~servo lag time
			EXPECT_TRUE(on_verify(pose));
		}
		// todo: debug this
		TEST_F(ftfBittleXPosture, Posture_Data_User) {
			using std::this_thread::sleep_for;

			ASSERT_TRUE(on_zero());		// reference pose
			sleep_for(milliseconds(50));	// ~servo lag time

			// n.b., type difference from doc
			const int8_t sit[] = {
				1, 0, -30, 1,
				0, 0, -45, 0, -5, -5, 20, 20
				, 45, 45, 105, 105, 45, 45, -45, -45,
			};

			{
				frame_t frame(16, 0);
				posture_t posture{ 0,0,0,frame };
				EXPECT_TRUE(on_extract(sit, sizeof(sit), posture));
				cmd_skill_data skill_data{ posture };
				EXPECT_TRUE(on_command(skill_data));

				// verify pose
				vector <joint_t> pose{
					{HEAD, posture.frame[HEAD]}
					, {LARM, posture.frame[LARM]}
					, {RARM, posture.frame[RARM]}
					, {RHIP, posture.frame[RHIP]}
					, {LHIP, posture.frame[LHIP]}
					, {LFOREARM, posture.frame[LFOREARM]}
					, {RFOREARM, posture.frame[RFOREARM]}
					, {RLEG, posture.frame[RLEG]}
					, {LLEG, posture.frame[LLEG]}
				};
				EXPECT_TRUE(on_verify(pose));
			}
		}
	}	// namespace ftBittleX
}	// namespace utPetoi
