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
	TestTube* new_tube_;							// �����µ��Թ�
	std::vector<TestTube> tested_tube_box_;			// �Ѽ���Թ����ӣ�Ĭ���������Թ�
	std::vector<Personnel> tested_personnels_;		// �Ѿ���������
	std::queue<Personnel> single_testing_queue_;	// ���˹ܼ�����
	std::queue<Personnel> mixed_testing_queue_;		// ������
	void TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels);
	std::vector<std::string> DecideCloseContact(const std::vector<std::string>& confirmed_blocks);			// ȷ���ܽ�
	void DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks);						// ȷ�����ܽ�
	void DecideNagetive();
public:
	int GetQueueAmount(QUEUE_TYPE queue_type);
	void EnQueue(QUEUE_TYPE queue_type, Personnel &personnel);
	void DelQueue(QUEUE_TYPE queue_type);
	void SetTubeResult(const std::vector<std::string>& tube_numbers, TEST_RESULT result);					// ���ú�������
	void DisplayTube();
	void DisplayPersonnel();
	void DisplayPersonnelCloser();

	void DecidePersonnelStatus();					// ������Ա״̬
	bool IsEmpty(QUEUE_TYPE queue_type);
	STATUS GetPersonnelStatus(const std::string & personnel_code);
};
#endif