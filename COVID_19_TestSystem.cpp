#include "COVID_19_TestingSystem.h"
// =====================
// Protected
// 大白核酸检测
void COVID_19_TestingSystem::TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels)
{
	TestTube tube;
	if (queue_type == QUEUE_TYPE::SINGLE)
	{
		tube.OpenOneTube(1, testing_personnels);				// 启封一个新试管
		tested_tube_box_.push_back(tube);						// 检测管放入试管箱
		testing_personnels[0].SetStatus(STATUS::WAITING_RESULT);// 检测完毕，检测人状态置为等待
		tested_personnels_.push_back(testing_personnels[0]);	// 该检测人加入已检测群体
	}
	else if (queue_type == QUEUE_TYPE::MIXED)
	{
		tube.OpenOneTube(0, testing_personnels);
		tested_tube_box_.push_back(tube);
		for (auto& personnel : testing_personnels)
		{
			personnel.SetStatus(STATUS::WAITING_RESULT);
			tested_personnels_.push_back(personnel);
		}
	}
}

// 确定密接
std::vector<std::string> COVID_19_TestingSystem::DecideCloseContact(const std::vector<std::string>& confirmed_blocks)
{
	std::vector<std::string> close_contact_blocks;
	// 确定密接
	for (auto& confirmed_block : confirmed_blocks)
	{
		std::string target_block = confirmed_block.substr(0, confirmed_block.length() - 4);
		for (auto& target_personnel : tested_personnels_)
		{
			std::string temp_block = target_personnel.GetCode().substr(0, target_personnel.GetCode().length() - 4);
			if (temp_block == target_block && target_personnel.GetStatus() != STATUS::COMFIRMED)
			{
				target_personnel.SetStatus(STATUS::CLOSE_CONTACT);
				close_contact_blocks.push_back(target_personnel.GetCode());
			}
		}
	}
	// 排队时密接确定
	for (auto& personnel : tested_personnels_)
		if (personnel.GetStatus() == STATUS::COMFIRMED)
		{
			std::vector<Personnel> close_contact_personnels = personnel.GetCloseContactInQueue();
			for (auto& close_contact_personnel : close_contact_personnels)
			{
				for(auto&target_personnel:tested_personnels_)
					if(target_personnel.GetCode() == close_contact_personnel.GetCode() 
						&& target_personnel.GetStatus() != STATUS::COMFIRMED)
						target_personnel.SetStatus(STATUS::CLOSE_CONTACT);
			}
		}

	return close_contact_blocks;
}

// 确定次密接
void COVID_19_TestingSystem::DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks)
{
	for (auto& dangerous_block : close_contact_blocks)
	{
		std::string target_block = dangerous_block.substr(0, dangerous_block.length() - 4);
		for (auto& personnel : tested_personnels_)
		{
			std::string temp_block = personnel.GetCode().substr(0, personnel.GetCode().length() - 4);
			if (temp_block == target_block && personnel.GetStatus() != STATUS::COMFIRMED && personnel.GetStatus() != STATUS::CLOSE_CONTACT)
			{
				personnel.SetStatus(STATUS::SUB_CLOSE_CONTACT);
			}
		}
	}
}


void COVID_19_TestingSystem::DecideNagetive()
{
	for (auto& personnel : tested_personnels_)
	{
		if (personnel.GetStatus() == STATUS::WAITING_RESULT)
			personnel.SetStatus(STATUS::NEGATIVE);
	}
}

// ===================================================================================
// Public
// 获取检测队列长度
int COVID_19_TestingSystem::GetQueueAmount(QUEUE_TYPE queue_type)
{
	if (queue_type == QUEUE_TYPE::SINGLE)
		return single_testing_queue_.size();
	else if (queue_type == QUEUE_TYPE::MIXED)
		return mixed_testing_queue_.size();
}

// 一个人选择检测模式，加入队尾
void COVID_19_TestingSystem::EnQueue(QUEUE_TYPE queue_type, Personnel& personnel)
{
	personnel.SetStatus(STATUS::QUEUING);
	std::queue<Personnel> temp_queue;
	std::vector<Personnel> temp_close_contact_personnels;
	if (queue_type == QUEUE_TYPE::SINGLE)
	{
		temp_queue = single_testing_queue_;
		while (!temp_queue.empty() && personnel.GetCloseContactInQueue().size() < 10)
		{
			personnel.AddCloseContactPersonnel(temp_queue.front());						// 当前入队人前面的10人密接
			temp_queue.pop();
		}
		single_testing_queue_.push(personnel);
	}
	else if (queue_type == QUEUE_TYPE::MIXED)
	{
		temp_queue = mixed_testing_queue_;
		while (!temp_queue.empty() && personnel.GetCloseContactInQueue().size() < 10)
		{
			personnel.AddCloseContactPersonnel(temp_queue.front());						// 当前入队人前面的10人密接
			temp_queue.pop();
		}
		mixed_testing_queue_.push(personnel);
	}
}

// 检测核酸、出队
void COVID_19_TestingSystem::DelQueue(QUEUE_TYPE queue_type)
{
	std::vector<Personnel> temporary_testing_group;
	std::queue<Personnel> temp_queue;
	if (queue_type == QUEUE_TYPE::SINGLE)
	{
		temp_queue = single_testing_queue_;
		temp_queue.pop();
		if (single_testing_queue_.empty())
			std::cout << "无人排队" << std::endl;
		temporary_testing_group.push_back(single_testing_queue_.front());// 获取队首的人
		while (!temp_queue.empty())									// 其后的人都加入临近群体中
		{
			temporary_testing_group[0].AddCloseContactPersonnel(temp_queue.front());
			temp_queue.pop();
		}
		TestNuclearAcid(queue_type, temporary_testing_group);		// 大白核酸检测

		single_testing_queue_.pop();								// 检测完毕，出队
	}
	else if (queue_type == QUEUE_TYPE::MIXED)
	{

		if (mixed_testing_queue_.empty())
			std::cout << "无人排队" << std::endl;
		int position = 0;
		while (temporary_testing_group.size() < 10 && !mixed_testing_queue_.empty())
		{
			temp_queue = mixed_testing_queue_;
			temp_queue.pop();
			temporary_testing_group.push_back(mixed_testing_queue_.front());
			while (!temp_queue.empty())									// 其后的人都加入临近群体中
			{
				temporary_testing_group[position].AddCloseContactPersonnel(temp_queue.front());
				temp_queue.pop();
			}
			position++;
			mixed_testing_queue_.pop();
		}
		TestNuclearAcid(queue_type, temporary_testing_group);
	}
	
}

// 填入检测结果
void COVID_19_TestingSystem::SetTubeResult(const std::vector<std::string> & tube_numbers, TEST_RESULT result)
{
	for(auto&num:tube_numbers)
		for (auto& tube : tested_tube_box_)
		{
			if(tube.GetTubeNum() == num)
				tube.SetTestResult(result);
		}	
}

// 决定人员状态
void COVID_19_TestingSystem::DecidePersonnelStatus()
{
	// 确定确诊楼号，标记确诊、可疑人员
	std::vector<std::string> confirmed_blocks;
	for (auto& tube : tested_tube_box_)
	{
		if (tube.GetTestResult() == TEST_RESULT::POSITIVE && tube.GetPersonnelInThisTube().size() == 1)		// 单人管阳性
		{
			Personnel personnel = tube.GetPersonnelInThisTube()[0];  
			for (auto& target_personnel : tested_personnels_)
			{
				if (target_personnel.GetCode() == personnel.GetCode())
				{
					target_personnel.SetStatus(STATUS::COMFIRMED);				// 单人管确诊
					confirmed_blocks.push_back(target_personnel.GetCode());		// 确诊楼号
				}	
			}
		}
		else if((tube.GetTestResult() == TEST_RESULT::POSITIVE || tube.GetTestResult() == TEST_RESULT::SUSPECTED)
				&& tube.GetPersonnelInThisTube().size() > 1)			// 混检阳性或可疑
		{
			for (auto& personnel : tube.GetPersonnelInThisTube())
			{
				for (auto& target_personnel : tested_personnels_)
				{
					if(target_personnel.GetCode() == personnel.GetCode())
						target_personnel.SetStatus(STATUS::SUSPECTED);					// 混检人员为可疑
				}
			}
		}
	}
	std::vector<std::string> close_contact_blocks = DecideCloseContact(confirmed_blocks);
	DecideSubCloseContact(close_contact_blocks);
	DecideNagetive();
}

// 查询
STATUS COVID_19_TestingSystem::GetPersonnelStatus(const std::string& personnel_code)
{
	for (auto& personnel : tested_personnels_)
	{
		if (personnel.GetCode() == personnel_code)
			return personnel.GetStatus();
	}
	return STATUS::NOT_FOUND;
}

void COVID_19_TestingSystem::DisplayTube()
{
	int cnt = 1;
	std::cout << std::endl;
	for (auto& tube : tested_tube_box_)
	{
		std::cout << " " << cnt << "	" << "测试管编号: " << tube.GetTubeNum() << "	状态： ";
		TEST_RESULT test_result = tube.GetTestResult();
		if (TEST_RESULT::NEGTIVE == test_result)
			std::cout << "阴性-Negative" << std::endl;
		if (TEST_RESULT::POSITIVE == test_result)
			std::cout << "阳性-Positive" << std::endl;
		if (TEST_RESULT::SUSPECTED == test_result)
			std::cout << "可疑" << std::endl;
		if (TEST_RESULT::TESTING == test_result)
			std::cout << "该管未检测" << std::endl;
		cnt++;
	}
		
	std::cout << "===================================================================" << std::endl;
}

void COVID_19_TestingSystem::DisplayPersonnel()
{
	int cnt = 1;
	std::cout << std::endl;
	for (auto& personnel : tested_personnels_)
	{
		std::cout << " " << cnt << "	" << "人员代码: " << personnel.GetCode() << "	状态： ";
		STATUS status = personnel.GetStatus();
		if (STATUS::CLOSE_CONTACT == status)
			std::cout << "密接" << std::endl;
		if (STATUS::COMFIRMED == status)
			std::cout << "确诊" << std::endl;
		if (STATUS::NEGATIVE == status)
			std::cout << "阴性" << std::endl;
		if (STATUS::NOT_TESTED == status)
			std::cout << "未检测" << std::endl;
		if (STATUS::QUEUING == status)
			std::cout << "队列中" << std::endl;
		if (STATUS::SUB_CLOSE_CONTACT == status)
			std::cout << "次密接" << std::endl;
		if (STATUS::SUSPECTED == status)
			std::cout << "可疑" << std::endl;
		if (STATUS::WAITING_RESULT == status)
			std::cout << "等待结果" << std::endl;
		cnt++;
	}

	std::cout << "===================================================================" << std::endl;
}
bool COVID_19_TestingSystem::IsEmpty(QUEUE_TYPE queue_type)
{
	if (queue_type == QUEUE_TYPE::SINGLE)
		return single_testing_queue_.empty();
	else if (queue_type == QUEUE_TYPE::MIXED)
		return mixed_testing_queue_.empty();
}

void COVID_19_TestingSystem::DisplayPersonnelCloser()
{
	int cnt = 1;
	std::cout << "排队时的密接者：\n" << std::endl;
	std::cout << "	人员代码 " << "密接者	" << std::endl;
	for (auto& personnel : tested_personnels_)
	{
		std::cout << " " << cnt << "	" << personnel.GetCode() << ": ";
		for (auto& closer : personnel.GetCloseContactInQueue())
			std::cout << closer.GetCode() << " ";
		std::cout << std::endl;
		cnt++;
	}
	std::cout << "=====================================================================" << std::endl;
}