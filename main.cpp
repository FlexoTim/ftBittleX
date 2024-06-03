// https://www.amazon.com/Twidec-connector-Connector-Terminals-Connectors/dp/B0B154J6GS?ref_=ast_sto_dp
#include "ftBittleX.h"

int main(int argc, char** argv)
{
    using std::string;
    using std::vector;

    ::testing::InitGoogleTest(&argc, argv);

    utPetoi::commandline = vector<string>(argv, argv + argc);

    return RUN_ALL_TESTS();
}