//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include "serial/serial.h"

#ifdef GTEST_OS_WINDOWS
#include "windows.h"
#include "strsafe.h"
#endif

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace ::testing;

#ifdef DISABLED_CODE
#else
#endif	// DISABLED_CODE

namespace utPetoi {
	using std::string;
	using std::ostream;
	using std::vector;
	using std::boolalpha;
	using std::noboolalpha;
	using std::hex;
	using std::dec;
	using std::left;
	using std::right;
	using std::setw;
	using std::cout;
	using std::chrono::milliseconds;
	using std::this_thread::sleep_for;

	extern vector <string> commandline;

	// ====================================================
	// utfwin32 - Windows base unit test fixture class
	// ====================================================
	class utfwin32 : public testing::Test
	{
#ifdef GTEST_OS_WINDOWS
	protected:
		DWORD expected_error{ ERROR_SUCCESS };

	public:
		utfwin32()
		{
			// flush errors
			on_error(TEXT(__FUNCTION__));
		}

		~utfwin32()
		{
			// report errors
			on_error(TEXT(__FUNCTION__));
		}
	protected:
		DWORD expect_error(DWORD dw = ERROR_SUCCESS);
		void on_error(LPCTSTR lpszFunction, DWORD dw = ::GetLastError());
#else
		// !GTEST_OS_WINDOWS
		// stub API's
	protected:
		unsigned long expect_error(unsigned long dw) { return 0UL; }
		void on_error(const char*, unsigned long) {}
#endif
	};

	namespace utserial {
		using namespace serial;

		ostream& operator <<(ostream& os, const PortInfo& rport);

		template <typename T>
		ostream& operator <<(ostream& os, const vector <T>& list)
		{
			for (auto val : list) {
				os << val;
			}
			return os;
		}

		ostream& operator <<(ostream& os, const PortInfo& rport);

		// ====================================================
		// utfSerial - test fixture for Serial class
		// ====================================================
		// default values
		const string default_port{ "COM6" };
		const flowcontrol_t default_flowcontrol{ flowcontrol_none };
		const uint32_t default_timeout{ 16U };

		class utfSerial : public utfwin32
		{
		protected:
			// serial port ctor parameters
			string port{ default_port };
			unsigned long baud{ 115200 };
			uint32_t timeout{ default_timeout };
			Timeout ctimeout{ Timeout::simpleTimeout(timeout) };
			bytesize_t bytesize{ eightbits };
			parity_t parity{ parity_none };
			stopbits_t stopbits{ stopbits_one };
			flowcontrol_t flowcontrol{ default_flowcontrol };

			Serial utSerial{};	// serial port under test
			// n.b., Serial default ctor

		public:
			utfSerial()
			{
				EXPECT_TRUE(parse_commandline());
				EXPECT_FALSE(utSerial.isOpen());
			}
			~utfSerial()
			{
				EXPECT_TRUE(close_port());
			}
		protected:
			bool parse_commandline();
			bool open_port();
			bool close_port();
		};

		// ====================================================
		// utfSerialOpen - test fixture for Serial class open port
		// ====================================================
		class utfSerialOpen : public utfSerial
		{
		protected:
		public:
			utfSerialOpen()
			{
				open_port();
			}
			~utfSerialOpen()
			{
				flush();
				// n.b., ~utfSerial calls close_port
			}
		protected:
			bool flush()
			{
				using namespace std::chrono_literals;

				if (!utSerial.isOpen()) {
					return false;
				}
				EXPECT_NO_THROW(utSerial.flush());
				std::this_thread::sleep_for(16ms);

				string reply{};
				size_t count{};	// track iteration count
				while (0 < utSerial.available()) {
					reply += utSerial.read(1);
					++count;
				}
				return 0 == utSerial.available();
			}
		};

		enum Command : uint8_t
		{
			SHUT_DOWN                   // rest and shutdown all servos
			, SWITCH_GYRO               // switch whether to use IMU data
			, PAUSE_MOTION              // TODO: difference between shutdown?
			, CALIBRATE                 // Calibration mode
			, SWITCH_RECOVERING         // TODO: check if this can be disabled
			, REPEAT_SKILL              // repeat last called skill
			, PLAY_CUSTOM_SKILL         // play custom defined skill
			, MOVE_JOINT_SEQUENCE       // move joints in sequence
			, MOVE_JOINTS_SIMULTANEOUS  // move joints all at once
			, SET_JOINTS                // set all joints at once
			, PLAY_MELODY               // play built-in melody
			, PLAY_CUSTOM_SOUND         // play the cutom provided sound

			, STEPPING      // stepping on the same spot
			, BACK          // going back
			, BACK_LEFT     // going back and left
			, BACK_RIGHT    // going back and right
			, CRAWL         // crawl forward
			, CRAWL_LEFT    // crawl left
			, CRAWL_RIGHT   // crawl right
			, WALK          // walk forward
			, WALK_LEFT     // walk left
			, WALK_RIGHT    // walk right
			, TROT          // trot forward
			, TROT_LEFT     // trot left
			, TROT_RIGHT    // trot right
			, BOUND         // not recommended

			, BALANCE       // balancing mode
			, BUTT_UP       // raise hip
			, CALIB_POSE    // Calibration pose, make easy to calibrate joints
			, REST          // rest mode TODO: difference between SHUT_DOWN
			, SIT
			, SLEEP
			, STRETCH
			, ZERO          // template for designing new posture

			, CHECK_AROUND
			, GREETING
			, PEE
			, PUSH_UP
			, MIMIC_DEATH
			, BACK_FLIP
		};

		// alias for command arguments
		using TaskArgs = vector <int16_t>;

		struct Task
		{
			Command cmd;
			TaskArgs arguments;
			float delay;		// in seconds

			Task(Command _cmd)
				: cmd(_cmd)
				, arguments{}
				, delay(0.f)
			{
			}
			Task(Command _cmd, float _delay)
				: cmd(_cmd)
				, arguments{}
				, delay(_delay)
			{
				//delay = 0.f;	// hack
			}
			Task(Command _cmd, TaskArgs _args, float _delay)
				: cmd(_cmd)
				, arguments{ _args.begin(), _args.end() }
				, delay(_delay)
			{
				//delay = 0.f;	// hack
			}
		};
	}	// namespace utserial

	namespace ftBittleX {
		using std::ostream;
		using std::istream;
		using std::istringstream;
		using std::ostringstream;
		using std::chrono::milliseconds;
		using std::chrono::time_point;
		using std::chrono::steady_clock;

		using namespace ::serial;

		const unsigned RX_LATENCY_DEF = 2000U;	// response timeout (ms)

		// an RAII timer
		struct elapse_timer {
			time_point<steady_clock> begin{ steady_clock::now() };
			string label{};

			elapse_timer(const string& lbl = "")
				: label(lbl)
			{
			}
			~elapse_timer()
			{
				//time_point<steady_clock> end{ steady_clock::now() };
				//milliseconds elapsed{ duration_cast<milliseconds>(end - begin) };
				//if (label.empty()) {
				//	label = __FUNCTION__;
				//}
				//cout << left << setw(12) << label
				//	<< ": " << elapsed.count() << " ms\n";
			}
			milliseconds elapsed() const {
				time_point<steady_clock> end{ steady_clock::now() };
				return duration_cast<milliseconds>(end - begin);
			}
		};

		// command identifiers
		// see opencat.h::tokens
		enum cmdid_t {
			QUERY
			, BEEP
			, ABORT
			, BEEP_BIN
			, CALIBRATE
			, REST
			, INDEXED_SIMULTANEOUS_ASC
			, INDEXED_SIMULTANEOUS_BIN
			, JOINTS
			, SKILL
			, SKILL_DATA
			, LISTED_BIN
			, INDEXED_SEQUENTIAL_ASC
			, INDEXED_SEQUENTIAL_BIN
			, PAUSE
			, TASK_QUEUE
			, SAVE
			, TEMP
			, XLEG
			, RANDOM_MIND
			, READ
			, WRITE
			, TYPE_ANALOG
			, TYPE_DIGITAL
			, COLOR
			, PRINT_GYRO
			, VERBOSELY_PRINT_GYRO
			, GYRO_FINENESS
			, GYRO_BALANCE
			, MELODY
			, SLOPE
			, TILT
			, MEOW
			, SERVO_MICROSECOND
			, ACCELERATE
			, DECELERATE
			, RESET

			, SERVO_FEEDBACK
			, TUNER
		};

		// ====================================================
		// servo index
		// ====================================================
		using angle_t = int8_t;	// narrow problem

		const uint8_t HEAD{ 0 };

		const uint8_t LFOREARM{ 12 };
		const uint8_t LARM{ 8 };
		const uint8_t LLEG{ 15 };
		const uint8_t LHIP{ 11 };

		const uint8_t RFOREARM{ 13 };
		const uint8_t RARM{ 9 };
		const uint8_t RLEG{ 14 };
		const uint8_t RHIP{ 10 };

		const angle_t MAX_ANGLE{ 120 };		// should be 125
		const angle_t MIN_ANGLE{ -120 };	// should be -125

		// ====================================================
		// ====================================================
		struct joint_t {
			uint8_t idx;		// servo index
			angle_t angle;		// rotation angle
		};

		struct angle_limit_t
		{
			uint8_t idx;		// servo index
			angle_t min_angle;
			angle_t max_angle;
		};

		extern angle_t min_angle;	// detected servo min
		extern angle_t max_angle;	// detected servo max
		extern vector <angle_limit_t> joint_limit;
		extern int angleLimit[][2];

		// ====================================================
		// ====================================================
		struct note_t {
			uint8_t tone;
			uint8_t duration;
		};

		struct gyro_t {
			float yaw{}, pitch{}, roll{};
			int x{}, y{}, z{}, accel{};
		};

		ostream& operator <<(ostream& os, const gyro_t& rec);
		istream& operator >>(istream& is, gyro_t& rec);

		// ====================================================
		// cmd_def_t - protocol command definition type
		// todo: rework cmd_def_t
		// ====================================================
		struct cmd_def_t {
			cmdid_t id;
			string cmd;
			string description;
			vector <uint8_t> data;		// command parameters (output)
			milliseconds RX_latency{};	// response start latency
			milliseconds RX_elapsed{};	// response elapsed

			// avoid name clash with std library
			bool is_binary() const
			{
				return !cmd.empty()
					&& !islower(cmd[0])
					&& !ispunct(cmd[0])
					;
			}
			bool append_direction(char dir)
			{
				dir = char(toupper(dir));
				switch (dir) {
				default:
					break;
				case 'F':
					description += "_forward";
					data.push_back(dir);
					break;
				case 'R':
					description += "_right";
					data.push_back(dir);
					break;
				case 'L':
					description += "_left";
					data.push_back(dir);
					break;
				case 'X':
					description += "_random";
					data.push_back(dir);
					break;
				}
				return true;
			}

			// ====================================================
			// template operators
			// todo: simplify this via promotion rules
			// ====================================================
			template <typename T>
			cmd_def_t& operator+(const T& val)
			{
				if (cmd.empty()) {
					return *this;
				}
				if (is_binary()) {
					data.push_back(uint8_t(val));
				}
				else {
					ostringstream os{};
					if (data.size()) {
						os << ' ';	// arg separator
					}
					os << int(val);
					string buf{ os.str() };
					for (auto ch : buf) {
						data.push_back(ch);
					}
				}

				return *this;
			}

			template <>
			cmd_def_t& operator+(const char& val)
			{
				if (cmd.empty()) {
					return *this;
				}
				if (is_binary()) {
					data.push_back(uint8_t(val));
				}
				else {
					data.push_back(val);
				}
				return *this;
			}
			template <>
			cmd_def_t& operator+(const string& val)
			{
				for (auto ch : val) {
					*this += ch;
				}
				return *this;
			}
			template <>
			cmd_def_t& operator+(const joint_t& val)
			{
				*this += val.idx;
				*this += val.angle;
				return *this;
			}
			template <>
			cmd_def_t& operator+(const note_t& val)
			{
				*this += val.tone;
				*this += val.duration;
				return *this;
			}

			template <typename T>
			cmd_def_t& operator+=(const T& val)
			{
				return *this + val;
			}
			template <typename T>
			cmd_def_t& operator+=(const vector <T>& list)
			{
				for (auto val : list) {
					*this += val;
				}
				return *this;
			}
			template <>
			cmd_def_t& operator+=(const note_t& val)
			{
				return *this + val;
			}
		};

		ostream& operator <<(ostream& os, const cmd_def_t& command);

		// ====================================================
		// cmd_skill - syntactic sugar class
		// Simplifies creating a skill command
		// ====================================================
		struct cmd_skill : public cmd_def_t
		{
			cmd_skill(string suffix, string desc)
				: cmd_def_t{ SKILL, "k", desc }
			{
				*this += string(suffix);
			}
		};

		struct skill_t
		{
			int8_t frame_count;	// behavior == negative
			// posture == 1
			// ow: gait

// degree; range: [-125, 125]; CW(+), CCW(-)
			int8_t roll;
			int8_t pitch;

			int8_t angle_ratio;	// servo angle multiplier

			skill_t(int8_t frames, int8_t r, int8_t p, int8_t ratio)
				: frame_count{ frames }
				, roll{ r }
				, pitch{ p }
				, angle_ratio{ ratio }
			{
			}
		};

		//ostream& operator <<(ostream& os, const skill_t& skill);
		//istream& operator >>(istream& is, skill_t& rskill);

		typedef vector <int8_t> frame_t;
		typedef vector <frame_t> framelist_t;

		//ostream& operator <<(ostream& os, const frame_t& frame);
		//istream& operator >>(istream& os, frame_t& frame);
		//ostream& operator <<(ostream& os, const framelist_t& skill);

		struct posture_t : public skill_t {
			frame_t frame;		// 16 servo angles

			posture_t(int8_t roll, int8_t pitch, int8_t ratio, const frame_t& angles)
				: skill_t{ 1, roll, pitch, ratio }
				, frame{ angles }
			{
				EXPECT_EQ(16U, frame.size()) << "frame size too small";
				if (frame.size() < 16U) {
					frame.resize(16U);
				}
			}
		};
		istream& operator >>(istream& is, posture_t& rposture);
		ostream& operator <<(ostream& os, const posture_t& posture);

		struct gait_t : public skill_t {
			framelist_t framelist;		// list of 16 servo angles

			gait_t(
				int8_t roll
				, int8_t pitch
				, int8_t ratio
				, const framelist_t& angles
			)
				: skill_t{ int8_t(angles.size()), roll, pitch, ratio }
				, framelist{ angles }
			{
				//for (auto frame : framelist) {
				//	EXPECT_EQ(16U, frame.size()) << "frame size too small";
				//}
			}
		};

		ostream& operator <<(ostream& os, const gait_t& gait);

		struct animation_t {
			// frame numbers are zero based
			int8_t start_frame;
			int8_t end_frame;
			int8_t loop_times;
		};
		istream& operator>>(istream& is, animation_t& ranimation);

		// speed: [1-127] (deg/step)
		// delay: [1-127] (50 ms)
		// trigger_axis:
		//		0  = no trigger
		//		1  = positive pitch (forward)
		//		-1 = negative pitch (backward)
		//		2  = positive roll (left)
		//		-2 = negative roll (right)
		// trigger_angle: [-125-125]
		struct frame_action_t {
			int8_t speed;
			int8_t delay;
			int8_t trigger_axis;
			int8_t trigger_angle;
		};

		typedef vector<frame_action_t> actionlist_t;

		ostream& operator <<(ostream& os, const actionlist_t& actionlist);

		struct behavior_t : public skill_t {
			animation_t animation;		// frame cycles
			framelist_t framelist;		// list of 16 servo angles
			actionlist_t actionlist;	// list of actions for angle

			behavior_t(
				int8_t roll
				, int8_t pitch
				, int8_t ratio
				, animation_t animate
				, const framelist_t& angles
				, actionlist_t actions
			)
				: skill_t{ int8_t(angles.size()), roll, pitch, ratio }
				, animation{ animate }
				, framelist{ angles }
				, actionlist{ actions }
			{
				// size of framelist and actionlist must match
				EXPECT_EQ(actionlist.size(), framelist.size()) << "mismatch: actionlist v. framelist";
				if (actionlist.size() < framelist.size()) {
					actionlist.resize(framelist.size());
				}
			}
		};

		// forward; lazy conversion support
		ostream& operator <<(ostream& os, const behavior_t& behavior);
		istream& operator >>(istream& is, behavior_t& rbehavior);

		struct cmd_skill_data : public cmd_def_t
		{
			template <typename T>
			cmd_skill_data(const T skill)
				: cmd_def_t{ SKILL_DATA, "K", "SKILL_DATA" }
			{
				ostringstream os;
				os << skill;
				string spos{ os.str() };
				vector <int8_t> pdata{ spos.begin(), spos.end() };
				*this += pdata;
			}

#ifdef DISABLED_CODE
			cmd_skill_data(posture_t posture)
				: cmd_def_t{ SKILL_DATA, "K", "SKILL_DATA" }
			{
				// lazy conversion
				ostringstream os;
				os << posture;
				string spos{ os.str() };
				vector <int8_t> pdata{ spos.begin(), spos.end() };
				*this += pdata;
			}
			cmd_skill_data(gait_t gait)
				: cmd_def_t{ SKILL_DATA, "K", "SKILL_DATA" }
			{
				// lazy conversion
				ostringstream os;
				os << gait;
				string spos{ os.str() };
				vector <int8_t> pdata{ spos.begin(), spos.end() };
				*this += pdata;
			}
			cmd_skill_data(behavior_t behavior)
				: cmd_def_t{ SKILL_DATA, "K", "SKILL_DATA" }
			{
				// lazy conversion
				ostringstream os;
				os << behavior;
				string spos{ os.str() };
				vector <int8_t> pdata{ spos.begin(), spos.end() };
				*this += pdata;
			}
#endif	// DISABLED_CODE
		};

		// ====================================================
		// ftfBittleX
		// ====================================================
		using ftfwin32 = utPetoi::utfwin32;

		class ftfBittleX : public ftfwin32
		{
			// serial port ctor parameters
			string port{ "COM5" };
			unsigned long baud{ 115200 };
			//uint32_t timeout{ 10U };	// standard TIMEOUT
			//uint32_t timeout{ 150U };	// TIMEOUT long
			uint32_t timeout{ 128U + 64U };	// TIMEOUT BittleX
			Timeout ctimeout{ Timeout::simpleTimeout(timeout) };
			bytesize_t bytesize{ eightbits };
			parity_t parity{ parity_none };
			stopbits_t stopbits{ stopbits_one };
			flowcontrol_t flowcontrol{ flowcontrol_none };
			//flowcontrol_t flowcontrol{ flowcontrol_hardware };
			Serial ftSerial{};
		protected:
			vector <string> response{};

			string model{};
			string version{};
			bool mute{ true };	// n.b., manual precondition

		public:
			ftfBittleX()
			{
				EXPECT_TRUE(on_connect());
#ifdef DISABLED_CODE
				EXPECT_TRUE(on_getmute());
				if (!mute) {
					EXPECT_TRUE(on_setmute(true));
				}
#endif	// DISABLED_CODE
				if (!HasFailure()) {
					EXPECT_TRUE(on_zero());
				}
			}
			~ftfBittleX()
			{
				if (!HasFailure()) {	// preserve leftover data
					EXPECT_TRUE(on_zero());
				}
				EXPECT_TRUE(on_disconnect());
			}
		protected:
			// ====================================
			// ====================================

			bool on_connect();
			bool on_disconnect();

			// internal api's
			bool on_send(const cmd_def_t& command);
			bool on_response(cmd_def_t& command, unsigned latency = RX_LATENCY_DEF);
			bool on_command(cmd_def_t& command, unsigned latency = RX_LATENCY_DEF);
			bool on_command(const vector <cmd_def_t>& command, unsigned latency = RX_LATENCY_DEF);
			ostream& echo_response(ostream& os);

			bool on_detect_minmax_angle(angle_limit_t& joint);
			bool on_detect_minmax_angle(vector <angle_limit_t>& joint_lim = joint_limit);
			bool on_verify(joint_t& joint, angle_t angle);
			bool on_verify(const vector <joint_t>& list);

			// conversion from array to class
			template <typename T>
			bool on_extract(const int8_t skill[], size_t size, T& type)
			{
				string strskill{ skill, skill + size };
				istringstream is{ strskill };
				is >> type;
				return !is.fail();
			}

			// ====================================
			// protocol API's
			// ====================================
			bool on_query();
			bool on_abort();
			bool on_getmute();
			bool on_setmute(bool on);
			bool on_volume(const int8_t& vol);
			bool on_playtune(const vector <note_t>& tune);
			bool on_calibrate();
			bool on_calibrate(vector <joint_t>& list);
			bool on_calibrate(const joint_t& joint);
			bool on_calibrate(const vector <joint_t>& list);

			bool on_rest();

			// group sequential and simultaneous,
			// ASC and BIN api's
			bool on_joint(uint8_t idx, angle_t& angle);
			bool on_joint(vector <int8_t>& joint);

			bool on_setjointsimul(const uint8_t idx, const angle_t angle);
			bool on_setjointsimul(const vector <joint_t>& list);
			bool on_setjointsimul_bin(const uint8_t idx, const angle_t angle);
			bool on_setjointsimul_bin(const vector <joint_t>& list);
			bool on_setjointseq(const uint8_t idx, const angle_t angle);
			//bool on_setjointseq(const joint_t& joint);	// YAGNI
			bool on_setjointseq(const vector <joint_t>& list);
			bool on_setjointseq_bin(const uint8_t idx, const angle_t angle);
			bool on_setjointseq_bin(const vector <joint_t>& list);

			// skill api's below
			bool on_DOF(vector <angle_t> list);	// todo

			bool on_pause();
			bool on_save();
			bool on_temp(unsigned latency = RX_LATENCY_DEF);

			bool on_xleg();			// obsolete; not implemented
			bool on_random_mind();	// disabled

			bool on_read(uint8_t pin, bool& val);	// digital read
			bool on_read(uint8_t pin, int& val);	// analog read
			bool on_write(uint8_t pin, bool val);	// not implemented
			bool on_write(uint8_t pin, int val);	// not implemented

			bool on_color();
			bool on_print_gyro();
			bool on_print_gyro_verbose();
			bool on_gyro_fineness();	// toggle gyro on/off
			bool on_gyro_balance();
			bool on_melody();
			bool on_slope();
			bool on_tilt();
			bool on_meow();
			bool on_servo_pwm();		// disabled
			bool on_accelerate();		// disabled
			bool on_decelerate();		// disabled
			bool on_reset();			// marks board as uninitialized

			// skill api's

			// ==========================
			// posture
			// ==========================
			bool on_posture(const string& cmd, const string& description);

			bool on_balance() { return on_posture("balance", "balance"); }
			bool on_buttUp() { return on_posture("buttUp", "butt_up"); }
			bool on_calibrate_pose() { return on_posture("calib", "calibrate"); }
			bool on_dropped() { return on_posture("dropped", "dropped"); }
			bool on_lifted() { return on_posture("lifted", "lifted"); }
			bool on_landing() { return on_posture("lnd", "Landing"); }
			bool on_sit() { return on_posture("sit", "Sit"); }
			bool on_stretch() { return on_posture("str", "Stretch"); }
			bool on_standup() { return on_posture("up", "Stand_up"); }
			bool on_zero() { return on_posture("zero", "Zero"); }

			bool on_posture_data(const vector <joint_t>& pose);

			// ==========================
			// gait
			// ==========================
			bool on_bound_forward();	// disabled

			bool on_backward();
			bool on_backward_left();
			bool on_backward_right();
			bool on_backward_random();

			bool on_crawl(char  dir = 'F');
			bool on_crawl_forward() { return on_crawl(); }
			bool on_crawl_left() { return on_crawl('L'); }
			bool on_crawl_right() { return on_crawl('R'); }
			bool on_crawl_random() { return on_crawl('X'); }

			bool on_gap(char dir = 'F');
			bool on_gap_forward() { return on_gap(); }
			bool on_gap_left() { return on_gap('L'); }
			bool on_gap_right() { return on_gap('R'); }
			bool on_gap_random() { return on_gap('X'); }

			bool on_halloween();
			bool on_jump_forward();		// test disabled (violent motion)

			bool on_push(char dir = 'F');
			bool on_push_forward() { return on_push(); }
			bool on_push_left() { return on_push('L'); }
			bool on_push_right() { return on_push('R'); }
			bool on_push_random() { return on_push('X'); }

			bool on_trot(char dir = 'F');
			bool on_trot_forward() { return on_trot(); }
			bool on_trot_left() { return on_trot('L'); }
			bool on_trot_right() { return on_trot('R'); }
			bool on_trot_random() { return on_trot('X'); }

			bool on_step(char dir = 'F');
			bool on_spring_left() { return on_step('L'); }
			bool on_spring_right() { return on_step('R'); }
			bool on_spring_random() { return on_step('X'); }

			bool on_walk(char dir = 'F');
			bool on_walk_forward() { return on_walk(); }
			bool on_walk_left() { return on_walk('L'); }
			bool on_walk_right() { return on_walk('R'); }
			bool on_walk_random() { return on_walk('X'); }

			bool on_gait_data();	// todo

			// ==========================
			// behavior
			// ==========================
			bool on_behavior(const string& cmd, const string& description);

			bool on_angry() { return on_behavior("ang", "angry"); }
			bool on_backflip() { return on_behavior("bf", "backflip"); }
			bool on_boxing() { return on_behavior("bx", "boxing"); }
			bool on_cheers() { return on_behavior("chr", "cheers"); }
			bool on_check() { return on_behavior("ck", "check"); }
			bool on_come_here() { return on_behavior("cmh", "come_here"); }
			bool on_dig() { return on_behavior("dg", "dig"); }
			bool on_frontflip() { return on_behavior("ff", "front_flip"); }
			bool on_high_five() { return on_behavior("fiv", "high_five"); }
			bool on_good_boy() { return on_behavior("gdb", "good_boy"); }
			bool on_handstand() { return on_behavior("hds", "handstand"); }
			bool on_hug() { return on_behavior("hg", "hug"); }
			bool on_hi() { return on_behavior("hi", "hi"); }
			bool on_hand_shake() { return on_behavior("hsk", "hand_shake"); }
			bool on_hands_up() { return on_behavior("hu", "hands_up"); }
			bool on_jump() { return on_behavior("jmp", "jump"); }
			bool on_kick() { return on_behavior("kc", "kick"); }
			bool on_leapover() { return on_behavior("lpov", "leap_over"); }
			bool on_moon_walk() { return on_behavior("mw", "moon_walk"); }
			bool on_nod() { return on_behavior("nd", "nod"); }
			bool on_play_dead() { return on_behavior("pd", "play_dead"); }
			bool on_pee() { return on_behavior("pee", "pee"); }
			bool on_pushups() { return on_behavior("pu", "pushups"); }
			bool on_pushups_one_hand() { return on_behavior("pu1", "pushups_one_hand"); }
			bool on_recover() { return on_behavior("rc", "recover"); }
			bool on_roll() { return on_behavior("rl", "roll"); }
			bool on_scratch() { return on_behavior("scrh", "scratch"); }
			bool on_sniff() { return on_behavior("snf", "sniff"); }
			bool on_table() { return on_behavior("tbl", "be_table"); }
			bool on_test() { return on_behavior("ts", "test"); }
			bool on_wave_head() { return on_behavior("wh", "wave_head"); }
			bool on_zero_behavior() { return on_behavior("zz", "zero"); }

			bool on_behavior_data();

			bool on_skill_data(const int8_t pu[], size_t size);

		private:
			bool parse_commandline();


		private:
			bool on_setjoint(cmd_def_t& command, const uint8_t idx, const angle_t angle);
			bool on_setjoint(cmd_def_t& command, const vector <joint_t>& list);
		};

		// ====================================================
		template <typename R, typename T>
		bool scan_response(const R& response, T& rec)
		{
			return false;
		}

		// convert response R records to T record until success
		template <typename R, typename T>
		bool scan_response(const vector <R>& response, T& rec)
		{
			for (auto ln : response) {
				if (scan_response(ln, rec)) {
					return true;
				}
			}
			return false;
		}

		// copy response elements into list
		template <typename R, typename T>
		bool scan_response(const vector <R>& response, vector <T>& list)
		{
			list.clear();
			for (auto ln : response) {
				T rec{};
				if (scan_response(ln, rec)) {
					list.push_back(rec);
				}
			}
			return 0 < list.size();
		}

		template <>
		bool scan_response(const string& response, gyro_t& rec);
	}	// namespace ftBittleX
}	// namespace utPetoi
