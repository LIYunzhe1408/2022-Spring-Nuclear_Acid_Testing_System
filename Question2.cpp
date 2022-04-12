// *
// @Author: 李昀哲 20123101
// Created at Apr.12.2022
// Attention.
// *
#include "Personnel.h"
#include "TestTube.h"
#include "COVID_19_TestingSystem.h"
int main()
{
	/// 流程
	/// Input人员的信息，personnel_code
	/// 去排队做核酸
	/// 等结果（编辑测试管结果）
	/// 根据结果确定检测人员状态（密接、次密接、确诊、阴性等）
	/// 输出结果 
	
	
	// ==============================================
	// 输入人员信息
	std::vector<Personnel> personnels;
	Personnel *person;
	for (int i = 0; i < 10; i++)
	{
		person = new Personnel("02"+ std::to_string(i) +"605"+std::to_string(i));
		personnels.push_back(*person);
	}
	Personnel personx("0202031"), persony("0284035");
	personnels.push_back(personx);
	personnels.push_back(persony);

	// ==============================================
	// 启动系统，排队做核酸
	COVID_19_TestingSystem TestingSystem;
	QUEUE_TYPE queue_type = QUEUE_TYPE::MIXED;
	for (auto& personnel : personnels)
		TestingSystem.EnQueue(queue_type, personnel);
	while(!TestingSystem.IsEmpty(queue_type))
		TestingSystem.DelQueue(queue_type);

	// ==============================================
	// 等待结果（自己编辑）
	std::vector<std::string> positive_num, negative_num;
	negative_num.push_back("0000" + std::to_string(0));
	positive_num.push_back("00001");


	// ==============================================
	// 根据结果处理信息
	TestingSystem.SetTubeResult(positive_num, TEST_RESULT::POSITIVE);
	TestingSystem.SetTubeResult(negative_num, TEST_RESULT::NEGTIVE);
	TestingSystem.DecidePersonnelStatus();


	// ==============================================
	// 输出
	TestingSystem.DisplayPersonnelCloser();
	TestingSystem.DisplayTube();
	TestingSystem.DisplayPersonnel();
	return 0;
}
