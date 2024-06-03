#include "ftBittleX.h"

namespace utPetoi {
	vector <string> commandline;

#ifdef GTEST_OS_WINDOWS
	// ====================================================
	// ====================================================

	DWORD utfwin32::expect_error(DWORD dw)
	{
		return expected_error = dw;
	}

	// Retrieve the system error message for the last-error code
	void utfwin32::on_error(LPCTSTR lpszFunction, DWORD dw)
	{
		if (ERROR_SUCCESS == dw) {
			EXPECT_EQ(dw, expected_error) << "POST: expected error not found";
			return;
		}
		if (expected_error == dw) {
			::SetLastError(ERROR_SUCCESS);
			return;
		}

		LPTSTR lpMsgBuf{};
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL
			, dw
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
			, LPTSTR(&lpMsgBuf)
			, 0
			, NULL
		);

		// Display the error message
		LPVOID lpDisplayBuf{
			::LocalAlloc(
				LMEM_ZEROINIT
				, (lstrlen((LPCTSTR)lpMsgBuf)
					+ lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)
			)
		};
		::StringCchPrintf(
			(LPTSTR)lpDisplayBuf
			, ::LocalSize(lpDisplayBuf) / sizeof(TCHAR)
			, TEXT("%s failed with error %d: %s")
			, lpszFunction
			, dw
			, lpMsgBuf
		);
#ifdef  UNICODE
		wstring msg{ LPTSTR(lpDisplayBuf) };
		wcout << msg;
#else
		string msg{ LPTSTR(lpDisplayBuf) };
		cout << msg;
#endif

		::LocalFree(lpMsgBuf);
		::LocalFree(lpDisplayBuf);

		::SetLastError(ERROR_SUCCESS);

		EXPECT_EQ(expected_error, dw) << "POST: unexpected error";
	}
#endif	// GTEST_OS_WINDOWS

	// ====================================================
	// utfwin32 unit tests
	// ====================================================
	TEST_F(utfwin32, ctor_valid) {
		SUCCEED();
	}
	TEST_F(utfwin32, DISABLED_unexpected_error_fails) {
		EXPECT_NO_THROW(::SetLastError(ERROR_CAN_NOT_COMPLETE));
	}
	TEST_F(utfwin32, expected_error) {
		EXPECT_NO_THROW(::SetLastError(ERROR_INVALID_FUNCTION));
		expect_error(ERROR_INVALID_FUNCTION);
	}
	TEST_F(utfwin32, on_error_ERROR_SUCCESS) {
		on_error(TEXT(__FUNCTION__));
	}
	TEST_F(utfwin32, DISABLED_on_error_unexpected_fails) {
		EXPECT_NO_THROW(::SetLastError(ERROR_INVALID_FUNCTION));
		on_error(TEXT(__FUNCTION__));
	}
	TEST_F(utfwin32, on_error_expected) {
		EXPECT_NO_THROW(::SetLastError(ERROR_CAN_NOT_COMPLETE));
		expect_error(ERROR_CAN_NOT_COMPLETE);
		on_error(TEXT(__FUNCTION__));
		expect_error(ERROR_SUCCESS);	// reset expected
	}

	namespace utserial {

		// ================================================================
		// ================================================================
		bool utfSerial::parse_commandline()
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

		bool utfSerial::open_port()
		{
			EXPECT_NO_THROW(utSerial.setBaudrate(baud));
			EXPECT_NO_THROW(utSerial.setTimeout(ctimeout));
			EXPECT_NO_THROW(utSerial.setBytesize(bytesize));
			EXPECT_NO_THROW(utSerial.setParity(parity));
			EXPECT_NO_THROW(utSerial.setStopbits(stopbits));
			EXPECT_NO_THROW(utSerial.setFlowcontrol(flowcontrol));
			EXPECT_NO_THROW(utSerial.setPort(port));	// setPort can reopen a port
			EXPECT_FALSE(utSerial.isOpen());
			EXPECT_NO_THROW(utSerial.open());
			EXPECT_TRUE(utSerial.isOpen());

#ifdef DISABLED_CODE
			EXPECT_NO_THROW(utSerial.flush());
#endif	// DISABLED_CODE

			// todo: delay for Arduino loopback (?)
			using std::chrono::milliseconds;
			using std::this_thread::sleep_for;
			sleep_for(milliseconds(8));

			// todo: must manually flush
			size_t count{ utSerial.available() };
			if (count) {
				EXPECT_FALSE(utSerial.read(count).empty());
				EXPECT_EQ(0U, utSerial.available());
			}

			return utSerial.isOpen();
		}

		bool utfSerial::close_port()
		{
			if (utSerial.isOpen()) {
				EXPECT_NO_THROW(utSerial.flush());
				EXPECT_NO_THROW(utSerial.close());
			}
			return !utSerial.isOpen();
		}
	}	// namespace utserial

	namespace utserial {

	}	// namespace utserial

}	// namespace utPetoi
