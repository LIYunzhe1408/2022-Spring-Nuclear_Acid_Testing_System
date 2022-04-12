// *
// @Author: ������ 20123101
// Created at Apr.12.2022
// Attention: Recommanded to be included in "TestingSystem.h".
// *
#pragma once
#ifndef __PERSONNEL_H__
#define __PERSONNEL_H_
#include <string>
#include <iostream>
#include <vector>
enum class STATUS{NEGATIVE, COMFIRMED, SUSPECTED, CLOSE_CONTACT, SUB_CLOSE_CONTACT, WAITING_RESULT, QUEUING, NOT_TESTED, NOT_FOUND};
class Personnel
{
protected:
	std::vector<Personnel> close_contact_in_queue_;
	std::string personnel_code_;	// ��Ա����
	STATUS status_;					// ��Ա״̬
public:
	Personnel(std::string personnel_code = "00000000", STATUS status = STATUS::NOT_TESTED);
	void AddCloseContactPersonnel(const Personnel& personnel);		// ����Ŷ�ʱ���ٽ���Ա
	std::vector<Personnel> GetCloseContactInQueue() const;			// ��ȡ�ٽ���Ա
	STATUS GetStatus();												// ��ȡ��ǰ��Ա״̬
	std::string GetCode();											// ��ȡ��ǰ��Ա����
	void SetStatus(STATUS status);									// ���ü����
	Personnel& operator=(const Personnel& personnel);
};
#endif