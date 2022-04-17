/// *
// @Author: 李昀哲 20123101
// Created at Apr.12.2022
// Attention: Recommanded to be included in "TestingSystem.h".
// *
#pragma once
#ifndef __TESTTUBE_H__
#define __TESTTUBE_H_
#include <iostream>
#include <vector>
#include "Personnel/Personnel.h"
enum class TEST_RESULT{NEGTIVE, POSITIVE, SUSPECTED, TESTING};
class TestTube
{
protected:
    TEST_RESULT test_result_;							// 测试结果
    int test_mode_;										// 测试方式：混合0/单人1
    static int total_mixed_test_tube_amount_;			// 定义混检管数量为静态变量。
    static int total_single_test_tube_amount_;			// 定义单管数量为静态变量
    std::string tube_number_;							// 该管编号
    std::vector<Personnel> tested_personnels;			// 测试人员组：10人为上限
    std::string ProduceTubeNum(int test_tube_number);	// 生成该管编号，仅类内使用
public:
    std::vector<Personnel> GetPersonnelInThisTube();
    void SetTestResult(TEST_RESULT test_result);
    void OpenOneTube(int test_mode, const std::vector<Personnel> & personnels);
    TEST_RESULT GetTestResult()
    {
        return test_result_;
    }
    std::string GetTubeNum() { return tube_number_; }
};

#endif