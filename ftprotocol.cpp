// ====================================================
// Serial protocol tests
// ====================================================
#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {

		const vector <note_t> music{
			{ 20, 4 }, { 22, 4 }, { 24, 4 }, { 15, 4 }
			, { 20, 4 }, { 22, 8 }, { 24, 1 }, { 22, 4 }
			, { 20, 4 }, { 22, 4 }, { 27, 4 }, { 27, 4 }
			, { 27, 4 }, { 27, 2 }, { 20, 4 }, { 19, 4 }
			, { 20, 4 }, { 20, 4 }, { 20, 4 }, { 20, 4 }
			, { 20, 2 }, { 19, 4 }, { 20, 4 }, { 19, 4 }
			, { 20, 4 }, { 19, 4 }, { 17, 4 }, { 15, 2 }
			, { 15, 4 }, { 15, 4 }, { 17, 4 }, { 17, 4 }
			, { 17, 4 }, { 17, 4 }, { 17, 2 }, { 15, 4 }
			, { 12, 4 }, { 15, 4 }, { 12, 4 }, { 15, 4 }
			, { 22, 4 }, { 20, 2 }
			, { uint8_t(0), 4 }
			, { 15, 4 }, { 24, 4 }, { 24, 4 }, { 24, 4 }
			, { 25, 4 }, { 27, 4 }, { 20, 4 }, { 20, 4 }
			, { 24, 4 }, { 22, 1 }, { 22, 1 }
			, { uint8_t(0), 2 }
		};

		using ftfBittleXProtocol = ftfBittleX;

		TEST_F(ftfBittleXProtocol, CR_only) {
			cmd_def_t command{ QUERY, "\r", "CR_only" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result);
		}
		TEST_F(ftfBittleXProtocol, LF_only) {
			cmd_def_t command{ QUERY, "\n", "LF_only" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result);
		}
		TEST_F(ftfBittleXProtocol, CRLF_only) {
			cmd_def_t command{ QUERY, "\r", "CRLF_only" };
			command += '\n';
			bool result{ on_command(command) };
			EXPECT_TRUE(result);
		}
		TEST_F(ftfBittleXProtocol, invalid) {
			cmd_def_t t_sound{ BEEP, "S", "T_SOUND" };
			EXPECT_FALSE(on_command(t_sound));
		}

		// ====================================================
		// protocol
		// ====================================================
		TEST_F(ftfBittleXProtocol, QUERY) {
			EXPECT_TRUE(on_query());
		}
		// n.b., affect audio response
		TEST_F(ftfBittleXProtocol, ABORT) {
			EXPECT_TRUE(on_abort());
		}

		// ====================================================
		// n.b., the BEEP tests are disabled to reduce BittleX
		// EEPROM read/write operations.
		// ====================================================
		TEST_F(ftfBittleXProtocol, DISABLED_BEEP_volume) {
			EXPECT_TRUE(on_volume(3));
		}
		TEST_F(ftfBittleXProtocol, DISABLED_BEEP_getmute) {
			EXPECT_TRUE(on_getmute());
		}
		TEST_F(ftfBittleXProtocol, DISABLED_BEEP_setmute) {
			ASSERT_TRUE(on_getmute());

			EXPECT_TRUE(on_setmute(false));
			EXPECT_FALSE(mute);
			EXPECT_TRUE(on_setmute(true));
			EXPECT_TRUE(mute);
		}
		TEST_F(ftfBittleXProtocol, DISABLED_BEEP_tune) {
			bool old_mute{ mute };
			on_setmute(false);
			EXPECT_TRUE(on_playtune(music));
			on_setmute(old_mute);
		}
		// ====================================================

		// n.b., affects voice response
		TEST_F(ftfBittleXProtocol, CALIBRATE) {
			EXPECT_TRUE(on_calibrate());
			EXPECT_EQ(5, response.size());
			ASSERT_TRUE(on_abort());
		}
		TEST_F(ftfBittleXProtocol, CALIBRATE_twice) {
			EXPECT_TRUE(on_calibrate());
			EXPECT_EQ(5, response.size());
			EXPECT_TRUE(on_calibrate());
			EXPECT_EQ(5, response.size());
			ASSERT_TRUE(on_abort());
		}

		TEST_F(ftfBittleXProtocol, CALIBRATE_read) {
			vector <joint_t> list{};
			EXPECT_TRUE(on_calibrate(list));
			ASSERT_TRUE(on_abort());
		}

		TEST_F(ftfBittleXProtocol, CALIBRATE_joint) {
			joint_t joint{LARM, 8};
			EXPECT_TRUE(on_calibrate(joint));
			EXPECT_EQ(5, response.size());
			ASSERT_TRUE(on_abort());
		}
		TEST_F(ftfBittleXProtocol, CALIBRATE_joint_bylist) {
			vector <joint_t> list{
				{HEAD, 1}
				, {LARM, 9}
				, {RARM, 10}
				, {RHIP, 11}
				, {LHIP, 12}
				, {LFOREARM, 13}
				, {RFOREARM, 14}
				, {RLEG, 15}
				, {LLEG, 16}
			};
			for (auto joint : list) {
				EXPECT_TRUE(on_calibrate(joint));
				EXPECT_LE(4, response.size());	// may have "calib" lins
				//sleep_for(milliseconds(50));
			}
			ASSERT_TRUE(on_abort());
		}

		TEST_F(ftfBittleXProtocol, CALIBRATE_list) {
			const vector <joint_t> list{
				{HEAD, 1}
				, {LARM, 9}
				, {RARM, 10}
				, {RHIP, 11}
				, {LHIP, 12}
				, {LFOREARM, 13}
				, {RFOREARM, 14}
				, {RLEG, 15}
				, {LLEG, 16}
			};
			EXPECT_TRUE(on_calibrate(list));
			EXPECT_LE(list.size() * 3, response.size());
			ASSERT_TRUE(on_abort());
		}

		TEST_F(ftfBittleXProtocol, REST) {
			EXPECT_TRUE(on_rest());
		}

		TEST_F(ftfBittleXProtocol, INDEXED_SIMULTANEOUS_ASC_angle) {

			ASSERT_TRUE(on_detect_minmax_angle());
			joint_t joint{ HEAD, -90 };
			EXPECT_TRUE(on_setjointsimul(joint.idx, joint.angle));
			sleep_for(milliseconds(50));	// ~servo lag time
			EXPECT_TRUE(on_verify(joint, -90));
			EXPECT_TRUE(on_setjointsimul(joint.idx, 0));
		}

		TEST_F(ftfBittleXProtocol, INDEXED_SIMULTANEOUS_ASC_list) {
			ASSERT_TRUE(on_detect_minmax_angle());
			ASSERT_TRUE(on_zero());

			vector <joint_t> hips{
				{LARM, 45}
				, {RARM, 45}
				, {RHIP, 45}
				, {LHIP, 45}
			};
			ASSERT_TRUE(on_setjointsimul(hips));
			sleep_for(milliseconds(50));	// ~servo lag time
			ASSERT_TRUE(on_verify(hips));

			vector <joint_t> legs{
				{LFOREARM, 0}
				, {RFOREARM, 0}
				, {RLEG, 0}
				, {LLEG, 0}
			};
			for (int8_t angle = min_angle; angle <= max_angle; ++angle) {
				for (auto& joint : legs) {
					joint.angle = angle;
				}
				ASSERT_TRUE(on_setjointsimul(legs));
				ASSERT_TRUE(on_verify(legs));
			}

			//ASSERT_TRUE(on_zero());
		}

		TEST_F(ftfBittleXProtocol, JOINTS_list) {
			vector <int8_t> joint{};
			EXPECT_TRUE(on_joint(joint));
			EXPECT_FALSE(joint.empty());
			cout << "joint angles:\n";
			for (auto angle : joint) {
				cout << int(angle) << ' ';
			}
			cout << '\n';
		}

		TEST_F(ftfBittleXProtocol, JOINTS_index) {
			uint8_t idx{ HEAD };
			angle_t angle{};
			EXPECT_TRUE(on_joint(idx, angle));
			cout << "joint angle[" << int(idx) << "] :"
				<< int(angle) << "\n";
		}

		// same as Kata Head_sweep
		// 240 degree sweep
		TEST_F(ftfBittleXProtocol, JOINTS_index_sweep) {
			joint_t joint{ HEAD };
			for (angle_t angle = MIN_ANGLE; angle <= MAX_ANGLE; ++angle) {
				EXPECT_TRUE(on_setjointseq(HEAD, angle));
				if (!on_verify(joint, angle)) {
					break;
				}
			}
			ASSERT_TRUE(on_setjointseq(HEAD, 0));
		}

		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_ASC_angle) {
			joint_t joint{ HEAD, 90 };
			EXPECT_TRUE(on_setjointseq(joint.idx, int8_t(joint.angle)));
			EXPECT_TRUE(on_verify(joint, 90));
			EXPECT_TRUE(on_setjointseq(joint.idx, 0));
		}

		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_BIN_angle) {
			joint_t joint{ HEAD, 45 };
			EXPECT_TRUE(on_setjointseq_bin(joint.idx, joint.angle));
			EXPECT_TRUE(on_verify(joint, 45));
			ASSERT_TRUE(on_setjointseq_bin(joint.idx, 0));
		}

		// api: on_setjointseq
		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_ASC_list) {
			ASSERT_TRUE(on_detect_minmax_angle());
			ASSERT_TRUE(on_zero());

			vector <joint_t> hips{
				{LARM, 45}
				, {RARM, 45}
				, {RHIP, 45}
				, {LHIP, 45}
			};
			ASSERT_TRUE(on_setjointseq(hips));
			sleep_for(milliseconds(50));	// ~servo lag time
			ASSERT_TRUE(on_verify(hips));

			vector <joint_t> legs{
				{LFOREARM}
				, {RFOREARM}
				, {RLEG}
				, {LLEG}
			};
			for (angle_t angle = min_angle; angle <= max_angle; ++angle) {
				for (auto& def : legs) {
					def.angle = angle;
				}
				ASSERT_TRUE(on_setjointseq(legs));
				ASSERT_TRUE(on_verify(legs));
			}

			//ASSERT_TRUE(on_zero());
		}

		// test angle > 90
		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_ASC_OWL) {
			vector <joint_t> owl{
				{HEAD, 0}
				, {HEAD, 30}
				, {HEAD, 60}
				, {HEAD, 90}
				, {HEAD, 60}
				, {HEAD, 30}
				, {HEAD, 0}
				, {HEAD, -30}
				, {HEAD, -60}
				, {HEAD, -90}
				, {HEAD, -60}
				, {HEAD, -30}
				, {HEAD, 0}
			};
			EXPECT_TRUE(on_setjointseq(owl));
			joint_t head{ HEAD, 0 };
			EXPECT_TRUE(on_verify(head, 0));
		}
		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_BIN_list) {
			ASSERT_TRUE(on_detect_minmax_angle());
			ASSERT_TRUE(on_zero());

			vector <joint_t> hips{
				{LARM, 45}
				, {RARM, 45}
				, {RHIP, 45}
				, {LHIP, 45}
			};
			ASSERT_TRUE(on_setjointseq_bin(hips));
			sleep_for(milliseconds(50));	// ~servo lag time
			EXPECT_TRUE(on_verify(hips));

			vector <joint_t> legs{
				{LFOREARM}
				, {RFOREARM}
				, {RLEG}
				, {LLEG}
			};
			for (angle_t angle = min_angle; angle <= max_angle; ++angle) {
				for (auto& joint : legs) {
					joint.angle = angle;
				}
				ASSERT_TRUE(on_setjointseq_bin(legs));
				EXPECT_TRUE(on_verify(legs));
			}

			//ASSERT_TRUE(on_zero());
		}

		// api: on_setjointsimul
		TEST_F(ftfBittleXProtocol, INDEXED_SIMULTANEOUS_BIN_angle) {
			joint_t joint{ HEAD, 45 };
			EXPECT_TRUE(on_setjointsimul_bin(joint.idx, 45));
			sleep_for(milliseconds(50));	// ~servo lag time
			EXPECT_TRUE(on_verify(joint, 45));
			ASSERT_TRUE(on_setjointsimul_bin(joint.idx, 0));
		}
		TEST_F(ftfBittleXProtocol, INDEXED_SIMULTANEOUS_BIN_list) {
			ASSERT_TRUE(on_detect_minmax_angle());
			ASSERT_TRUE(on_zero());

			vector <joint_t> hips{
				{LARM, 45}
				, {RARM, 45}
				, {RHIP, 45}
				, {LHIP, 45}
			};
			ASSERT_TRUE(on_setjointsimul_bin(hips));
			sleep_for(milliseconds(50));	// ~servo lag time
			ASSERT_TRUE(on_verify(hips));

			vector <joint_t> legs{
				{LFOREARM}
				, {RFOREARM}
				, {RLEG}
				, {LLEG}
			};
			for (angle_t angle = min_angle; angle <= max_angle; ++angle) {
				for (auto& def : legs) {
					def.angle = angle;
				}
				ASSERT_TRUE(on_setjointsimul_bin(legs));
				ASSERT_TRUE(on_verify(legs));
			}

			//ASSERT_TRUE(on_zero());
		}

		// api: on_setjointseq
		TEST_F(ftfBittleXProtocol, INDEXED_SEQUENTIAL_ASC_list_repeat) {
			ASSERT_TRUE(on_zero());
			ASSERT_TRUE(on_setjointseq(LHIP, 45));
			ASSERT_TRUE(on_setjointseq(RHIP, 45));

			vector <joint_t> leg_repeat{
				{LFOREARM, 0}
				, {RFOREARM, 0}
				, {RLEG, 0}
				, {LLEG, 0}

				, {LFOREARM, -30 }
				, {RFOREARM, -30}
				, {RLEG, -30}
				, {LLEG, -30}

				, {LFOREARM, -60}
				, {RFOREARM, -60}
				, {RLEG, -60}
				, {LLEG, -60}

				, {LFOREARM, -90}
				, {RFOREARM, -90}
				, {RLEG, -90}
				, {LLEG, -90}

				, {LFOREARM, -60}
				, {RFOREARM, -60}
				, {RLEG, -60}
				, {LLEG, -60}

				, {LFOREARM, -30}
				, {RFOREARM, -30}
				, {RLEG, -30}
				, {LLEG, -30}

				, {LFOREARM, 0}
				, {RFOREARM, 0}
				, {RLEG, 0}
				, {LLEG, 0}
			};
			EXPECT_TRUE(on_setjointseq(leg_repeat));
			//ASSERT_TRUE(on_zero());
		}

		TEST_F(ftfBittleXProtocol, COLOR) {
			EXPECT_TRUE(on_color());
		}

		// test VERBOSELY_PRINT_GYRO toggles ON/OFF
		TEST_F(ftfBittleXProtocol, VERBOSELY_PRINT_GYRO) {
			EXPECT_TRUE(on_print_gyro_verbose());	// toggle ON

			{
				elapse_timer test_time{};
				milliseconds elapsed{ test_time.elapsed() };
				while (elapsed < milliseconds(100)) {
					elapsed = test_time.elapsed();
				}
				EXPECT_TRUE(on_print_gyro_verbose());	// toggle OFF
			}

			cout << "lines received: " << response.size() << "\n";
			vector <gyro_t> list;
			if (scan_response(response, list)) {
				const int wth{ 8 };
				cout << setw(wth) << "yaw"
					<< setw(wth) << "pitch"
					<< setw(wth) << "roll"
					<< setw(wth) << "x"
					<< setw(wth) << "y"
					<< setw(wth) << "z"
					<< setw(wth) << "accel"
					<< '\n'
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< setw(wth) << "--------"
					<< '\n'; ;
				for (auto rec : list) {
					cout << rec;
				}
			}
		}

		TEST_F(ftfBittleXProtocol, PRINT_GYRO) {
			EXPECT_TRUE(on_print_gyro());
			gyro_t rec;
			scan_response(response, rec);
			cout << rec;
		}
		TEST_F(ftfBittleXProtocol, GYRO_BALANCE) {
			EXPECT_TRUE(on_gyro_balance());
		}

		TEST_F(ftfBittleXProtocol, LISTED_BIN) {
			vector <int8_t> all_servos{
				20, 0, 0, 0
				, 0, 0, 0, 0
				, 5, 0, 45, 45
				, 80, 80, 36, 36
			};
			EXPECT_TRUE(on_DOF(all_servos));
		}

		TEST_F(ftfBittleXProtocol, PAUSE) {
			EXPECT_TRUE(on_pause());
		}

#pragma message("todo: What is default TEMP skill?")
		TEST_F(ftfBittleXProtocol, TEMP) {
			unsigned latency{ 5 * RX_LATENCY_DEF };
			EXPECT_TRUE(on_temp(latency));
		}

		TEST_F(ftfBittleXProtocol, DISABLED_RANDOM_MIND) {
			EXPECT_TRUE(on_random_mind());
		}

		TEST_F(ftfBittleXProtocol, DIGITAL_READ) {
			const uint8_t IR_PIN{ 23 };
			bool val{};
			bool result{ on_read(IR_PIN, val) };
			EXPECT_TRUE(result);
			if (result) {
				cout << left << setw(12) << "IR value" << ": "
					<< val << "\n";
			}
		}
		TEST_F(ftfBittleXProtocol, DISABLED_DIGITAL_WRITE) {
		}

		TEST_F(ftfBittleXProtocol, ANALOG_READ) {
			const uint8_t VOLTAGE_PIN{ 4 };
			int voltage{};
			bool result{ on_read(VOLTAGE_PIN, voltage) };
			EXPECT_TRUE(result);
			if (result) {
				cout << left << setw(12) << "Voltage" << ": "
					<< voltage << "\n";
			}
		}
		TEST_F(ftfBittleXProtocol, DISABLED_ANALOG_WRITE) {
		}

		// n.b., block EEPROM writes
		TEST_F(ftfBittleXProtocol, DISABLED_SAVE) {
			EXPECT_TRUE(on_save());
		}

		TEST_F(ftfBittleXProtocol, GYRO_FINENESS) {
			EXPECT_TRUE(on_gyro_fineness());
		}
		TEST_F(ftfBittleXProtocol, DISABLED_MELODY) {
			bool old_mute{ mute };
			ASSERT_TRUE(on_setmute(false));
			EXPECT_TRUE(on_melody());
			ASSERT_TRUE(on_setmute(old_mute));
		}
		TEST_F(ftfBittleXProtocol, SLOPE) {
			EXPECT_TRUE(on_slope());
		}
		TEST_F(ftfBittleXProtocol, TILT) {
			EXPECT_TRUE(on_tilt());
		}
		TEST_F(ftfBittleXProtocol, MEOW) {
			EXPECT_TRUE(on_meow());
		}
		TEST_F(ftfBittleXProtocol, DISABLED_SERVO_PWM) {
			EXPECT_TRUE(on_servo_pwm());
		}
		TEST_F(ftfBittleXProtocol, DISABLED_ACCELERATE) {
			EXPECT_TRUE(on_accelerate());
		}
		TEST_F(ftfBittleXProtocol, DISABLED_DECELERATE) {
			EXPECT_TRUE(on_decelerate());
		}

		TEST_F(ftfBittleXProtocol, on_command_list) {
			vector <cmd_def_t> list{
				{ JOINTS, "j", "JOINTS" }
				, { PRINT_GYRO, "v", "PRINT_GYRO" }
			};
			EXPECT_TRUE(on_command(list));
		}

		// ====================================================
		// example.py - sample from python code
		// ====================================================
		TEST_F(ftfBittleXProtocol, DISABLED_example_py) {
			on_gyro_fineness();     // toggle gyro on/off
			//on_random_mind();     // turn off the random behavior
			on_gyro_balance();      // stand normally

			on_setjointseq(HEAD, -50);

			vector < joint_t > test_pose{
				{LARM, -5}
				, {LARM, 10}
				, {LARM, -5}
				, {LARM, 10}
				, {LARM, -5}
				, {LARM, 10}
				, {LARM, -5}
				, {LARM, 10}
				, {LARM, -5}
				, {LARM, 10}
				, {LARM, -5}
				, {LARM, 10}
			};
			on_setjointseq(test_pose);

			test_pose.clear();
			// method 1: manually add elements
			test_pose.push_back(joint_t{ LARM, -15 });
			test_pose.push_back(joint_t{ RARM, -20 });
			on_setjointsimul(test_pose);

			on_rest();

			on_calibrate();
			on_setjointseq(RARM, -9);
			on_abort();

			// method 2: copy a list
			{
				vector <joint_t> new_pose{
					{LARM, -20}
					, {HEAD, 0}
					, {RARM, -20}
					, {RHIP, 20}
					, {LHIP, 20}
					, {RLEG, 80}
					, {LLEG, 80}
				};
				test_pose = new_pose;
				on_setjointseq_bin(test_pose);
			}
			{
				vector <joint_t> new_pose{
					{LARM, 20}
					, {HEAD, 40}
					, {RARM, -20}
					, {RHIP, 20}
					, {LHIP, 50}
					, {RLEG, 60}
					, {LLEG, 60}
				};
				test_pose = new_pose;
				on_setjointseq_bin(test_pose);
			}
		}
		TEST_F(ftfBittleXProtocol, DISABLED_ASC_newCmd_overflow) {
			const unsigned BUFF_LEN{ 2507 };  // 1524 =125*20+7=2507
			// n.b.,
			// ea. element digit is 1 byte
			// ea. element has space suffix
			//vector <int8_t> symphony(BUFF_LEN / 3, 32);	// no overflow
			vector <int8_t> symphony(BUFF_LEN / 3 + 1, 32);	// overflow
			cmd_def_t command{ BEEP, "b", "BEEP" };
			command += symphony;
			unsigned latency{ 14000 };
			EXPECT_TRUE(on_command(command, latency));

			if (!HasFailure()) {	// process overflow standup command
				// uncomment to leave standup response in buffer
				//using std::this_thread::sleep_for;
				//sleep_for(milliseconds(100));	// standup 

				cmd_def_t cmd_standup{ SKILL, "k", "SKILL" };
				cmd_standup += string("up");
				EXPECT_TRUE(on_response(cmd_standup));
			}
		}
		TEST_F(ftfBittleXProtocol, DISABLED_BIN_newCmd_overflow) {
			const unsigned BUFF_LEN{ 2507 };  // 1524 =125*20+7=2507
			vector <int8_t> symphony(BUFF_LEN - 1, 32);	// malformed
			//vector <int8_t> symphony(BUFF_LEN, 32);	// no overflow
			//vector <int8_t> symphony(BUFF_LEN + 1, 32);	// overflow
			cmd_def_t command{ BEEP_BIN, "B", "BEEP_BIN" };
			command += symphony;
			unsigned latency{ 14000 };
			EXPECT_TRUE(on_command(command, latency));

			if (!HasFailure()) {	// process overflow standup command
				// uncomment to leave standup response in buffer
				//using std::this_thread::sleep_for;
				//sleep_for(milliseconds(100));	// standup 

				cmd_def_t cmd_standup{ SKILL, "k", "SKILL" };
				cmd_standup += string("up");
				EXPECT_TRUE(on_response(cmd_standup));
			}
		}
		TEST_F(ftfBittleXProtocol, DISABLED_SKILL_DATA_newCmd_overflow) {
			const unsigned BUFF_LEN{ 2507 };  // 1524 =125*20+7=2507
			vector <int8_t> nonsense(BUFF_LEN + 1, 32);	// overflow
			cmd_def_t command{ SKILL_DATA, "K", "SKILL_DATA" };
			command += nonsense;
			unsigned latency{ 1000 };
			EXPECT_TRUE(on_command(command, latency));

			if (!HasFailure()) {	// process overflow standup command
				// uncomment to leave standup response in buffer
				//using std::this_thread::sleep_for;
				//sleep_for(milliseconds(100));	// standup 

				cmd_def_t cmd_standup{ SKILL, "k", "SKILL" };
				cmd_standup += string("up");
				EXPECT_TRUE(on_response(cmd_standup));
			}
		}

		TEST_F(ftfBittleXProtocol, detect_minmax_angle) {
			vector <angle_limit_t> jlim{
				{HEAD}
				, {RLEG}
				, {LLEG}
			};
			EXPECT_TRUE(on_detect_minmax_angle(jlim));
			min_angle = angle_t(INT8_MAX);
			max_angle = angle_t(INT8_MAX);
			EXPECT_EQ(jlim[0].min_angle, angleLimit[HEAD][0]);
			EXPECT_GE(jlim[0].max_angle, angleLimit[HEAD][1]);
			EXPECT_EQ(jlim[1].min_angle, angleLimit[RLEG][0]);
			EXPECT_GE(jlim[1].max_angle, angleLimit[RLEG][1]);
			EXPECT_EQ(jlim[2].min_angle, angleLimit[LLEG][0]);
			EXPECT_GE(jlim[2].max_angle, angleLimit[LLEG][1]);
		}

		// defect	: LLEG fails to move to test pose.
		// test		: pause for servo cool down time
		// requires	: visual check of LLEG nove
		TEST_F(ftfBittleXProtocol, servo_collision) {
			vector <joint_t> test_pose{
				{ LHIP, 45 }
				, { LLEG, 90 }
			};
			ASSERT_TRUE(on_setjointsimul(test_pose));
			ASSERT_TRUE(on_verify(test_pose));

			angle_t angle{ -90 };	// body collision
			joint_t joint{ LLEG, angle };
			ASSERT_TRUE(on_setjointsimul(joint.idx, joint.angle));
			angle = angle_t(angleLimit[LLEG][0]);	// clipping
			bool clipped{ on_verify(joint, angle) };
			EXPECT_TRUE(clipped);

			EXPECT_TRUE(on_setjointsimul(test_pose));
			EXPECT_TRUE(on_verify(test_pose));
			if (clipped) {
				cout << "\nWaiting for servo...\n\n";
				sleep_for(milliseconds(2500));
			}
		}
	}	// namespace ftBittleX
}	// namespace utPetoi
