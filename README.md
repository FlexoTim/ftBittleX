# ftBittleX

Petoi BittleX feature test

## Usage

  This program contains tests written using Google Test. You can use the
  following command line flags to control its behavior:
  
  Test Selection:
    --gtest_list_tests
        List the names of all tests instead of running them. The name of
        TEST(Foo, Bar) is "Foo.Bar".
    --gtest_filter=POSTIVE_PATTERNS[-NEGATIVE_PATTERNS]
        Run only the tests whose name matches one of the positive patterns but
        none of the negative patterns. '?' matches any single character; '*'
        matches any substring; ':' separates two patterns.
    --gtest_also_run_disabled_tests
        Run all disabled tests too.
  
  Test Execution:
    --gtest_repeat=[COUNT]
        Run the tests repeatedly; use a negative count to repeat forever.
    --gtest_shuffle
        Randomize tests' orders on every iteration.
    --gtest_random_seed=[NUMBER]
        Random number seed to use for shuffling test orders (between 1 and
        99999, or 0 to use a seed based on the current time).
  
  Test Output:
    --gtest_color=(yes|no|auto)
        Enable/disable colored output. The default is auto.
    --gtest_print_time=0
        Don't print the elapsed time of each test.
    --gtest_output=(json|xml)[:DIRECTORY_PATH\|:FILE_PATH]
        Generate a JSON or XML report in the given directory or with the given
        file name. FILE_PATH defaults to test_details.xml.
  
  Assertion Behavior:
    --gtest_break_on_failure
        Turn assertion failures into debugger break-points.
    --gtest_throw_on_failure
        Turn assertion failures into C++ exceptions for use by an external
        test framework.
    --gtest_catch_exceptions=0
        Do not report exceptions as test failures. Instead, allow them
        to crash the program or throw a pop-up (on Windows).
  
  Except for --gtest_list_tests, you can alternatively set the corresponding
  environment variable of a flag (all letters in upper-case). For example, to
  disable colored text output, you can either specify --gtest_color=no or set
  the GTEST_COLOR environment variable to no.
  
  For more information, please read the Google Test documentation at
  https://github.com/google/googletest/. If you find a bug in Google Test
  (not one in your own code or tests), please report it to
  <googletestframework@googlegroups.com>.

## Featue test list

**Test Case: ftfBittleXBehavior.**  
-    angry
-    DISABLED_backflip
-    boxing
-    cheers
-    check
-    come_here
-    dig
-    DISABLED_frontflip
-    high_five
-    good_boy
-    handstand
-    hug
-    hi
-    hand_shake
-    hands_up
-    jump
-    kick
-    leapover
-    moon_walk
-    nod
-    play_dead
-    pee
-    push_ups
-    pushups_one_hand
-    recover
-    roll
-    scratch
-    sniff
-    table
-    test
-    wave_head
-    zero
-    Behavior_Data
-    Behavior_Data_User

**Test Case: ftfcmd_def_t.**  
-    max_cmdid_t
-    default_ctor_nothrow
-    default_ctor_id_match
-    default_ctor_cmd_empty
-    default_ctor_description_empty
-    default_ctor_data_empty
-    ctor_id_match
-    ctor_cmd_match
-    ctor_description_match
-    add_ASC_char_match
-    add_ASC_string_match
-    add_ASC_int_match
-    add_ASC_vector_match
-    add_BIN_char_match
-    add_BIN_string_match
-    add_BIN_int_match

**Test Case: ftfBittleXGait.**  
-    DISABLED_bound_forward
-    backward
-    backward_left
-    backward_right
-    backward_random
-    crawl_forward
-    crawl_Left
-    crawl_right
-    crawl_random
-    gap_forward
-    gap_Left
-    gap_right
-    gap_random
-    halloween
-    DISABLED_jump_forward
-    push_forward
-    push_left
-    push_right
-    push_random
-    trot_forward
-    trot_Left
-    trot_right
-    trot_random
-    step
-    spring_left
-    spring_right
-    spring_random
-    walk_forward
-    walk_left
-    walk_right
-    walk_random
-    Gait_Data
-    Gait_Data_User

**Test Case: ftfBittleXPosture.**  
-    Balance_skill
-    ButtUp_skill
-    Calibrate_skill
-    Dropped
-    Lifted
-    Landing
-    Sit
-    Stretch
-    Standup
-    Zero
-    Posture_Data
-    Posture_Data_User

**Test Case: ftfBittleXProtocol.**  
-    CR_only
-    LF_only
-    CRLF_only
-    invalid
-    QUERY
-    ABORT
-    DISABLED_BEEP_volume
-    DISABLED_BEEP_getmute
-    DISABLED_BEEP_setmute
-    DISABLED_BEEP_tune
-    CALIBRATE
-    REST
-    INDEXED_SIMULTANEOUS_ASC_angle
-    INDEXED_SIMULTANEOUS_ASC_list
-    JOINTS_list
-    JOINTS_index
-    JOINTS_index_sweep
-    INDEXED_SEQUENTIAL_ASC_angle
-    INDEXED_SEQUENTIAL_BIN_angle
-    INDEXED_SEQUENTIAL_ASC_list
-    INDEXED_SEQUENTIAL_ASC_OWL
-    INDEXED_SEQUENTIAL_BIN_list
-    INDEXED_SIMULTANEOUS_BIN_angle
-    INDEXED_SIMULTANEOUS_BIN_list
-    INDEXED_SEQUENTIAL_ASC_list_repeat
-    COLOR
-    VERBOSELY_PRINT_GYRO
-    PRINT_GYRO
-    GYRO_BALANCE
-    LISTED_BIN
-    PAUSE
-    TEMP
-    DISABLED_RANDOM_MIND
-    DIGITAL_READ
-    ANALOG_READ
-    DISABLED_SAVE
-    GYRO_FINENESS
-    DISABLED_MELODY
-    SLOPE
-    TILT
-    MEOW
-    DISABLED_SERVO_PWM
-    DISABLED_ACCELERATE
-    DISABLED_DECELERATE
-    on_command_list
-    DISABLED_example_py
-    DISABLED_ASC_newCmd_overflow
-    DISABLED_BIN_newCmd_overflow
-    DISABLED_SKILL_DATA_newCmd_overflow
-    detect_minmax_angle
-    servo_collision

**Test Case: utfwin32.**  
-    ctor_valid
-    DISABLED_unexpected_error_fails
-    expected_error
-    on_error_ERROR_SUCCESS
-    DISABLED_on_error_unexpected_fails
-    on_error_expected

## Output

>[==========] Running 132 tests from 6 test cases.  
[----------] Global test environment set-up.  
>
>  ...test ouput...  
>
>[----------] Global test environment tear-down  
[==========] 132 tests from 6 test cases ran. (424465 ms total)  
[  PASSED  ] 124 tests.  
[  FAILED  ] 8 tests, listed below:  
[  FAILED  ] ftfBittleXProtocol.CR_only  
[  FAILED  ] ftfBittleXProtocol.LF_only  
[  FAILED  ] ftfBittleXProtocol.CRLF_only  
[  FAILED  ] ftfBittleXProtocol.invalid  
[  FAILED  ] ftfBittleXProtocol.COLOR  
[  FAILED  ] ftfBittleXProtocol.TEMP  
[  FAILED  ] ftfBittleXProtocol.SLOPE 
[  FAILED  ] ftfBittleXProtocol.detect_minmax_angle  
>  
>8 FAILED TESTS  
YOU HAVE 20 DISABLED TESTS
