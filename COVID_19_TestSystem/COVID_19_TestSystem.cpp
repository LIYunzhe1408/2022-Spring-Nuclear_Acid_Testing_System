#include "COVID_19_TestSystem/COVID_19_TestingSystem.h"
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
        std::string target_block = confirmed_block.substr(0, confirmed_block.length() - 5);
        for (auto& target_personnel : tested_personnels_)
        {
            std::string temp_block = target_personnel.GetCode().substr(0, target_personnel.GetCode().length() - 5);
            if (temp_block == target_block
                && target_personnel.GetStatus() != STATUS::COMFIRMED
                && target_personnel.GetStatus() != STATUS::SUSPECTED)
            {
                target_personnel.SetStatus(STATUS::CLOSE_CONTACT);
                close_contact_blocks.push_back(target_personnel.GetCode());
            }
        }
    }
    // �Ŷ�ʱ�ܽ�ȷ��
    for (auto& personnel : confirmed_personnels_)   // �������࣬ʱ�临�Ӷȸ߲���̫Ӱ��
    {
        std::vector<Personnel> close_contact_personnels = personnel.GetCloseContactInQueue();
        for (auto& close_contact_personnel : close_contact_personnels)
        {
            for (auto& target_personnel : tested_personnels_)
                if (target_personnel.GetCode() == close_contact_personnel.GetCode()
                    && target_personnel.GetStatus() != STATUS::COMFIRMED
                    && target_personnel.GetStatus() != STATUS::SUSPECTED)
                {
                    target_personnel.SetStatus(STATUS::CLOSE_CONTACT);
                    close_contact_blocks.push_back(target_personnel.GetCode());
                }

        }
    }
    return close_contact_blocks;
}

// ȷ�����ܽ�
void COVID_19_TestingSystem::DecideSubCloseContact(const std::vector<std::string>& close_contact_blocks)
{
    for (auto& dangerous_block : close_contact_blocks)
    {
        std::string target_block = dangerous_block.substr(0, dangerous_block.length() - 5);
        for (auto& personnel : tested_personnels_)
        {
            std::string temp_block = personnel.GetCode().substr(0, personnel.GetCode().length() - 5);
            if (temp_block == target_block
                && personnel.GetStatus() != STATUS::COMFIRMED
                && personnel.GetStatus() != STATUS::CLOSE_CONTACT
                && personnel.GetStatus() != STATUS::SUSPECTED)
            {
                personnel.SetStatus(STATUS::SUB_CLOSE_CONTACT);
            }
        }
    }
}

// ȷ������
void COVID_19_TestingSystem::DecideNegative()
{
    for (auto& personnel : tested_personnels_)
    {
        if (personnel.GetStatus() != STATUS::CLOSE_CONTACT
            && personnel.GetStatus() != STATUS::COMFIRMED
            && personnel.GetStatus() != STATUS::SUB_CLOSE_CONTACT
            && personnel.GetStatus() != STATUS::SUSPECTED)
            personnel.SetStatus(STATUS::NEGATIVE);
    }
}

// ��������
void COVID_19_TestingSystem::SetTubeResult(TestTube &tube, TEST_RESULT result)
{
    if (tube.GetTubeNum()[0] == '0')
        result = TEST_RESULT::SUSPECTED;
    tube.SetTestResult(result);
}

void COVID_19_TestingSystem::UpdateAllTypeInfo()
{
    std::vector<Personnel> confirmed_personnels;
    std::vector<Personnel> negative_personnels;
    std::vector<Personnel> suspected_personnels;
    std::vector<Personnel> close_contact_personnels;
    std::vector<Personnel> sub_close_contact_personnels;
    std::vector<Personnel> need_to_be_uploaded_personnels;
    std::vector<Personnel> queueing_personnels;

    std::queue<Personnel> temp_queue;
    temp_queue = single_testing_queue_;
    while (!temp_queue.empty())
    {
        queueing_personnels.push_back(temp_queue.front());
        temp_queue.pop();
    }
    temp_queue = mixed_testing_queue_;
    while (!temp_queue.empty())
    {
        queueing_personnels.push_back(temp_queue.front());
        temp_queue.pop();
    }
    for (auto& personnel : tested_personnels_)
    {
        if (personnel.GetStatus() == STATUS::WAITING_RESULT)
            need_to_be_uploaded_personnels.push_back(personnel);
        else if (personnel.GetStatus() == STATUS::NEGATIVE)
            negative_personnels.push_back(personnel);
        else if (personnel.GetStatus() == STATUS::COMFIRMED)
            confirmed_personnels.push_back(personnel);
        else if (personnel.GetStatus() == STATUS::CLOSE_CONTACT)
            close_contact_personnels.push_back(personnel);
        else if (personnel.GetStatus() == STATUS::SUB_CLOSE_CONTACT)
            sub_close_contact_personnels.push_back(personnel);
        else if (personnel.GetStatus() == STATUS::SUSPECTED)
            suspected_personnels.push_back(personnel);
    }
    negative_personnels_ = negative_personnels;
    confirmed_personnels_ = confirmed_personnels;
    close_contact_personnels_ = close_contact_personnels;
    sub_close_contact_personnels_ = sub_close_contact_personnels;
    need_to_be_uploaded_personnels_ = need_to_be_uploaded_personnels;
    queueing_personnels_ = queueing_personnels;
    suspected_personnels_ = suspected_personnels;
}



void COVID_19_TestingSystem::DisplayType(STATUS status)
{
    std::string name;
    int cnt = 1;
    std::vector<Personnel> temp_vector;
    if (status == STATUS::WAITING_RESULT)
    {
        temp_vector = need_to_be_uploaded_personnels_;
        name = "���ϴ�";
    }
    else if (status == STATUS::NEGATIVE)
    {
        temp_vector = negative_personnels_;
        name = "����";
    }
    else if (status == STATUS::COMFIRMED)
    {
        temp_vector = confirmed_personnels_;
        name = "ȷ��";
    }
    else if (status == STATUS::CLOSE_CONTACT)
    {
        temp_vector = close_contact_personnels_;
        name = "�ܽ�";
    }
    else if (status == STATUS::SUB_CLOSE_CONTACT)
    {
        temp_vector = sub_close_contact_personnels_;
        name = "���ܽ�";
    }
    else if (status == STATUS::SUSPECTED)
    {
        temp_vector = suspected_personnels_;
        name = "����";
    }
    else if (status == STATUS::QUEUING)
    {
        temp_vector = queueing_personnels_;
        name = "������";
    }
    else
        return;
    if (temp_vector.empty())
        std::cout << "<Info>  ��" + name + "��Ա��\n";
    else
    {
        std::cout << name << ": \n";
        std::cout << "���\t" << "��Ա����\n";

        for (auto& it : temp_vector)
        {
            std::cout << cnt << '\t' << it.GetCode() << std::endl;
            cnt++;
        }
    }
    std::cout << "-------------------------------------------------------------------" << std::endl;
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

// ��ȡ�Ѽ����Ա����
int COVID_19_TestingSystem::GetTestedPersonnelAmount() const
{
    return	tested_personnels_.size();
}



int COVID_19_TestingSystem::GetTestedTubeAmount() const
{
    return	tested_tube_box_.size();
}

// һ����ѡ����ģʽ�������β
void COVID_19_TestingSystem::EnQueue(QUEUE_TYPE queue_type, Personnel personnel)
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
// ������Թ�
void COVID_19_TestingSystem::RandomizeTubeInfo()
{
    double positive_num = tested_personnels_.size() * kPOSITIVE_PROBABILITY;
    if (positive_num < 1)
        positive_num = 1;
    std::random_device rd;
    std::mt19937 random(rd());
    std::uniform_int_distribution<int> distrib(0, tested_tube_box_.size()-1); // ָ����Χ
    for (int i = 0; i < positive_num; i++)
    {
        SetTubeResult(tested_tube_box_[distrib(random)], TEST_RESULT::POSITIVE);
    }

    for (auto& tube : tested_tube_box_)
        if (tube.GetTestResult() != TEST_RESULT::POSITIVE && tube.GetTestResult() != TEST_RESULT::SUSPECTED)
            tube.SetTestResult(TEST_RESULT::NEGTIVE);
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
                    confirmed_personnels_.push_back(target_personnel);
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
    DecideNegative();
}

// ��ѯ
STATUS COVID_19_TestingSystem::GetPersonnelStatus(const std::string& personnel_code)
{
    std::queue<Personnel>temp_queue = single_testing_queue_;
    for (auto& personnel : tested_personnels_)
    {
        if (personnel.GetCode() == personnel_code)
            return personnel.GetStatus();
    }

    while(!temp_queue.empty())
    {
        if (personnel_code == temp_queue.front().GetCode())
            return temp_queue.front().GetStatus();
        temp_queue.pop();
    }
    temp_queue = mixed_testing_queue_;
    while (!temp_queue.empty())
    {
        if (personnel_code == temp_queue.front().GetCode())
            return temp_queue.front().GetStatus();
        temp_queue.pop();
    }
    return STATUS::NOT_FOUND;
}

void COVID_19_TestingSystem::DisplayStatus(const std::string& personnel_code)
{
    STATUS status = GetPersonnelStatus(personnel_code);
    std::cout << "-------------------------------------------------\n";
    if (status == STATUS::WAITING_RESULT)
    {
        std::cout << personnel_code << ": " <<  "���ϴ�";
    }
    else if (status == STATUS::NEGATIVE)
    {
        std::cout << personnel_code << ": " << "����";
    }
    else if (status == STATUS::COMFIRMED)
    {
        std::cout << personnel_code << ": " << "ȷ��";
    }
    else if (status == STATUS::CLOSE_CONTACT)
    {
        std::cout << personnel_code << ": " << "�ܽ�";
    }
    else if (status == STATUS::SUB_CLOSE_CONTACT)
    {
        std::cout << personnel_code << ": " << "���ܽ�";
    }
    else if (status == STATUS::SUSPECTED)
    {
        std::cout << personnel_code << ": " << "����";
    }
    else if (status == STATUS::NOT_FOUND)
    {
        std::cout << "<Error> " << personnel_code << ": " << "δ�ҵ�����Ա";
    }
    else if (status == STATUS::NOT_TESTED)
    {
        std::cout << personnel_code << ": " << "δ���";
    }
    else if (status == STATUS::QUEUING)
    {
        std::cout << personnel_code << ": " << "������";
    }
    std::cout << std::endl;
}

void COVID_19_TestingSystem::DisplayTube()
{
    std::cout << "=========================== ��ǰ�Թ���Ϣ ==========================" << std::endl;
    int cnt = 1;
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
    std::cout << "=========================== ��ǰ��Ա��Ϣ ==========================" << std::endl;
    if (tested_personnels_.empty())
        std::cout << "��ǰû����Աǰ�����\n";
    int cnt = 1;
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
    std::cout << "=====================================================================" << std::endl;
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

void COVID_19_TestingSystem::DisplayAllTypePersonnel()
{
    UpdateAllTypeInfo();
    std::cout << "=========================== ��Ա��Ϣ ==============================" << std::endl;
    for (int i = 0; i < 9; i++)
        DisplayType(static_cast<STATUS>(i));
    std::cout << "===================================================================" << std::endl;
}

void COVID_19_TestingSystem::DisplayQueueing()
{
    std::cout << "-----------------------------------------------------\n";
    int column = 1;
    std::queue<Personnel>temp_queue = mixed_testing_queue_;

    std::cout << "�����У�\n";
    std::cout << temp_queue.front().GetCode();
    temp_queue.pop();
    while (!temp_queue.empty())
    {
        std::cout <<  " -> " << temp_queue.front().GetCode();
        temp_queue.pop();
        if (column % 5 == 0)
            std::cout << '\n';
        column++;
    }

    std::cout << '\n';
    std::cout << "-----------------------------------------------------\n";
    column = 1;
    temp_queue = single_testing_queue_;
    std::cout << "������У�\n";
    std::cout << temp_queue.front().GetCode();
    temp_queue.pop();
    while (!temp_queue.empty())
    {
        std::cout << " -> " << temp_queue.front().GetCode();
        temp_queue.pop();
        if (column % 5 == 0)
            std::cout << '\n';
        column++;
    }
    std::cout << '\n';
}
