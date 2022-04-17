#pragma once
#ifndef __COVID_19_TESTINGSYSTEM_H__
#define __COVID_19_TESTINGSYSTEM_H__
#include <queue>
#include <iostream>
#include "Personnel/Personnel.h"
#include "TestTube/TestTube.h"
#include <random>
#include <iomanip>
enum class QUEUE_TYPE{SINGLE, MIXED};
class COVID_19_TestingSystem
{
protected:
    TestTube* new_tube_;							// 启封新的试管
    const double kPOSITIVE_PROBABILITY = 0.012;		// 阳性几率

    // 7种状态组
    std::vector<Personnel> confirmed_personnels_;
    std::vector<Personnel> negative_personnels_;
    std::vector<Personnel> suspected_personnels_;
    std::vector<Personnel> close_contact_personnels_;
    std::vector<Personnel> sub_close_contact_personnels_;
    std::vector<Personnel> need_to_be_uploaded_personnels_;
    std::vector<Personnel> queueing_personnels_;


    std::vector<TestTube> tested_tube_box_;			// 已检测试管箱子，默认有无限试管
    std::vector<Personnel> tested_personnels_;		// 已经检测过的人
    std::queue<Personnel> single_testing_queue_;	// 单人管检测队列
    std::queue<Personnel> mixed_testing_queue_;		// 混检队列
    void TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels);
    std::vector<std::string> DecideCloseContact(const std::vector<std::string>& confirmed_blocks);			// 确定密接
    void DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks);						// 确定次密接
    void DecideNegative();
    void SetTubeResult(TestTube &tube, TEST_RESULT result);					// 设置核酸检测结果
    void UpdateAllTypeInfo();
    void DisplayType(STATUS status);
    STATUS GetPersonnelStatus(const std::string& personnel_code);
public:
    int GetQueueAmount(QUEUE_TYPE queue_type);
    int GetTestedPersonnelAmount() const;
    int GetTestedTubeAmount() const;
    void EnQueue(QUEUE_TYPE queue_type, Personnel personnel);
    void DelQueue(QUEUE_TYPE queue_type);
    void RandomizeTubeInfo();												// 随机阳性管，概率为1.2%。

    void DisplayStatus(const std::string& personnel_code);
    void DisplayTube();
    void DisplayPersonnel();
    void DisplayPersonnelCloser();
    void DisplayAllTypePersonnel();
    void DisplayQueueing();

    void DecidePersonnelStatus();					// 决定人员状态
    bool IsEmpty(QUEUE_TYPE queue_type);
};
#endif