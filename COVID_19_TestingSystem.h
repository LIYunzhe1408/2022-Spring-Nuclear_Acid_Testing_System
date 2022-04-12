#pragma once
#ifndef __COVID_19_TESTINGSYSTEM_H__
#define __COVID_19_TESTINGSYSTEM_H__
#include <queue>
#include <iostream>
#include "Personnel.h"
#include "TestTube.h"
enum class QUEUE_TYPE{SINGLE, MIXED};
class COVID_19_TestingSystem
{
protected:
	TestTube* new_tube_;							// 启封新的试管
	std::vector<TestTube> tested_tube_box_;			// 已检测试管箱子，默认有无限试管
	std::vector<Personnel> tested_personnels_;		// 已经检测过的人
	std::queue<Personnel> single_testing_queue_;	// 单人管检测队列
	std::queue<Personnel> mixed_testing_queue_;		// 混检队列
	void TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels);
	std::vector<std::string> DecideCloseContact(const std::vector<std::string>& confirmed_blocks);			// 确定密接
	void DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks);						// 确定次密接
	void DecideNagetive();
public:
	int GetQueueAmount(QUEUE_TYPE queue_type);
	void EnQueue(QUEUE_TYPE queue_type, Personnel &personnel);
	void DelQueue(QUEUE_TYPE queue_type);
	void SetTubeResult(const std::vector<std::string>& tube_numbers, TEST_RESULT result);					// 设置核酸检测结果
	void DisplayTube();
	void DisplayPersonnel();
	void DisplayPersonnelCloser();

	void DecidePersonnelStatus();					// 决定人员状态
	bool IsEmpty(QUEUE_TYPE queue_type);
	STATUS GetPersonnelStatus(const std::string & personnel_code);
};
#endif