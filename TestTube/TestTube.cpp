#include "TestTube.h"
// =============================================
// Protected
int TestTube::total_single_test_tube_amount_ = -1;	// 静态变量，记录单管检测方式人数，用于生成流水号
int TestTube::total_mixed_test_tube_amount_ = -1;	// 静态变量，记录混合检测方式人数，用于生成流水号

// 生成该测试管编号
std::string TestTube::ProduceTubeNum(int test_tube_number)
{
    std::string tube_number = std::to_string(test_mode_);
    if (test_tube_number == 0)
        tube_number += "000";
    while (test_tube_number < 1000 && test_tube_number != 0)
    {
        test_tube_number *= 10;
        tube_number += "0";
    }
    if(test_mode_ == 1)
        tube_number += std::to_string(total_single_test_tube_amount_);
    if (test_mode_ == 0)
        tube_number += std::to_string(total_mixed_test_tube_amount_);
    return tube_number;
}

// ==============================================
// Public
// 设置检测结果
vector<Personnel> TestTube::GetPersonnelInThisTube()
{
    return tested_personnels;
}

void TestTube::SetTestResult(TEST_RESULT test_result)
{
    test_result_ = test_result;
}

// 开封一个检测管
void TestTube::OpenOneTube(int test_mode, vector<Personnel> personnels)
{
    test_result_ = TEST_RESULT::TESTING;
    test_mode_ = test_mode;					// 记录这一管的测试模式
    if (test_mode_ == 1)
    {
        tested_personnels.push_back(personnels[0]);							// 记录单管的人员
        total_single_test_tube_amount_++;									// 单管检测总人数加一
        tube_number_ = ProduceTubeNum(total_single_test_tube_amount_);		// 生成该测试管编号
    }
    else if (test_mode_ == 0)
    {
        tested_personnels = personnels;
        total_mixed_test_tube_amount_++;
        tube_number_ = ProduceTubeNum(total_mixed_test_tube_amount_);
    }
}
bool TestTube::IsTested()
{
    return is_tested_;
}

void TestTube::SetTested()
{
    is_tested_ = true;
}