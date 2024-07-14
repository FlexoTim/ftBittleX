//
// ftBittleX.cpp
//

#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {
		// ====================================================
		// useful common variables
		// ====================================================
		const angle_t DEF_ANGLE{ angle_t(INT8_MAX) };	// truncation warning
		angle_t min_angle{ DEF_ANGLE };
		angle_t max_angle{ DEF_ANGLE };
		vector <angle_limit_t> joint_limit{
			{HEAD}
			, {LFOREARM}, {LARM}
			, {RFOREARM}, {RARM}
			, {LLEG}, {LHIP}
			, {RLEG}, {RHIP}
		};
		int angleLimit[][2] = {
			{ -120, 120 }
			, { -85, 50 }
			, { -120, 120 }
			, { -120, 120 }
			, { -90, 60 }
			, { -90, 60 }
			, { -90, 90 }
			, { -90, 90 }
			, { -200, 80 }
			, { -200, 80 }
			, { -80, 200 }
			, { -80, 200 }
			, { -80, 80 }
			, { -80, 80 }
			, { -70, 80 }
			, { -80, 80 }
		};

		vector <cmd_def_t> cmdlist{
			{QUERY, "?", "QUERY" }
			, {REST, "d", "REST" }
			, {JOINTS, "j", "JOINTS" }
			, {BEEP, "b", "BEEP" }
			, {ABORT, "a", "ABORT" }
			, {BEEP_BIN, "B", "BEEP_BIN" }
			, {CALIBRATE, "c", "CALIBRATE" }
			, {INDEXED_SIMULTANEOUS_ASC, "i", "INDEXED_SIMULTANEOUS_ASC" }
			, {INDEXED_SIMULTANEOUS_BIN, "I", "INDEXED_SIMULTANEOUS_BIN" }
			, {SKILL, "k", "SKILL" }
			, {SKILL_DATA, "K", "SKILL_DATA" }
			, {LISTED_BIN, "L", "LISTED_BIN" }
			, {INDEXED_SEQUENTIAL_ASC, "m", "INDEXED_SEQUENTIAL_ASC" }
			, {INDEXED_SEQUENTIAL_BIN, "M", "INDEXED_SEQUENTIAL_BIN" }
			, {PAUSE, "p", "PAUSE" }
			//, {TASK_QUEUE, "TASK_QUEUE" }
			, {SAVE, "s", "SAVE" }
			, {TEMP, "T", "TEMP" }
			//, {XLEG, "x", "XLEG" }
			, {RANDOM_MIND, "z", "RANDOM_MIND" }
			, {READ, "R", "READ" }
			, {WRITE, "W", "WRITE" }
			//, {TYPE_ANALOG, "a", "TYPE_ANALOG" }
			//, {TYPE_DIGITAL, "d", "TYPE_DIGITAL" }
			//, {COLOR, "C", "COLOR" }
			, {PRINT_GYRO, "v", "PRINT_GYRO" }
			, {VERBOSELY_PRINT_GYRO, "V", "VERBOSELY_PRINT_GYRO" }
			, {GYRO_FINENESS, "g", "GYRO_FINENESS" }
			, {GYRO_BALANCE, "G", "GYRO_BALANCE" }
			, {MELODY, "o", "MELODY" }
			//, {SLOPE, "l", "SLOPE" }
			//, {TILT, "t", "TILT" }
			//, {MEOW, "u", "MEOW" }
			//, {SERVO_MICROSECOND, "w", "SERVO_MICROSECOND" }
			//, {ACCELERATE, ".", "ACCELERATE" }
			//, {DECELERATE, ",", "DECELERATE" }
			, {RESET, "!", "RESET" }

			//, {SERVO_FEEDBACK, "SERVO_FEEDBACK" }
			//, {TUNER, "TUNER" }
		};

		// ====================================================
		// output operators for various types
		// ====================================================
		ostream& operator <<(ostream& os, const gyro_t& rec)
		{
			const int wth{ 8 };
			// columns
			os << setw(wth) << rec.yaw
				<< setw(wth) << rec.pitch
				<< setw(wth) << rec.roll
				<< setw(wth) << rec.x
				<< setw(wth) << rec.y
				<< setw(wth) << rec.z
				<< setw(wth) << rec.accel
				<< '\n';

			return os;
		}

		istream& operator>>(istream& is, gyro_t& rec)
		{
			is >> rec.yaw >> rec.pitch >> rec.roll
				>> rec.x >> rec.y >> rec.z
				>> rec.accel;
			return is;
		}

		ostream& operator <<(ostream& os, const skill_t& skill)
		{
			os << skill.frame_count
				<< skill.roll
				<< skill.pitch
				<< skill.angle_ratio
				;

			return os;
		}
		istream& operator >>(istream& is, skill_t& rskill)
		{
			is >> rskill.frame_count
				>> rskill.roll
				>> rskill.pitch
				>> rskill.angle_ratio;

			return is;
		}

		ostream& operator <<(ostream& os, const frame_t& frame)
		{
			for (auto angle : frame) {
				os << angle;
			}
			return os;
		}
		istream& operator >>(istream& is, frame_t& frame)
		{
			frame.resize(16, 0);
			for (auto& angle : frame) {
				is >> angle;
			}
			return is;
		}

		ostream& operator <<(ostream& os, const cmd_def_t& command)
		{
			os << left << setw(12) << "description" << ": " << command.description << '\n'
				<< left << setw(12) << "cmd" << ": " << command.cmd << '\n'
				<< left << setw(12) << "id" << ": " << command.id
				<< '\n'
				;
			if (!command.data.empty()) {
				os << left << setw(12) << "data" << ": ";
				if (command.is_binary()) {
					for (auto dat : command.data) {
						os << int(dat);
					}
				}
				else {
					for (auto dat : command.data) {
						if (isgraph(dat)) {
							os << char(dat);
						}
						else {
							os << hex << dat << dec;
						}
					}
				}
				os << '\n';
			}

			// "find" friendly format, i.e., numric values first
			cout << right << setw(8) << command.RX_latency.count()
				<< " ms : RX_latency\n"
				<< right << setw(8) << command.RX_elapsed.count()
				<< " ms : RX_elapsed\n";

			return os;
		}

		ostream& operator <<(ostream& os, const posture_t& posture)
		{
			const skill_t& rskill{ posture };
			os << rskill
				<< posture.frame;
			return os;
		}
		istream& operator >>(istream& is, posture_t& rposture)
		{
			skill_t& rskill{ rposture };
			is >> rskill
				>> rposture.frame;
			return is;
		}

		ostream& operator <<(ostream& os, const gait_t& gait)
		{
			const skill_t& rskill{ gait };
			os << rskill;

			for (auto frame : gait.framelist) {
				for (auto angle : frame) {
					os << angle;
				}
			}
			return os;
		}

		ostream& operator <<(ostream& os, const animation_t& animation)
		{
			os << animation.start_frame
				<< animation.end_frame
				<< animation.loop_times;
			return os;
		}
		ostream& operator <<(ostream& os, const frame_action_t& action)
		{
			os << action.speed
				<< action.delay
				<< action.trigger_axis
				<< action.trigger_angle;
			return os;
		}
		ostream& operator <<(ostream& os, const actionlist_t& actionlist)
		{
			for (auto action : actionlist) {
				os << action;
			}
			return os;
		}

		ostream& operator <<(ostream& os, const behavior_t& behavior)
		{
			// can't use the default operator because
			// behavior requires a negative frame count
			os << int8_t(-behavior.frame_count)
				<< behavior.roll
				<< behavior.pitch
				<< behavior.angle_ratio;

			os << behavior.animation.start_frame
				<< behavior.animation.end_frame
				<< behavior.animation.loop_times;

			// output each frame followed by animation data
			for (int idxf = 0; idxf < behavior.framelist.size(); ++idxf) {
				os << behavior.framelist[idxf];
				os << behavior.actionlist[idxf];
			}

			return os;
		}

		// ====================================================
		// ====================================================
		template <>
		bool scan_response(const string& response, gyro_t& rec)
		{
			if (response.length() <= 3) {
				return false;
			}
			istringstream is{ response };
			is >> rec;
			return !is.fail();
		}

		// ====================================================
		// ====================================================
		bool ftfBittleX::parse_commandline()
		{
			for (auto arg : commandline) {
				string param{ "--port=" };
				size_t pos{ arg.find(param) };
				if (string::npos != pos) {
					pos += param.length();
					port = arg.substr(pos);
					continue;
				}
				param = "--baud=";
				pos = arg.find(param);
				if (string::npos != pos) {
					pos += param.length();
					baud = atol(arg.substr(pos).c_str());
					continue;
				}
			}
			return true;
		}

		bool ftfBittleX::on_connect()
		{
			EXPECT_TRUE(parse_commandline());

			EXPECT_NO_THROW(ftSerial.setBaudrate(baud));
			EXPECT_NO_THROW(ftSerial.setTimeout(ctimeout));
			EXPECT_NO_THROW(ftSerial.setBytesize(bytesize));
			EXPECT_NO_THROW(ftSerial.setParity(parity));
			EXPECT_NO_THROW(ftSerial.setStopbits(stopbits));
			EXPECT_NO_THROW(ftSerial.setFlowcontrol(flowcontrol));
			EXPECT_NO_THROW(ftSerial.setPort(port));	// setPort can reopen a port
			EXPECT_FALSE(ftSerial.isOpen());
			EXPECT_NO_THROW(ftSerial.open());
			EXPECT_TRUE(ftSerial.isOpen());
			EXPECT_NO_THROW(ftSerial.flush());

			// todo: why is this needed (?)
			sleep_for(milliseconds(1));

			// todo: must manually flush
			size_t count{ ftSerial.available() };
			if (count) {
				EXPECT_FALSE(ftSerial.read(count).empty());
				EXPECT_EQ(0U, ftSerial.available());
			}

			return ftSerial.isOpen();
		}

		bool ftfBittleX::on_disconnect()
		{
			EXPECT_EQ(0U, ftSerial.available());
			if (ftSerial.available()) {
				cout << __FUNCTION__ " unread data:\n"
					<< ftSerial.read(ftSerial.available())
					;
			}
			EXPECT_NO_THROW(ftSerial.close());
			return !ftSerial.isOpen();
		}
		bool ftfBittleX::on_send(const cmd_def_t& command)
		{
			cout << __FUNCTION__ "\n"
				//<< command
				;

			string outbuf{ command.cmd };
			char EOL{ command.is_binary() ? '~' : '\n' };
			if (!command.data.empty()) {
				ostringstream os{};
				for (auto val : command.data) {
					os << val;
				}
				outbuf += os.str();
			}
			outbuf += EOL;
			cout << left << setw(12) << "TX command" << ": ";
			if (command.is_binary()) {
				for (int ch : outbuf) {
					cout << hex << ch << dec;
				}
			}
			else {
				for (auto ch : outbuf) {
					cout << ch;
				}
			}
			//cout << outbuf;
			if (command.is_binary()) {
				cout << '\n';
			}

			cout << setw(12) << "write count" << ": " << outbuf.length() << '\n';
			size_t write_count{ ftSerial.write(outbuf) };
			on_error(__FUNCTION__);
			bool result{ outbuf.length() == write_count };
			EXPECT_TRUE(result) << "ftSerial.write FAILED "
				<< "expected: " << outbuf.length()
				<< " actual: " << write_count;

#ifdef _DEBUG
			cout << left << setw(12)
				<< "expected" << ": " << outbuf.length() << '\n'
				<< left << setw(12)
				<< "actual" << ": " << write_count << '\n';
#endif
			return result;
		}

		bool ftfBittleX::on_response(cmd_def_t& command, unsigned latency)
		{
			cout << __FUNCTION__ "\n";

			bool result{ false };
			response.clear();

			// from reaction():
			// except SKILL ...,
			// postures, gaits and other tokens can confirm
			// completion by sending the token back
			//
			// SKILL appears to echo twicw

			{
				command.RX_latency = milliseconds(0);
				elapse_timer RXlatency("RX latency");	// response delay time
				while (!ftSerial.available()
					&& RXlatency.elapsed() < milliseconds(latency)
					)
				{
					// busy work
					//command.RX_latency = RXlatency.elapsed();
					;
				}
				command.RX_latency = RXlatency.elapsed();
				if (milliseconds(latency).count() <= command.RX_latency.count()) {
					EXPECT_GT(milliseconds(latency).count(), command.RX_latency.count())
						<< "RX latency timeout!";
					//cout << __FUNCTION__ " RX latency timeout!\n"
					//	<< command;
					return result;	// timeout
				}
			}

			{
				size_t EOL_needed{
					('k' == tolower(command.cmd[0]))		// skill
					? 2U
					: ('c' == tolower(command.cmd[0]))	// calibrate
						? 2U
						: 1U
				};
				size_t EOL_count{ EOL_needed };
				string EOLstr{ command.cmd[0] };
				EOLstr += "\r\n";
				string EOLtoggle{ EOLstr };
				EOLtoggle[0] = islower(EOLtoggle[0])
					? char(toupper(EOLtoggle[0]))
					: char(tolower(EOLtoggle[0]));
				command.RX_elapsed = milliseconds(0);
				elapse_timer RXelapsed("RX elapsed");
				while (ftSerial.available()
					|| RXelapsed.elapsed() < milliseconds(latency)
					)
				{
					string line{ ftSerial.readline() };
					on_error(__FUNCTION__);

					if (line.empty()) {	// ignore empty lines
						continue;
					}

					EXPECT_NE("Undefined token!\r\n", line);

					response.push_back(line);

					result = (EOLstr == line);
					if (result) {
						if (--EOL_count) {
							continue;
						}
						cout << "Command completed: normal\n";
						break;
					}
					result = (EOLtoggle == line);
					if (result) {
						if (--EOL_count) {
							continue;
						}
						cout << "Command completed: toggle\n";
						break;
					}
					EOL_count = EOL_needed;		// rest EOL count;
				}
				command.RX_elapsed = RXelapsed.elapsed();
				result = result && (command.RX_elapsed < milliseconds(latency));
				if (milliseconds(latency) <= command.RX_elapsed) {
					cout << __FUNCTION__ " RX elapsed timeout!\n";
				}
				//if (result) {
				//	// wait an additional time for servo lag
				//	milliseconds servo_lag{ RXelapsed.elapsed() + milliseconds(96) };
				//	while (RXelapsed.elapsed() < servo_lag) {
				//		;
				//	}
				//}
			}
			cout << command;
			echo_response(cout);

			return result;
		}
		ostream& ftfBittleX::echo_response(ostream& os)
		{
			os << left << setw(12) << "response" << ": "
				<< response.size() << " lines\n";
			for (auto& line : response) {
				if (line.empty()) {
					os << "empty line\n";
				}
				else {
					os << line;
				}
			}
			os << left << setw(12) << "response" << ": end\n";
			return os;
		}

		bool ftfBittleX::on_detect_minmax_angle(angle_limit_t& jointlim)
		{
			vector <joint_t> test_pose{
				// standard pose
				//{HEAD, 0}
				//, { LFOREARM, 0 }, { LARM, 0 }
				//, { RFOREARM, 0 }, { RARM, 0 }
				//, { LLEG, 90 }, { LHIP, 90 }
				//, { RLEG, 90 }, { RHIP, 90 }

				// experimental pose
				{ HEAD, 0 }
				, { LFOREARM, 90 }, { LARM, 0 }
				, { RFOREARM, 90 }, { RARM, 0 }
				, { LLEG, 90 }, { LHIP, 45 }
				, { RLEG, 90 }, { RHIP, 45 }
			};
			// set test pose
			EXPECT_TRUE(on_setjointsimul(test_pose));
			EXPECT_TRUE(on_verify(test_pose));

			// move to angle 90
			// verify max angle
			//angle_t angle{ (HEAD == jointlim.idx) ? MAX_ANGLE : 90 };
			angle_t angle{ MAX_ANGLE };	// 120 degrees should be acheivable
			joint_t joint{ jointlim.idx, angle };
			EXPECT_TRUE(on_setjointseq(jointlim.idx, angle));
			// don't assert
			bool unclipped{ on_verify(joint, angle) };
			jointlim.max_angle = joint.angle;

			// move to angle -90
			// verify min angle
			angle = (HEAD == jointlim.idx) ? MIN_ANGLE : -90;
			joint.angle = angle;
			EXPECT_TRUE(on_setjointseq(jointlim.idx, angle));
			// don't assert
			unclipped = unclipped && on_verify(joint, angle);
			jointlim.min_angle = joint.angle;

			// restore test pose
			EXPECT_TRUE(on_setjointsimul(test_pose));
			EXPECT_TRUE(on_verify(test_pose));
			if (!unclipped) {
				// defect: clipped angles imply collision
				// allow servo cool down time
				// servo should catch up
				sleep_for(milliseconds(2500));	// pause
			}

			//return !HasFailure();
			return true;
		}

		bool ftfBittleX::on_detect_minmax_angle(vector <angle_limit_t>& joint_lim)
		{
			if ((DEF_ANGLE != min_angle)
				&& (DEF_ANGLE != max_angle)) {
				return true;
			}

			min_angle = -120;
			max_angle = 120;
			for (auto& joint : joint_lim) {
				EXPECT_TRUE(on_detect_minmax_angle(joint));
				if (HasFailure()) {
					return false;
				}
				min_angle = max(min_angle, joint.min_angle);
				max_angle = min(max_angle, joint.max_angle);
			}

			cout << __FUNCTION__ "\n"
				<< setw(12) << "servo"
				<< setw(12) << "min angle"
				<< setw(12) << "max angle"
				<< "\n";

			for (auto joint : joint_lim) {
				cout << setw(12) << int(joint.idx)
					<< setw(12) << int(joint.min_angle)
					<< setw(12) << int(joint.max_angle) << "\n";
			}
			cout << setw(12) << "min angle" << ": "
				<< int(min_angle) << "\n"
				<< setw(12) << "max angle" << ": "
				<< int(max_angle) << "\n"
				<< "\n";

			return true;
		}

		// match input angle with joint.angle
		// n.b., may fail due to servo lag time
		bool ftfBittleX::on_verify(joint_t& joint, angle_t angle)
		{
			// read joint angle
			EXPECT_TRUE(on_joint(joint.idx, joint.angle));
			//EXPECT_EQ(joint.angle, angle) << "joint mismatch";
			if (joint.angle != angle) {
				cout << "ANGLE MISMATCH: "
					<< int(angle) << " != " << int(joint.angle)
					<< "\n";
			}
			return (joint.angle == angle);	// allow test to continue
		}
		bool ftfBittleX::on_verify(const vector <joint_t>& list)
		{
			cout << __FUNCTION__ "\n";

			vector <int8_t> actual_joints{};
			bool result{ on_joint(actual_joints) };
			EXPECT_TRUE(result);
			if (result) {
				for (auto joint : list) {
					//EXPECT_EQ(joint.angle, actual_joints[joint.idx]);
					bool bmatch{ joint.angle == actual_joints[joint.idx] };
					result = result && bmatch;
				}
			}

			cout << __FUNCTION__ ": joint list "
				<< (result ? "match" : "mismatch")
				<< "\n";

			return result;
		}

		// ====================================================
		// commands
		// ====================================================

		// generic command api's
		bool ftfBittleX::on_command(cmd_def_t& command, unsigned latency)
		{
			bool result{ on_send(command) };
			EXPECT_TRUE(result) << "on_send failed";
			result = result && on_response(command, latency);
			return result;
		}
		bool ftfBittleX::on_command(const vector <cmd_def_t>& list, unsigned latency)
		{
			bool result{ true };
			for (auto cmd : list) {
				result = result && on_command(cmd, latency);
				if (!result) {
					break;
				}
			}
			return result;
		}

		bool ftfBittleX::on_setjoint(cmd_def_t& command, const uint8_t idx, const angle_t angle)
		{
			// todo: command.id valid?
			command += idx;
			command += angle;

			bool result{ on_command(command) };
			return result;
		}
		bool ftfBittleX::on_setjoint(cmd_def_t& command, const vector <joint_t>& list)
		{
			command.data.clear();
			command += list;

			unsigned int latency{ unsigned int(list.size()) * 1000U };
			bool result{ on_command(command, latency) };
			return result;
		}

		// protocol: INDEXED_SEQUENTIAL_ASC
		bool ftfBittleX::on_setjointseq(const uint8_t idx, const angle_t angle)
		{
			cmd_def_t command{ INDEXED_SEQUENTIAL_ASC, "m", "INDEXED_SEQUENTIAL_ASC" };
			return on_setjoint(command, idx, angle);
		}

		// documentation order
		bool ftfBittleX::on_query()
		{
			cmd_def_t command{ QUERY, "?", "QUERY" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result);
			if (!result) {
				return result;
			}
			EXPECT_LE(3, response.size());
			// 'G' response prefix
			bool prefix{ 3 < response.size() };
			size_t idx{ prefix ? 1U : 0U };
			model = response[idx].substr(0, response[idx].length() - 2);
			version = response[idx + 1].substr(0, response[idx + 1].length() - 2);
			cout << left << setw(10) << "model" << ": " << model << '\n'
				<< left << setw(10) << "version" << ": " << version << '\n';
			result = result && !(model.empty() || version.empty());
			return result;
		}

		bool ftfBittleX::on_abort()
		{
			cmd_def_t command{ ABORT, "a", "ABORT" };
			return on_command(command);
		}

		// reading mute toggles the state
		bool ftfBittleX::on_getmute() {
			cmd_def_t command{ BEEP, "b", "BEEP" };
			bool result{ on_command(command) };
			EXPECT_GE(3U, response.size()) << "format error!";
			if (!response.empty()) {
				mute = (string::npos != response[0].find("Muted"));
				cout << "mute: " << boolalpha << mute << noboolalpha << '\n';
				EXPECT_TRUE(on_setmute(!mute));
			}

			return result;
		}

		bool ftfBittleX::on_setmute(bool on)
		{
			if (on == mute) {
				return true;
			}
			// toggle the mute state on
			cmd_def_t command{ BEEP, "b", "BEEP" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result);
			// todo: response must have "Unmute"
			mute = result;
			cout << "mute: " << boolalpha << mute << noboolalpha << '\n';
			return result;
		}

		bool ftfBittleX::on_volume(const int8_t& vol)
		{
			cmd_def_t command{ BEEP, "b", "BEEP" };
			command += vol;

			bool result{ on_command(command) };
			EXPECT_TRUE(result);
			EXPECT_GE(3U, response.size());

			// todo: side effect
			mute = (0 == vol);
			cout << "mute: " << boolalpha << mute << noboolalpha << '\n';

			return result;
		}

		bool ftfBittleX::on_playtune(const vector <note_t>& tune)
		{
			cmd_def_t command{ BEEP, "b", "BEEP" };
			for (auto note : tune) {
				command += note;
			}

			unsigned int late{ unsigned(tune.size()) * RX_LATENCY_DEF };
			bool result{ on_command(command, late) };
			EXPECT_TRUE(result);

			return result;
		}

		bool ftfBittleX::on_calibrate()
		{
			cmd_def_t command{ CALIBRATE, "c", "CALIBRATE" };
			return on_command(command);
		}
		bool ftfBittleX::on_calibrate(vector <joint_t>& list)
		{
			list.clear();
			bool result{ on_calibrate() };
			result = result && (5U == response.size());
			if (!result) {
				return result;
			}
			// calib
			// <joint index list>
			// <angle>,...<angle>,
			// c
			// c
			istringstream is{ response[2] };
			uint8_t idx{ 0 };
			while (is) {
				angle_t angle;
				char comma;
				is >> angle >> comma;
				if (is.fail()) {
					break;
				}
				joint_t joint{ idx++, angle};
				list.push_back(joint);
			}
			result = (list.size() == idx);
			return result;
		}

		bool ftfBittleX::on_calibrate(const joint_t& joint)
		{
			cmd_def_t command{ CALIBRATE, "c", "CALIBRATE" };
			command += joint;
			return on_command(command);
		}
		bool ftfBittleX::on_calibrate(const vector <joint_t>& list )
		{
			cmd_def_t command{ CALIBRATE, "c", "CALIBRATE" };
			// first put into calibarate mode
			bool result{ on_command(command) };
			if (!result) {
				return result;
			}

			// now send list
			command += list;
			result = result && on_command(command);

			return result;
		}

		bool ftfBittleX::on_rest()
		{
			cmd_def_t command{ REST, "d", "REST" };
			return on_command(command, 2000);
		}

		// read a servo angle
		bool ftfBittleX::on_joint(uint8_t idx, angle_t& angle)
		{
			angle = 0;
			cmd_def_t command{ JOINTS, "j", "Joints" };
			command += idx;

			bool result{ on_command(command, 2 * RX_LATENCY_DEF) };
			EXPECT_TRUE(result) << "on_command failed";

			result = result && (1 < response.size());
			result = result && ("=\r\n" == response[0]);
			if (!result) {
				return result;
			}

			istringstream is{ response[1] };
			int ival;				// must read as int
			is >> ival;
			angle = angle_t(ival);	// conert to angle

			return result;
		}

		// read all servo angles
		bool ftfBittleX::on_joint(vector <int8_t>& joint)
		{
			joint.clear();
			cmd_def_t command{ JOINTS, "j", "JOINTS" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result) << "on_command failed";

			// search for prefix line "=" followed by list
			size_t idx{};
			for (auto line : response) {
				if ("=\r\n" == line) {
					idx += 2;	// list location
					break;
				}
				++idx;
			}
			result = result && (idx < response.size());
			EXPECT_TRUE(result) << __FUNCTION__ << ": response format error";
			if (!result) {
				return result;
			}
			istringstream is{ response[idx] };
			while (is) {
				int angle;
				char comma;
				is >> angle >> comma;
				if (is.fail()) {
					break;
				}
				joint.push_back(int8_t(angle));
			}

			return result;
		}

		// set a servo angle; simultaneous 
		bool ftfBittleX::on_setjointsimul(const uint8_t idx, const angle_t angle)
		{
			cmd_def_t command{ INDEXED_SIMULTANEOUS_ASC, "i", "INDEXED_SIMULTANEOUS_ASC" };
			return on_setjoint(command, idx, angle);
		}
		// set list of servo angles; sequential 
		bool ftfBittleX::on_setjointsimul(const vector <joint_t>& list)
		{
			cmd_def_t command{ INDEXED_SIMULTANEOUS_ASC, "i", "INDEXED_SIMULTANEOUS_ASC" };
			return on_setjoint(command, list);
		}

		// set a servo angle; simultaneous, binary
		bool ftfBittleX::on_setjointsimul_bin(const uint8_t idx, const angle_t angle)
		{
			cmd_def_t command{ INDEXED_SIMULTANEOUS_BIN, "I", "INDEXED_SIMULTANEOUS_BIN" };
			return on_setjoint(command, idx, angle);
		}
		// set list of servo angles; simultaneous, binary
		bool ftfBittleX::on_setjointsimul_bin(const vector <joint_t>& list)
		{
			cmd_def_t command{ INDEXED_SIMULTANEOUS_BIN, "I", "INDEXED_SIMULTANEOUS_BIN" };
			return on_setjoint(command, list);
		}

		// set list of servo angles; sequential
		bool ftfBittleX::on_setjointseq(const vector <joint_t>& list)
		{
			cmd_def_t command{ INDEXED_SEQUENTIAL_ASC, "m", "INDEXED_SEQUENTIAL_ASC" };
			return on_setjoint(command, list);
		}

		// set servo angle; sequential, binary
		bool ftfBittleX::on_setjointseq_bin(const uint8_t idx, const angle_t angle)
		{
			cmd_def_t command{ INDEXED_SEQUENTIAL_BIN, "M", "INDEXED_SEQUENTIAL_BIN" };
			return on_setjoint(command, idx, angle);
		}
		// set list of servo angles; sequential, binary
		bool ftfBittleX::on_setjointseq_bin(const vector <joint_t>& list)
		{
			cmd_def_t command{ INDEXED_SEQUENTIAL_BIN, "M", "INDEXED_SEQUENTIAL_BIN" };
			return on_setjoint(command, list);
		}

		bool ftfBittleX::on_DOF(vector <angle_t> list)
		{
			cmd_def_t command{ LISTED_BIN, "L", "LISTED_BIN" };
			for (auto angle : list) {
				command += angle;
			}
			return on_command(command);
		}

		// todo: pause echos 'P' on command completion
		// acts like a toggle; next comand flips it
		bool ftfBittleX::on_pause()
		{
			cmd_def_t command{ PAUSE, "p", "PAUSE" };
			bool result{ on_send(command) };
			EXPECT_TRUE(result);
			command.cmd = "P";	// hack: change for response
			result = result && on_response(command);
			EXPECT_TRUE(result);
			return result;
		}

		bool ftfBittleX::on_save()
		{
			cmd_def_t command{ SAVE, "s", "SAVE" };
			return on_command(command);
		}

		bool ftfBittleX::on_temp(unsigned latency)
		{
			cmd_def_t command{ TEMP, "T", "TEMP" };

			// hack: switch to SKILL response
			// set timeout to 10 seconds
			//return on_command(command);

			bool result{ on_send(command) };
			EXPECT_TRUE(result);
			command.cmd = "k";	// hack: change for response
			result = result && on_response(command, latency);
			EXPECT_TRUE(result);
			return result;
		}

		bool ftfBittleX::on_random_mind()
		{
			cmd_def_t command{ RANDOM_MIND, "z", "RANDOM_MIND" };
			return on_command(command, 2 * RX_LATENCY_DEF);
		}

		bool ftfBittleX::on_read(uint8_t pin, bool& val)
		{
			cmd_def_t command{ READ, "R", "READ_digital" };
			command += 'd';
			command += pin;
			bool result{ on_command(command) };
			// n.b., positional dependency
			result = result && (3 <= response.size());
			result = result && ("=\r\n" == response[0]);
			result = result && ("R\r\n" == response[2]);

			EXPECT_TRUE(result);	// trace breadcrumb
			if (!result) {
				return result;
			}

			// read response value
			istringstream is{ response[1] };
			is >> val;

			return result;
		}
		bool ftfBittleX::on_read(uint8_t pin, int& val)
		{
			cmd_def_t command{ READ, "R", "READ_analog" };
			command += 'a';
			command += pin;
			bool result{ on_command(command) };
			// n.b., positional dependency
			result = result && (3 <= response.size());
			result = result && ("=\r\n" == response[0]);
			result = result && ("R\r\n" == response[2]);
			EXPECT_TRUE(result);	// trace breadcrumb
			if (!result) {
				return result;
			}

			// read response value
			istringstream is{ response[1] };
			is >> val;

			return result;
		}
#ifdef DISABLED_CODE
		bool ftfBittleX::on_write(uint8_t pin, bool val);	// not implemented
		bool ftfBittleX::on_write(uint8_t pin, int val);	// not implemented
#endif	// DISABLED_CODE

		bool ftfBittleX::on_color()
		{
			cmd_def_t command{ COLOR, "C", "COLOR" };
			return on_command(command);
		}

		bool ftfBittleX::on_print_gyro()
		{
			cmd_def_t command{ PRINT_GYRO, "v", "PRINT_GYRO" };
			bool result{ on_command(command) };
			EXPECT_TRUE(result);

#pragma message("todo: read gyro values")
			gyro_t rec;
			scan_response(response, rec);

			return result;
		}
		bool ftfBittleX::on_print_gyro_verbose()
		{
			cmd_def_t command{ VERBOSELY_PRINT_GYRO, "V", "VERBOSELY_PRINT_GYRO" };
			return on_command(command);
		}
		// todo: gyro_fineness echos 'G' on command completion
		bool ftfBittleX::on_gyro_fineness()
		{
			cmd_def_t command{ GYRO_FINENESS, "g", "GYRO_FINENESS" };
			return on_command(command);
		}

		// GYRO_BALANCE
		bool ftfBittleX::on_gyro_balance()
		{
			cmd_def_t command{ GYRO_BALANCE, "G", "GYRO_BALANCE" };
			return on_command(command);
		}

		bool ftfBittleX::on_melody()
		{
			cmd_def_t command{ MELODY, "o", "MELODY" };
			return on_command(command, 10000);
		}
#pragma message("todo: What is SLOPE response?")
		bool ftfBittleX::on_slope()
		{
			cmd_def_t command{ SLOPE, "l", "SLOPE" };
			return on_command(command);
		}

		bool ftfBittleX::on_tilt()
		{
			cmd_def_t command{ TILT, "t", "TILT" };
			return on_command(command);
		}

		bool ftfBittleX::on_meow()
		{
			cmd_def_t command{ MEOW, "u", "MEOW" };
			return on_command(command);
		}

		bool ftfBittleX::on_servo_pwm()
		{
			cmd_def_t command{ SERVO_MICROSECOND, "w", "SERVO_MICROSECOND" };
			return on_command(command);
		}

		bool ftfBittleX::on_accelerate()
		{
			cmd_def_t command{ ACCELERATE, ".", "ACCELERATE" };
			return on_command(command);
		}
		bool ftfBittleX::on_decelerate()
		{
			cmd_def_t command{ DECELERATE, ",", "DECELERATE" };
			return on_command(command);
		}
		bool ftfBittleX::on_reset()
		{
			cmd_def_t command{ RESET, "!", "RESET" };
			return on_command(command);
		}

		// ====================================================
		// skill posture
		// ====================================================
		using cmd_posture = cmd_skill;

		bool ftfBittleX::on_posture(const string& cmd, const string& description)
		{
			cmd_posture command{ cmd, description };
			return on_command(command);
		}

		bool ftfBittleX::on_posture_data(const vector <joint_t>& pose)
		{
			const int DOF{ 16 };	// todo: use OpenCat::DOF
			frame_t frame(DOF, 0);
			for (auto joint : pose) {
				// todo: range check joint.idx
				if (DOF <= joint.idx) {
					return false;
				}
				frame[joint.idx] = joint.angle;
			}
			posture_t posture{ 0, 0, 1, frame };
			cmd_skill_data skill_data{ posture };
			return on_command(skill_data);
		}

		// ====================================================
		// skill gait
		// ====================================================
	//#pragma message("todo: collapse gait code")
		using cmd_gait = cmd_skill;

		bool ftfBittleX::on_bound_forward()
		{
			cmd_gait command{ "bdF", "bound forward" };
			return on_command(command);
		}
		bool ftfBittleX::on_backward()
		{
			cmd_gait command{ "bk", "backward" };
			return on_command(command);
		}
		bool ftfBittleX::on_backward_left()
		{
			cmd_gait command{ "bk", "backward" };
			command.append_direction('L');
			return on_command(command);
		}
		bool ftfBittleX::on_backward_right()
		{
			cmd_gait command{ "bk", "backward" };
			command.append_direction('R');
			return on_command(command);
		}
		bool ftfBittleX::on_backward_random()
		{
			cmd_gait command{ "bk", "backward" };
			command.append_direction('X');
			return on_command(command);
		}

		bool ftfBittleX::on_crawl(char dir)
		{
			cmd_gait command{ "cr", "crawl" };
			command.append_direction(dir);
			return on_command(command);
		}
		bool ftfBittleX::on_gap(char dir)
		{
			cmd_gait command{ "gp", "gap" };
			command.append_direction(dir);
			return on_command(command);
		}

		bool ftfBittleX::on_halloween()
		{
			cmd_gait command{ "hlw", "halloween" };
			return on_command(command);
		}

		// (violent motion)
		bool ftfBittleX::on_jump_forward()
		{
			cmd_gait command{ "jpF", "jump forward" };
			return on_command(command);
		}

		bool ftfBittleX::on_push(char dir)
		{
			cmd_gait command{ "ph", "push" };
			command.append_direction(dir);
			return on_command(command);
		}

		bool ftfBittleX::on_trot(char dir)
		{
			cmd_gait command{ "tr", "trot forward" };
			command.append_direction(dir);
			return on_command(command);
		}

		bool ftfBittleX::on_step(char dir)
		{
			cmd_gait command{ "vt", "step" };
			command.append_direction(dir);
			return on_command(command);
		}

		bool ftfBittleX::on_walk(char dir)
		{
			cmd_gait command{ "wk", "walk" };
			command.append_direction(dir);
			return on_command(command);
		}

		bool ftfBittleX::on_gait_data()
		{
			// extracted from InstinctBittleESP.h (bk - backwards)
			// todo: how to simplfy long lists of angles?
			// todo: the gait spec uses only servos 8-15;
			// inconsistent list size
			framelist_t framelist
			{
				{{ 36, 40, 36, 62,  6, -3,  6,  1, }}
				, {{ 35, 44, 34, 64,  6, -3,  7,  2, }}
				, {{ 34, 47, 32, 63,  7, -4,  7,  4, }}
				, {{ 31, 50, 31, 61,  8, -4,  8,  8, }}
				, {{ 30, 53, 28, 59,  8, -3,  9,  9, }}
				, {{ 28, 55, 26, 57,  9, -3, 11, 10, }}
				, {{ 26, 58, 25, 57, 10, -2, 10, 10, }}
				, {{ 25, 58, 24, 56, 10,  0,  9,  9, }}
				, {{ 22, 57, 26, 55, 12,  2,  6,  8, }}
				, {{ 20, 55, 27, 54, 13,  5,  3,  7, }}
				, {{ 18, 51, 29, 52, 14,  8,  2,  7, }}
				, {{ 16, 51, 33, 52, 16,  8, -1,  6, }}
				, {{ 15, 51, 36, 50, 15,  6, -2,  6, }}
				, {{ 16, 49, 39, 49, 12,  6, -3,  6, }}
				, {{ 17, 48, 43, 47,  9,  5, -3,  5, }}
				, {{ 18, 47, 46, 46,  8,  5, -4,  5, }}
				, {{ 21, 45, 49, 44,  5,  5, -4,  5, }}
				, {{ 25, 44, 52, 44,  4,  5, -3,  5, }}
				, {{ 29, 43, 55, 42,  2,  5, -3,  5, }}
				, {{ 32, 41, 57, 40,  0,  5, -2,  5, }}
				, {{ 35, 39, 60, 38, -1,  6, -1,  6, }}
				, {{ 39, 37, 61, 36, -3,  6,  0,  6, }}
				, {{ 42, 36, 63, 35, -3,  6,  1,  6, }}
				, {{ 45, 34, 64, 34, -4,  7,  2,  7, }}
				, {{ 49, 32, 62, 31, -4,  7,  6,  8, }}
				, {{ 51, 31, 60, 30, -4,  8,  9,  8, }}
				, {{ 54, 28, 58, 28, -3,  9, 10,  9, }}
				, {{ 57, 28, 57, 25, -2,  9, 10, 12, }}
				, {{ 57, 26, 57, 24,  0, 10,  9, 11, }}
				, {{ 57, 24, 56, 25,  2, 11,  8,  8, }}
				, {{ 56, 21, 54, 26,  3, 12,  8,  4, }}
				, {{ 53, 19, 53, 27,  7, 13,  7,  3, }}
				, {{ 51, 17, 52, 31,  8, 15,  6,  1, }}
				, {{ 51, 15, 51, 34,  8, 17,  6, -1, }}
				, {{ 50, 15, 49, 38,  6, 14,  6, -2, }}
				, {{ 49, 16, 48, 41,  6, 12,  5, -3, }}
				, {{ 47, 18, 47, 44,  5,  8,  5, -3, }}
				, {{ 46, 19, 45, 48,  5,  7,  5, -4, }}
				, {{ 45, 24, 44, 51,  5,  4,  5, -4, }}
				, {{ 44, 27, 43, 53,  5,  3,  5, -3, }}
				, {{ 42, 30, 41, 56,  5,  1,  5, -3, }}
				, {{ 40, 34, 39, 58,  5, -1,  6, -2, }}
				, {{ 38, 37, 37, 60,  6, -2,  6, -1, }}
			};
			gait_t gait{ 0, 0, 1, framelist };
			cmd_skill_data skill_data{ gait };
			return on_command(skill_data);
		}

		// ====================================================
		// skill behavior
		// ====================================================
		using cmd_behavior = cmd_skill;

		// n.b., note the default latency time
		bool ftfBittleX::on_behavior(const string& cmd, const string& description)
		{
			const unsigned int DEF_LATENCY{ 15000 };
			cmd_behavior command{ cmd, description };
			return on_command(command, DEF_LATENCY);
		}

		bool ftfBittleX::on_behavior_data()
		{
			// the format is not clear.
			// it appears that servos 0-7 are used.
			// Does a frame contain only elements for servos 8-15"

			// push-ups
			const int8_t pu[] = {
				-10, 0, 0, 1,
				7,   8, 3,
				0,   0,  0,  0,  0,  0,  0,  0, 30, 30, 30, 30, 30, 30, 30, 30,		8, 0, 0, 0,
				15,  0,  0,  0,  0,  0,  0,  0, 30, 35, 40, 21, 50, 15, 15, 41,		12, 0, 0, 0,
				15,  0,  0,  0,  0,  0,  0,  0, 30, 35, 40, 30, 50, 15, 15, 14,		16, 0, 0, 0,
				30,  0,  0,  0,  0,  0,  0,  0, 27, 35, 40, 60, 50, 15, 20, 45,		16, 0, 0, 0,
				15,  0,  0,  0,  0,  0,  0,  0, 42, 35, 40, 60, 25, 20, 20, 60,		12, 0, 0, 0,
				0,   0,  0,  0,  0,  0,  0,  0, 48, 45, 75, 60, 20, 37, 20, 60,		12, 0, 0, 0,
				-15, 0,  0,  0,  0,  0,  0,  0, 60, 60, 70, 70, 15, 15, 60, 60,		16, 0, 0, 0,
				0,   0,  0,  0,  0,  0,  0,  0, 30, 30, 110, 110, 60, 60, 60, 60,	12, 1, 0, 0,
				30,  0,  0,  0,  0,  0,  0,  0, 70, 70, 85, 85, -50, -50, 60, 60,	16, 0, 0, 0,
				0,   0,  0,  0,  0,  0,  0,  0, 30, 30, 30, 30, 30, 30, 30, 30,		8, 0, 0, 0,
			};
			cmd_def_t test{ SKILL_DATA, "K", "PUSH_UP" };
			for (auto val : pu) {
				test += val;
			}

			int8_t roll{ 0 };
			int8_t pitch{ 0 };
			int8_t ratio{ 1 };
			animation_t pu_animate{ 7, 8, 3 };
			framelist_t pu_frames{
				{
					{ 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30,}
					, { 15, 0, 0, 0, 0, 0, 0, 0, 30, 35, 40, 21, 50, 15, 15, 41,}
					, { 15, 0, 0, 0, 0, 0, 0, 0, 30, 35, 40, 30, 50, 15, 15, 14,}
					, { 30, 0, 0, 0, 0, 0, 0, 0, 27, 35, 40, 60, 50, 15, 20, 45,}
					, { 15, 0, 0, 0, 0, 0, 0, 0, 42, 35, 40, 60, 25, 20, 20, 60,}
					, {	0, 0, 0, 0, 0, 0, 0, 0, 48, 45, 75, 60, 20, 37, 20, 60,}
					, { -15, 0, 0, 0, 0, 0, 0, 0, 60, 60, 70, 70, 15, 15, 60, 60,}
					, {	0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 110, 110, 60, 60, 60, 60,}
					, { 30, 0, 0, 0, 0, 0, 0, 0, 70, 70, 85, 85, -50, -50, 60, 60,}
					, {	0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30,}
				}
			};
			actionlist_t pu_actions{
				{
					{8, 0, 0, 0,}
					, {12, 0, 0, 0,}
					, {16, 0, 0, 0,}
					, {16, 0, 0, 0,}
					, {12, 0, 0, 0,}
					, {12, 0, 0, 0,}
					, {16, 0, 0, 0,}
					, {12, 1, 0, 0,}
					, {16, 0, 0, 0,}
					, {8, 0, 0, 0}
				}
			};
			behavior_t pu_behavior{
				roll
				, pitch
				, ratio
				, pu_animate
				, pu_frames
				, pu_actions
			};
			ostringstream os;
			os << pu_behavior;
			string dat{ os.str() };
			vector <int8_t> buf{ dat.begin(), dat.end() };
			EXPECT_EQ(sizeof(pu), buf.size());
			for (auto i = 0; i < sizeof(pu); ++i) {
				EXPECT_EQ(pu[i], dat[i]);
			}

			cmd_skill_data skill_data{ pu_behavior };
			return on_command(skill_data, 4 * RX_LATENCY_DEF);
		}

		bool ftfBittleX::on_skill_data(const int8_t dat[], size_t size)
		{
			cmd_def_t cmd_skill{ SKILL_DATA, "K", "USER_SKILL" };
			vector <int8_t> user_skill{ dat, dat + size };
			cmd_skill += user_skill;
			return on_command(cmd_skill, 2 * RX_LATENCY_DEF);
		}
	}	// namespace ftBittleX
}	// namespace utPetoi

