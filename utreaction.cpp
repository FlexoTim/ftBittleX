#include "ftBittleX.h"

namespace utPetoi {
	namespace ftBittleX {
#pragma warning(disable: 4996)

		namespace internal {
			const int8_t DOF{ 16 };
			const int8_t MAXOFFSET{ 120 };
			const int8_t MINOFFSET{ -120 };

			void output_offset_list(const int8_t* list) {
				for (int i = 0; i < DOF; ++i) {
					int8_t offset{list[i]};
					cout << "offset[" << i << "]: ";
					if (int8_t(127) == offset) {
						cout << "unmodified\n";
					}
					else {
						cout << int(offset) << "\n";
					}
				}
			}

			bool extract_offset_list(char* buffer, int8_t* list)
			{
				bool result{ false };
				if (nullptr == buffer || nullptr == list) {
					return result;
				}

				const char* delimeter{ " ,\t" };
				char* pch{ strtok(buffer, delimeter) };
				while (nullptr != pch) {
					int8_t idx{ int8_t(atoi(pch)) };	// index
					pch = strtok(nullptr, delimeter);   // offset
					if (nullptr == pch) {
						return result;
					}
					result = true;
					int8_t offset{ int8_t(atoi(pch)) };

					// filter out invalid elements
					if ((idx < 0) || (DOF <= idx)
						|| (offset < MINOFFSET)
						|| (MAXOFFSET < offset)) {
						continue;
					}
					list[int(idx)] = offset;

					pch = strtok(nullptr, delimeter);   // offset
				}
				// list holds offsets for all joints
				// An offset of 127 means the joint is unmodified
				return result;
			}
			bool extract_offset_listB(char* buffer, int8_t* list, const char* delimeter)
			{
				char* pch{ strtok(buffer, delimeter) };
				while (nullptr != pch) {
					int8_t idx{ int8_t(atoi(pch)) };
					pch = strtok(nullptr, delimeter);
					int8_t offset{ (nullptr == pch) ? int8_t(127) : int8_t(atoi(pch)) };
					if (0 <= idx && idx < DOF) {
						list[idx] = offset;
					}
					pch = strtok(nullptr, delimeter);
				}
				return true;
			}
		}
		using std::strtok;
		using utfreaction = utfwin32;

		TEST_F(utfreaction, strtok_baseline)
		{
			char newCmd[]{ "0 1 8 9 10 11 12 13 14 15 15 16" };
			char delim[]{ " ,\t" };
			char* pch{ strtok(newCmd, delim) };
			do {
				int target[2]{};
				int inLen{ 0 };
				for (byte b = 0; b < 2 && nullptr != pch; ++b) {
					cout << "pch: " << setw(3) << pch
						<< " atoi(pch): " << setw(3) << atoi(pch)
						<< '\n';
					target[b] = atoi(pch);
					pch = strtok(NULL, " ,\t");
					inLen++;
				}
			} while (nullptr != pch);
		}

		TEST_F(utfreaction, strtok_strcpy)
		{
			char newCmd[]{ "0 1 8 9 10 11 12 13 14 15 15 16" };
			char delim[]{ " ,\t" };
			char* pch{ strtok(newCmd, delim) };
			do {
				int target[2]{};
				int inLen{ 0 };
				for (byte b = 0; b < 2 && nullptr != pch; ++b) {
					cout << "pch: " << setw(3) << pch
						<< " atoi(pch): " << setw(3) << atoi(pch)
						<< '\n';
					target[b] = atoi(pch);
					pch = strtok(NULL, " ,\t");
					inLen++;
				}
				strcpy(newCmd, "calib");
			} while (nullptr != pch);
		}

		TEST_F(utfreaction, extract_offset_list_baseline)
		{
			using namespace internal;

			char newCmd[]{ "0 1 8 9 10 11 12 13 14 15 15 16" };
			int8_t list[DOF]{};               // safety
			memset(list, 127, sizeof(list));    // default value; (unset)
			if (!extract_offset_list(newCmd, list)) {
				cout << "invalid command syntax; ignored\n";
			}
			else {
				output_offset_list(list);
			}
		}
		TEST_F(utfreaction, extract_offset_list_B)
		{
			using namespace internal;

			char newCmd[]{ "0 1 8 9 10 11 12 13 14 15 15 16" };
			int8_t list[DOF]{};
			memset(list, 127, sizeof(list));    // default value; (unset)
			const char* delimeter{ " ,\t" };
			extract_offset_listB(newCmd, list, delimeter);
			output_offset_list(list);
		}
		TEST_F(utfreaction, extract_offset_list_B_comma)
		{
			using namespace internal;

			char newCmd[]{ "0,1,8,9,10,11,12,13,14,15,15,16" };
			int8_t list[DOF]{};
			memset(list, 127, sizeof(list));    // default value; (unset)
			const char* delimeter{ " ,\t" };
			extract_offset_listB(newCmd, list, delimeter);
			output_offset_list(list);
		}

		TEST_F(utfreaction, extract_offset_list_B_invalid_index)
		{
			using namespace internal;

			char newCmd[]{ "0 1 -8 9 10 11 12 13 14 15 -15 16" };
			int8_t list[DOF]{};
			memset(list, 127, sizeof(list));    // default value; (unset)
			const char* delimeter{ " ,\t" };
			extract_offset_listB(newCmd, list, delimeter);
			output_offset_list(list);
		}
		TEST_F(utfreaction, extract_offset_list_B_duplicate_offsets)
		{
			using namespace internal;

			char newCmd[]{
				"0 1 8 9 10 11 12 13 14 15 15 16 0 1 8 9 10 11 12 13 14 15 15 16"
			};
			int8_t list[DOF]{};
			memset(list, 127, sizeof(list));    // default value; (unset)
			const char* delimeter{ " ,\t" };
			extract_offset_listB(newCmd, list, delimeter);
			output_offset_list(list);
		}
		TEST_F(utfreaction, extract_offset_list_B_malformed)
		{
			using namespace internal;

			//char newCmd[]{ "0  1  8  9  10  11  12  13  14  15  15  16" };
			char newCmd[]{ "0,,1,,8,,9,,10,,11,,12,,13,,14,,15,,15,,16" };
			int8_t list[DOF]{};
			memset(list, 127, sizeof(list));    // default value; (unset)
			const char* delimeter{ " ,\t" };
			extract_offset_listB(newCmd, list, delimeter);
			output_offset_list(list);
		}

#pragma warning(4: 4996)

#ifdef RESEARCH
		// the following is for research only
		using utftransform = utfwin32;

		// template<typename T>
		// void transform(
		//	T *target
		//	, byte angleDataRatio = 1
		//	, float speedRatio = 1
		//	, byte offset = 0
		//	, int period = 0
		//	, int runDelay = 8
		// )
		// 
		TEST_F(utftransform, DISABLED_refactor)
		{
			int* diff = new int[DOF - offset]
				, maxDiff = 0;
			for (byte i = offset; i < DOF; i++) {
				if (manualHeadQ
					&& i < HEAD_GROUP_LEN
					&& T_SKILL == token)
					// the head motion will be
					// handled by skill.perform()
					continue;
				diff[i - offset] =
					currentAng[i]
					- target[i - offset] * angleDataRatio;
				maxDiff = max(
					maxDiff
					, abs(diff[i - offset])
				);
			}
			int steps =
				0 < speedRatio
				? int(round(maxDiff / 1.0 /*degreeStep*/ / speedRatio))
				: 0;  //default speed is 1 degree per step
			for (int s = 0; s <= steps; s++) {
				for (byte i = offset; i < DOF; i++) {
#ifdef ESP_PWM
					if (movedJoint[i])
						// don't drive the servo if
						// it's being moved by hand
						// in the follow function.
						continue;

					if (manualHeadQ
						&& i < HEAD_GROUP_LEN
						&& T_SKILL == token)
						// the head motion will be
						// handled by skill.perform()
						continue;

					if (WALKING_DOF == 8
						&& i > 3
						&& i < 8)
						continue;

					if (WALKING_DOF == 12
						&& i < 4)
						continue;
#endif
					float dutyAng =
						(target[i - offset]
							* angleDataRatio
							+ (steps == 0
								? 0 
								: (1 + cos(M_PI * s / steps))
									/ 2 * diff[i - offset]
								)
							);
					calibratedPWM(i, dutyAng);
				}
				delay((DOF - offset) / 2);
			}
			delete[] diff;
		}
#endif	// RESEARCH
	}	// namespace ftBittleX
}	// namespace utPetoi
