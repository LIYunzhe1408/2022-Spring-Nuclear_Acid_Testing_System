#include "COVID_19_TestingSystem.h"
// =====================
// Protected
// ��׺�����
void COVID_19_TestingSystem::TestNuclearAcid(QUEUE_TYPE queue_type, std::vector<Personnel>& testing_personnels)
{
	TestTube tube;
	if (queue_type == QUEUE_TYPE::SINGLE)
	{
		tube.OpenOneTube(1, testing_personnels);				// ����һ�����Թ�
		tested_tube_box_.push_back(tube);						// ���ܷ����Թ���
		testing_personnels[0].SetStatus(STATUS::WAITING_RESULT);// �����ϣ������״̬��Ϊ�ȴ�
		tested_personnels_.push_back(testing_personnels[0]);	// �ü���˼����Ѽ��Ⱥ��
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

// ȷ���ܽ�
std::vector<std::string> COVID_19_TestingSystem::DecideCloseContact(const std::vector<std::string>& confirmed_blocks)
{
	std::vector<std::string> close_contact_blocks;
	// ȷ���ܽ�
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
	// �Ŷ�ʱ�ܽ�ȷ��
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

// ȷ�����ܽ�
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
// ��ȡ�����г���
int COVID_19_TestingSystem::GetQueueAmount(QUEUE_TYPE queue_type)
{
	if (queue_type == QUEUE_TYPE::SINGLE)
		return single_testing_queue_.size();
	else if (queue_type == QUEUE_TYPE::MIXED)
		return mixed_testing_queue_.size();
}

// һ����ѡ����ģʽ�������β
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
			personnel.AddCloseContactPersonnel(temp_queue.front());						// ��ǰ�����ǰ���10���ܽ�
			temp_queue.pop();
		}
		single_testing_queue_.push(personnel);
	}
	else if (queue_type == QUEUE_TYPE::MIXED)
	{
		temp_queue = mixed_testing_queue_;
		while (!temp_queue.empty() && personnel.GetCloseContactInQueue().size() < 10)
		{
			personnel.AddCloseContactPersonnel(temp_queue.front());						// ��ǰ�����ǰ���10���ܽ�
			temp_queue.pop();
		}
		mixed_testing_queue_.push(personnel);
	}
}

// �����ᡢ����
void COVID_19_TestingSystem::DelQueue(QUEUE_TYPE queue_type)
{
	std::vector<Personnel> temporary_testing_group;
	std::queue<Personnel> temp_queue;
	if (queue_type == QUEUE_TYPE::SINGLE)
	{
		temp_queue = single_testing_queue_;
		temp_queue.pop();
		if (single_testing_queue_.empty())
			std::cout << "�����Ŷ�" << std::endl;
		temporary_testing_group.push_back(single_testing_queue_.front());// ��ȡ���׵���
		while (!temp_queue.empty())									// �����˶������ٽ�Ⱥ����
		{
			temporary_testing_group[0].AddCloseContactPersonnel(temp_queue.front());
			temp_queue.pop();
		}
		TestNuclearAcid(queue_type, temporary_testing_group);		// ��׺�����

		single_testing_queue_.pop();								// �����ϣ�����
	}
	else if (queue_type == QUEUE_TYPE::MIXED)
	{

		if (mixed_testing_queue_.empty())
			std::cout << "�����Ŷ�" << std::endl;
		int position = 0;
		while (temporary_testing_group.size() < 10 && !mixed_testing_queue_.empty())
		{
			temp_queue = mixed_testing_queue_;
			temp_queue.pop();
			temporary_testing_group.push_back(mixed_testing_queue_.front());
			while (!temp_queue.empty())									// �����˶������ٽ�Ⱥ����
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

// ��������
void COVID_19_TestingSystem::SetTubeResult(const std::vector<std::string> & tube_numbers, TEST_RESULT result)
{
	for(auto&num:tube_numbers)
		for (auto& tube : tested_tube_box_)
		{
			if(tube.GetTubeNum() == num)
				tube.SetTestResult(result);
		}	
}

// ������Ա״̬
void COVID_19_TestingSystem::DecidePersonnelStatus()
{
	// ȷ��ȷ��¥�ţ����ȷ�������Ա
	std::vector<std::string> confirmed_blocks;
	for (auto& tube : tested_tube_box_)
	{
		if (tube.GetTestResult() == TEST_RESULT::POSITIVE && tube.GetPersonnelInThisTube().size() == 1)		// ���˹�����
		{
			Personnel personnel = tube.GetPersonnelInThisTube()[0];  
			for (auto& target_personnel : tested_personnels_)
			{
				if (target_personnel.GetCode() == personnel.GetCode())
				{
					target_personnel.SetStatus(STATUS::COMFIRMED);				// ���˹�ȷ��
					confirmed_blocks.push_back(target_personnel.GetCode());		// ȷ��¥��
				}	
			}
		}
		else if((tube.GetTestResult() == TEST_RESULT::POSITIVE || tube.GetTestResult() == TEST_RESULT::SUSPECTED)
				&& tube.GetPersonnelInThisTube().size() > 1)			// ������Ի����
		{
			for (auto& personnel : tube.GetPersonnelInThisTube())
			{
				for (auto& target_personnel : tested_personnels_)
				{
					if(target_personnel.GetCode() == personnel.GetCode())
						target_personnel.SetStatus(STATUS::SUSPECTED);					// �����ԱΪ����
				}
			}
		}
	}
	std::vector<std::string> close_contact_blocks = DecideCloseContact(confirmed_blocks);
	DecideSubCloseContact(close_contact_blocks);
	DecideNagetive();
}

// ��ѯ
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
		std::cout << " " << cnt << "	" << "���Թܱ��: " << tube.GetTubeNum() << "	״̬�� ";
		TEST_RESULT test_result = tube.GetTestResult();
		if (TEST_RESULT::NEGTIVE == test_result)
			std::cout << "����-Negative" << std::endl;
		if (TEST_RESULT::POSITIVE == test_result)
			std::cout << "����-Positive" << std::endl;
		if (TEST_RESULT::SUSPECTED == test_result)
			std::cout << "����" << std::endl;
		if (TEST_RESULT::TESTING == test_result)
			std::cout << "�ù�δ���" << std::endl;
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
		std::cout << " " << cnt << "	" << "��Ա����: " << personnel.GetCode() << "	״̬�� ";
		STATUS status = personnel.GetStatus();
		if (STATUS::CLOSE_CONTACT == status)
			std::cout << "�ܽ�" << std::endl;
		if (STATUS::COMFIRMED == status)
			std::cout << "ȷ��" << std::endl;
		if (STATUS::NEGATIVE == status)
			std::cout << "����" << std::endl;
		if (STATUS::NOT_TESTED == status)
			std::cout << "δ���" << std::endl;
		if (STATUS::QUEUING == status)
			std::cout << "������" << std::endl;
		if (STATUS::SUB_CLOSE_CONTACT == status)
			std::cout << "���ܽ�" << std::endl;
		if (STATUS::SUSPECTED == status)
			std::cout << "����" << std::endl;
		if (STATUS::WAITING_RESULT == status)
			std::cout << "�ȴ����" << std::endl;
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
	std::cout << "�Ŷ�ʱ���ܽ��ߣ�\n" << std::endl;
	std::cout << "	��Ա���� " << "�ܽ���	" << std::endl;
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