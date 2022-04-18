/// *
// @Author: 李昀哲 20123101
// Created at Apr.12.2022
// Attention: Recommanded to be included in "Question2.cpp".
// *
#pragma once
#ifndef __COVID_19_TESTINGSYSTEM_H__
#define __COVID_19_TESTINGSYSTEM_H__
#include <iostream>
#include "Personnel/Personnel.h"
#include "TestTube/TestTube.h"
#include <random>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <queue>
enum class QUEUE_TYPE{SINGLE, MIXED};
class COVID_19_TestingSystem
{
protected:
    const double kPOSITIVE_PROBABILITY = 0.012;						// 阳性几率
    std::string line_up_file_name_, nucleic_acid_test_file_name_;	// 文件名
    TestTube* new_tube_;											// 新的试管

    // 7种状态组
    vector<Personnel> negative_personnels_;
    vector<Personnel> queueing_personnels_;
    vector<Personnel> suspected_personnels_;
    vector<Personnel> confirmed_personnels_;
    vector<Personnel> close_contact_personnels_;
    vector<Personnel> sub_close_contact_personnels_;
    vector<Personnel> need_to_be_uploaded_personnels_;

    int tested_mixed_amount_;							// 已检测混检人数
    int tested_single_amount_;							// 已检测单检人数
    vector<TestTube> tested_tube_box_;				// 已检测试管箱子，默认有无限试管
    vector<Personnel> tested_personnels_;				// 已经检测过的人
    queue<Personnel> single_testing_queue_;				// 单人管检测队列
    queue<Personnel> mixed_testing_queue_;				// 混检队列


    // 初始化历史信息
    void UpdateHistoryInfo();
    void UpdateAmount(std::ifstream& inFile);
    void UpdateTestedResult(std::ifstream& inFile);
    void AddTestedPersonnel(const Personnel& personnel);
    // 显示菜单
    int GetTestedMixedAmount() const;
    int GetTestedSingleAmount()  const;
    int GetTestedPersonnelAmount() const;
    int GetQueueAmount(QUEUE_TYPE queue_type);
    // 初始化队列
    void LineUpFromFile(const std::string& file_name);
    // 功能1：
    void EnQueue(QUEUE_TYPE queue_type, Personnel personnel);
    // 功能2：
    void DelQueue(QUEUE_TYPE queue_type);
    void TestNuclearAcid(QUEUE_TYPE queue_type, vector<Personnel>& testing_personnels);
    // 功能3：
    void DisplayQueueing();
    // 功能4：
    int GetTestedTubeAmount();
    void RandomizeTubeInfo();									// 随机阳性管，概率为1.2%。
    void SetTubeResult(TestTube& tube, TEST_RESULT result);		// 设置核酸检测结果
    void DisplayTube();
    void DecidePersonnelStatus();								// 决定人员状态
    vector<std::string> DecideCloseContact(vector<std::string> confirmed_blocks);			// 确定密接
    void DecideSubCloseContact(vector<std::string> close_contact_blocks);						// 确定次密接
    void DecideNegative();
    // 功能5：
    void UpdateAllTypeInfo();
    void DisplayType(STATUS status);
    // 功能6：
    void DisplayStatus(const std::string& personnel_code);
    STATUS GetPersonnelStatus(const std::string& personnel_code);
    // 保存：
    void SaveToFile(const std::string& file_name);

    // 辅助通用功能
    void DisplayPersonnel();
    void DisplayPersonnelCloser();
    bool IsEmpty(QUEUE_TYPE queue_type);

public:
    COVID_19_TestingSystem(std::string line_up_file_name, std::string nucleic_acid_test_file_name)
            :line_up_file_name_(line_up_file_name), nucleic_acid_test_file_name_(nucleic_acid_test_file_name)
    {
        UpdateHistoryInfo();
    }
    void DisplayMenu();					// 显示菜单
    void InitQueue();					// 功能0
    void AddQueuePersonnel();			// 功能1
    void Testing();						// 功能2
    void ShowQueueStatus();				// 功能3
    void RegisterPersonnelInfo();		// 功能4
    void DisplayAllTypePersonnel();		// 功能5
    void DisplaySpecificPersonnel();	// 功能6
    void Save();						// 保存
};
#endif