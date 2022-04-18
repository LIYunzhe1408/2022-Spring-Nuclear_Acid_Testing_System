/// *
// @Author: ������ 20123101
// Created at Apr.12.2022
// Attention: Recommanded to be included in "COVID_19_TestingSystem.h".
// *
#pragma once
#ifndef __TESTTUBE_H__
#define __TESTTUBE_H_
#include <iostream>
#include <vector>
#include "Personnel/Personnel.h"
using namespace std;
enum class TEST_RESULT{NEGTIVE, POSITIVE, SUSPECTED, TESTING};
class TestTube
{
protected:
    bool is_tested_ = false;
    TEST_RESULT test_result_;							// ���Խ��
    int test_mode_;										// ���Է�ʽ�����0/����1
    static int total_mixed_test_tube_amount_;			// �����������Ϊ��̬������
    static int total_single_test_tube_amount_;			// ���嵥������Ϊ��̬����
    std::string tube_number_;							// �ùܱ��
    vector<Personnel> tested_personnels;				// ������Ա�飺10��Ϊ����
    std::string ProduceTubeNum(int test_tube_number);	// ���ɸùܱ�ţ�������ʹ��
public:
    bool IsTested();
    void SetTested();
    vector<Personnel> GetPersonnelInThisTube();
    void SetTestResult(TEST_RESULT test_result);
    void OpenOneTube(int test_mode, vector<Personnel> personnels);
    TEST_RESULT GetTestResult()
    {
        return test_result_;
    }
    std::string GetTubeNum() { return tube_number_; }
};

#endif