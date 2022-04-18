/// *
// @Author: ������ 20123101
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
    const double kPOSITIVE_PROBABILITY = 0.012;						// ���Լ���
    std::string line_up_file_name_, nucleic_acid_test_file_name_;	// �ļ���
    TestTube* new_tube_;											// �µ��Թ�

    // 7��״̬��
    vector<Personnel> negative_personnels_;
    vector<Personnel> queueing_personnels_;
    vector<Personnel> suspected_personnels_;
    vector<Personnel> confirmed_personnels_;
    vector<Personnel> close_contact_personnels_;
    vector<Personnel> sub_close_contact_personnels_;
    vector<Personnel> need_to_be_uploaded_personnels_;

    int tested_mixed_amount_;							// �Ѽ��������
    int tested_single_amount_;							// �Ѽ�ⵥ������
    vector<TestTube> tested_tube_box_;				// �Ѽ���Թ����ӣ�Ĭ���������Թ�
    vector<Personnel> tested_personnels_;				// �Ѿ���������
    queue<Personnel> single_testing_queue_;				// ���˹ܼ�����
    queue<Personnel> mixed_testing_queue_;				// ������


    // ��ʼ����ʷ��Ϣ
    void UpdateHistoryInfo();
    void UpdateAmount(std::ifstream& inFile);
    void UpdateTestedResult(std::ifstream& inFile);
    void AddTestedPersonnel(const Personnel& personnel);
    // ��ʾ�˵�
    int GetTestedMixedAmount() const;
    int GetTestedSingleAmount()  const;
    int GetTestedPersonnelAmount() const;
    int GetQueueAmount(QUEUE_TYPE queue_type);
    // ��ʼ������
    void LineUpFromFile(const std::string& file_name);
    // ����1��
    void EnQueue(QUEUE_TYPE queue_type, Personnel personnel);
    // ����2��
    void DelQueue(QUEUE_TYPE queue_type);
    void TestNuclearAcid(QUEUE_TYPE queue_type, vector<Personnel>& testing_personnels);
    // ����3��
    void DisplayQueueing();
    // ����4��
    int GetTestedTubeAmount();
    void RandomizeTubeInfo();									// ������Թܣ�����Ϊ1.2%��
    void SetTubeResult(TestTube& tube, TEST_RESULT result);		// ���ú�������
    void DisplayTube();
    void DecidePersonnelStatus();								// ������Ա״̬
    vector<std::string> DecideCloseContact(vector<std::string> confirmed_blocks);			// ȷ���ܽ�
    void DecideSubCloseContact(vector<std::string> close_contact_blocks);						// ȷ�����ܽ�
    void DecideNegative();
    // ����5��
    void UpdateAllTypeInfo();
    void DisplayType(STATUS status);
    // ����6��
    void DisplayStatus(const std::string& personnel_code);
    STATUS GetPersonnelStatus(const std::string& personnel_code);
    // ���棺
    void SaveToFile(const std::string& file_name);

    // ����ͨ�ù���
    void DisplayPersonnel();
    void DisplayPersonnelCloser();
    bool IsEmpty(QUEUE_TYPE queue_type);

public:
    COVID_19_TestingSystem(std::string line_up_file_name, std::string nucleic_acid_test_file_name)
            :line_up_file_name_(line_up_file_name), nucleic_acid_test_file_name_(nucleic_acid_test_file_name)
    {
        UpdateHistoryInfo();
    }
    void DisplayMenu();					// ��ʾ�˵�
    void InitQueue();					// ����0
    void AddQueuePersonnel();			// ����1
    void Testing();						// ����2
    void ShowQueueStatus();				// ����3
    void RegisterPersonnelInfo();		// ����4
    void DisplayAllTypePersonnel();		// ����5
    void DisplaySpecificPersonnel();	// ����6
    void Save();						// ����
};
#endif