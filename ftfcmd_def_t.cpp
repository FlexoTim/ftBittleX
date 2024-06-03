// ====================================================
// ====================================================
#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {
		using ftfcmd_def_t = ftfwin32;

		extern vector <cmd_def_t> cmdlist;

		TEST_F(ftfcmd_def_t, max_cmdid_t) {
			const cmdid_t max_cmdid{ TUNER };
			EXPECT_GT(max_cmdid, cmdlist.size());
		}

		// ====================================================
		// constructor
		// ====================================================
		TEST_F(ftfcmd_def_t, default_ctor_nothrow) {
			EXPECT_NO_THROW(cmd_def_t command{});
		}
		TEST_F(ftfcmd_def_t, default_ctor_id_match) {
			cmd_def_t command{};
			EXPECT_EQ(cmdid_t::QUERY, command.id);
		}
		TEST_F(ftfcmd_def_t, default_ctor_cmd_empty) {
			cmd_def_t command{};
			EXPECT_TRUE(command.cmd.empty());
		}
		TEST_F(ftfcmd_def_t, default_ctor_description_empty) {
			cmd_def_t command{};
			EXPECT_TRUE(command.description.empty());
		}
		TEST_F(ftfcmd_def_t, default_ctor_data_empty) {
			cmd_def_t command{};
			EXPECT_EQ(0U, command.data.size());
		}
		TEST_F(ftfcmd_def_t, ctor_id_match) {
			cmd_def_t command{ QUERY, "?", "QUERY" };
			EXPECT_EQ(cmdid_t::QUERY, command.id);
		}
		TEST_F(ftfcmd_def_t, ctor_cmd_match) {
			cmd_def_t command{ QUERY, "?", "QUERY" };
			EXPECT_EQ("?", command.cmd);
		}
		TEST_F(ftfcmd_def_t, ctor_description_match) {
			cmd_def_t command{ QUERY, "?", "QUERY" };
			EXPECT_EQ("QUERY", command.description);
		}

		// ====================================================
		// add operator
		// ====================================================
		TEST_F(ftfcmd_def_t, add_ASC_char_match) {
			cmd_def_t command{ JOINTS, "j", "JOINTS" };
			ASSERT_TRUE(command.data.empty());
			command += 'A';
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(1U, command.data.size());
			EXPECT_EQ('A', command.data[0]);
			cout << command;
		}
		TEST_F(ftfcmd_def_t, add_ASC_string_match) {
			cmd_def_t command{ JOINTS, "j", "JOINTS" };
			ASSERT_TRUE(command.data.empty());
			command += string("0123456789");
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(10U, command.data.size());
			EXPECT_EQ(string("0123456789"), string(command.data.begin(), command.data.end()));
			cout << command;
		}

		TEST_F(ftfcmd_def_t, add_ASC_int_match) {
			cmd_def_t command{ JOINTS, "j", "JOINTS" };
			ASSERT_TRUE(command.data.empty());
			command += -1;
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(2U, command.data.size());
			EXPECT_EQ('-', command.data[0]);
			EXPECT_EQ('1', command.data[1]);
			cout << command;
		}
		TEST_F(ftfcmd_def_t, add_ASC_vector_match) {
			cmd_def_t command{ JOINTS, "j", "JOINTS" };
			ASSERT_TRUE(command.data.empty());
			vector <uint8_t> joints{ LLEG, RLEG };
			command += joints;
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(5U, command.data.size());
			EXPECT_EQ(string("15 14"), string(command.data.begin(), command.data.end()));
			cout << command;
		}

		TEST_F(ftfcmd_def_t, add_BIN_char_match) {
			cmd_def_t command{ JOINTS, "I", "INDEXED_SIMULTANEOUS_BIN" };
			ASSERT_TRUE(command.data.empty());
			command += 'A';
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(1U, command.data.size());
			EXPECT_EQ('A', command.data[0]);
			cout << command;
		}
		TEST_F(ftfcmd_def_t, add_BIN_string_match) {
			cmd_def_t command{ JOINTS, "I", "INDEXED_SIMULTANEOUS_BIN" };
			ASSERT_TRUE(command.data.empty());
			command += string("0123456789");
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(10U, command.data.size());
			EXPECT_EQ(string("0123456789"), string(command.data.begin(), command.data.end()));
			cout << command;
		}
		TEST_F(ftfcmd_def_t, add_BIN_int_match) {
			cmd_def_t command{ JOINTS, "I", "INDEXED_SIMULTANEOUS_BIN" };
			ASSERT_TRUE(command.data.empty());
			command += -1;
			EXPECT_FALSE(command.data.empty());
			EXPECT_EQ(1U, command.data.size());
			EXPECT_EQ(255U, command.data[0]);
			cout << command;
		}
	}	// namespace ftBittleX
}	// namespace utPetoi
