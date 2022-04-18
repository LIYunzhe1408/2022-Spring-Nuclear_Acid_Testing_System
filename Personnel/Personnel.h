// *
// @Author: ������ 20123101
// Created at Apr.12.2022
// Attention: Recommanded to be included in "TestingSystem.h".
// *
#pragma once
#ifndef __PERSONNEL_H__
#define __PERSONNEL_H_
#include <vector>
#include <string>
#include <iostream>
using namespace std;
enum class STATUS{NEGATIVE, COMFIRMED, SUSPECTED, CLOSE_CONTACT, SUB_CLOSE_CONTACT, WAITING_RESULT, QUEUING, NOT_TESTED, NOT_FOUND};
class Personnel
{
protected:
    vector<Personnel> close_contact_in_queue_;
    std::string personnel_code_;	// ��Ա����
    STATUS status_;					// ��Ա״̬
public:
    Personnel(std::string personnel_code = "00000000", STATUS status = STATUS::NOT_TESTED);
    void AddCloseContactPersonnel(const Personnel& personnel);		// ����Ŷ�ʱ���ٽ���Ա
    vector<Personnel> GetCloseContactInQueue() const;				// ��ȡ�ٽ���Ա
    STATUS GetStatus();												// ��ȡ��ǰ��Ա״̬
    std::string GetCode();											// ��ȡ��ǰ��Ա����
    void SetStatus(STATUS status);									// ���ü����
    Personnel& operator=(const Personnel& personnel);
    friend std::istream &operator>>(std::istream  &, Personnel& personnel)
    {
        std::cin >> personnel.personnel_code_;

        return std::cin;
    }
};
#endif