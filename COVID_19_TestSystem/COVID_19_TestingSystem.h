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
    TestTube* new_tube_;							// �����µ��Թ�
    const double kPOSITIVE_PROBABILITY = 0.012;		// ���Լ���

    // 7��״̬��
    std::vector<Personnel> confirmed_personnels_;
    std::vector<Personnel> negative_personnels_;
    std::vector<Personnel> suspected_personnels_;
    std::vector<Personnel> close_contact_personnels_;
    std::vector<Personnel> sub_close_contact_personnels_;
    std::vector<Personnel> need_to_be_uploaded_personnels_;
    std::vector<Personnel> queueing_personnels_;


    std::vector<TestTube> tested_tube_box_;			// �Ѽ���Թ����ӣ�Ĭ���������Թ�
    std::vector<Personnel> tested_personnels_;		// �Ѿ���������
    std::queue<Personnel> single_testing_queue_;	// ���˹ܼ�����
    std::queue<Personnel> mixed_testing_queue_;		// ������
    void TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels);
    std::vector<std::string> DecideCloseContact(const std::vector<std::string>& confirmed_blocks);			// ȷ���ܽ�
    void DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks);						// ȷ�����ܽ�
    void DecideNegative();
    void SetTubeResult(TestTube &tube, TEST_RESULT result);					// ���ú�������
    void UpdateAllTypeInfo();
    void DisplayType(STATUS status);
    STATUS GetPersonnelStatus(const std::string& personnel_code);
public:
    int GetQueueAmount(QUEUE_TYPE queue_type);
    int GetTestedPersonnelAmount() const;
    int GetTestedTubeAmount() const;
    void EnQueue(QUEUE_TYPE queue_type, Personnel personnel);
    void DelQueue(QUEUE_TYPE queue_type);
    void RandomizeTubeInfo();												// ������Թܣ�����Ϊ1.2%��

    void DisplayStatus(const std::string& personnel_code);
    void DisplayTube();
    void DisplayPersonnel();
    void DisplayPersonnelCloser();
    void DisplayAllTypePersonnel();
    void DisplayQueueing();

    void DecidePersonnelStatus();					// ������Ա״̬
    bool IsEmpty(QUEUE_TYPE queue_type);
};
#endif