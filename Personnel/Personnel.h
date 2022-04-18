// *
// @Author: 李昀哲 20123101
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
    std::string personnel_code_;	// 人员代码
    STATUS status_;					// 人员状态
public:
    Personnel(std::string personnel_code = "00000000", STATUS status = STATUS::NOT_TESTED);
    void AddCloseContactPersonnel(const Personnel& personnel);		// 添加排队时的临近人员
    vector<Personnel> GetCloseContactInQueue() const;				// 获取临近人员
    STATUS GetStatus();												// 获取当前人员状态
    std::string GetCode();											// 获取当前人员代码
    void SetStatus(STATUS status);									// 设置检测结果
    Personnel& operator=(const Personnel& personnel);
    friend std::istream &operator>>(std::istream  &, Personnel& personnel)
    {
        std::cin >> personnel.personnel_code_;

        return std::cin;
    }
};
#endif