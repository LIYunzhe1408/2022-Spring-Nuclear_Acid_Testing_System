#include "COVID_19_TestingSystem.h"
// =====================
// Protected

// --------------------------��ʼ���׶�----------------------------------------------
// ������ʷ��Ϣ��������״̬��
void COVID_19_TestingSystem::UpdateHistoryInfo()
{
    std::cout << "==========================================\n";
    std::string file_name = nucleic_acid_test_file_name_;
    std::ifstream inFile(file_name);
    UpdateAmount(inFile);
    UpdateTestedResult(inFile);
    inFile.close();
    std::cout << "��ʼ������ɡ�\n";
    std::cout << "------------------------------------------\n";
    std::cout << "<Info>  Uploaded from ( " + file_name << " )\n";
    std::cout << "==========================================\n";
    system("pause");
    system("cls");
}

// �����Ѽ��������Ϣ
void COVID_19_TestingSystem::UpdateAmount(std::ifstream& inFile)
{
    int mixed_amount = 0, single_amount = 0;
    std::string line;
    getline(inFile, line);
    std::stringstream ss;
    ss << line;
    int cnt = 0;
    while (!ss.eof())
    {
        int temp;
        while (ss >> temp)
        {
            if (cnt == 0)
            {
                mixed_amount = temp; cnt++;
            }
            if (cnt == 1)
                single_amount = temp;
        }
    }
    tested_mixed_amount_ = mixed_amount;
    tested_single_amount_ = single_amount;
}

// �����Ѽ������Ϣ
void COVID_19_TestingSystem::UpdateTestedResult(std::ifstream& inFile)
{
    char c[90];
    for (int i = 0; i < 2; i++) inFile.getline(c, 90);
    while (inFile.getline(c, 90))
    {
        std::string string1, string2;
        for (int i = 0; i < 8; i++)
            string1 += c[i];
        for (int i = 9; i < 13; i++)
            string2 += c[i];
        if (string1 != " ")
        {
            Personnel personnel(string1); STATUS status;
            if (string2 == "ȷ��")
                status = STATUS::COMFIRMED;
            else if (string2 == "�ܽ�")
                status = STATUS::CLOSE_CONTACT;
            else if (string2 == "���ܽ�")
                status = STATUS::SUB_CLOSE_CONTACT;
            else if (string2 == "����")
                status = STATUS::NEGATIVE;
            else if (string2 == "δ���")
                status = STATUS::NOT_TESTED;
            else if (string2 == "����")
                status = STATUS::SUSPECTED;
            else if (string2 == "������")
                status = STATUS::QUEUING;
            else if (string2 == "�ȴ����")
                status = STATUS::WAITING_RESULT;
            personnel.SetStatus(status);
            AddTestedPersonnel(personnel);
        }
    }
}

// �����Ѽ����Ա�����ڳ�ʼ���׶��ļ����룩
void COVID_19_TestingSystem::AddTestedPersonnel(const Personnel& personnel)
{
    tested_personnels_.push_back(personnel);
}


// --------------------------��ʾ�˵�--------------------------------------------------
// ��ȡ�����г���
int COVID_19_TestingSystem::GetQueueAmount(QUEUE_TYPE queue_type)
{
    if (queue_type == QUEUE_TYPE::SINGLE)
        return single_testing_queue_.size();
    else if (queue_type == QUEUE_TYPE::MIXED)
        return mixed_testing_queue_.size();
}

// ��ȡ��ɻ���������
int COVID_19_TestingSystem::GetTestedMixedAmount() const
{
    return tested_mixed_amount_;
}

// ��ȡ��ɵ��������
int	COVID_19_TestingSystem::GetTestedSingleAmount() const
{
    return tested_single_amount_;
}

// ��ȡ�Ѽ����Ա��������
int COVID_19_TestingSystem::GetTestedPersonnelAmount() const
{
    return	GetTestedSingleAmount() + GetTestedMixedAmount();
}

// --------------------------��ʼ������-------------------------------------------------
// ���ļ���ʼ��������Ϣ
void COVID_19_TestingSystem::LineUpFromFile(const std::string& file_name)
{
    std::ifstream inFile(file_name);
    int mixed_amount = 0, single_amount = 0;
    std::string line;
    getline(inFile, line);
    std::stringstream ss;
    ss << line;
    int cnt = 0;
    while (!ss.eof())
    {
        int temp;
        while (ss >> temp)
        {
            if (cnt == 0)
            {
                mixed_amount = temp; cnt++;
            }
            if (cnt == 1)
                single_amount = temp;
        }
    }
    char c[10]; cnt = 1;
    while (inFile.getline(c, 10))
    {
        std::string string;
        for (int i = 0; i < 8; i++)
            string += c[i];
        Personnel personnel(string);
        if (cnt <= mixed_amount)
            EnQueue(QUEUE_TYPE::MIXED, personnel);
        else
            EnQueue(QUEUE_TYPE::SINGLE, personnel);
        cnt++;
    }
}

// --------------------------����1------------------------------------------------------
// һ����ѡ����ģʽ�������β
void COVID_19_TestingSystem::EnQueue(QUEUE_TYPE queue_type, Personnel personnel)
{
    personnel.SetStatus(STATUS::QUEUING);
    queue<Personnel> temp_queue;
    vector<Personnel> temp_close_contact_personnels;
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

// --------------------------����2------------------------------------------------------
// �����ᡢ����
void COVID_19_TestingSystem::DelQueue(QUEUE_TYPE queue_type)
{
    vector<Personnel> temporary_testing_group;
    queue<Personnel> temp_queue;
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

        tested_single_amount_++;
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
            tested_mixed_amount_++;
            mixed_testing_queue_.pop();
        }
        TestNuclearAcid(queue_type, temporary_testing_group);
    }

}

// ��׺�����
void COVID_19_TestingSystem::TestNuclearAcid(QUEUE_TYPE queue_type, vector<Personnel>& testing_personnels)
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

// --------------------------����3------------------------------------------------------
// ��ʾ�������
void COVID_19_TestingSystem::DisplayQueueing()
{
    int column = 1;
    queue<Personnel>temp_queue = mixed_testing_queue_;

    if (mixed_testing_queue_.empty())
        std::cout << "<Error> ��ǰ������Ϊ�ա�\n";
    if (single_testing_queue_.empty())
        std::cout << "<Error> ��ǰ�������Ϊ�ա�\n";
    if (mixed_testing_queue_.empty() && single_testing_queue_.empty())
        return;
    std::cout << "-----------------------------------------------------\n";

    if (!mixed_testing_queue_.empty())
    {
        std::cout << "�����У�\n";
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
    }
    if (!single_testing_queue_.empty())
    {
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
    }
    std::cout << '\n';
}


// --------------------------����4------------------------------------------------------
// ��ȡ�Ѽ���Թܵ�����
int COVID_19_TestingSystem::GetTestedTubeAmount()
{
    return	tested_tube_box_.size();
}

// ������Թ�
void COVID_19_TestingSystem::RandomizeTubeInfo()
{
    double positive_num = tested_personnels_.size() * kPOSITIVE_PROBABILITY;
    if (positive_num < 1)
        positive_num = 1;
    std::random_device rd;
    std::mt19937 random(rd());
    std::uniform_int_distribution<int> distrib(0, tested_tube_box_.size() - 1); // ָ����Χ
    for (int i = 0; i < positive_num; i++)
    {
        SetTubeResult(tested_tube_box_[distrib(random)], TEST_RESULT::POSITIVE);
    }

    for (auto& tube : tested_tube_box_)
        if (tube.GetTestResult() != TEST_RESULT::POSITIVE
            && tube.GetTestResult() != TEST_RESULT::SUSPECTED
            && tube.IsTested() == false)
        {
            tube.SetTestResult(TEST_RESULT::NEGTIVE);
            tube.SetTested();
        }
}

// ��������
void COVID_19_TestingSystem::SetTubeResult(TestTube& tube, TEST_RESULT result)
{
    if (!tube.IsTested())
    {
        if (tube.GetTubeNum()[0] == '0')
            result = TEST_RESULT::SUSPECTED;
        tube.SetTestResult(result);
        tube.SetTested();
    }

}

// ��ʾ������Ϣ
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

// ������Ա״̬
void COVID_19_TestingSystem::DecidePersonnelStatus()
{
    // ȷ��ȷ��¥�ţ����ȷ�������Ա
    vector<std::string> confirmed_blocks;
    for (auto& tube : tested_tube_box_)
    {
        if (tube.GetTestResult() == TEST_RESULT::POSITIVE && tube.GetPersonnelInThisTube().size() == 1)		// ���˹�����
        {
            Personnel personnel = tube.GetPersonnelInThisTube()[0];
            for (auto& target_personnel : tested_personnels_)
            {
                if (target_personnel.GetCode() == personnel.GetCode())
                {
                    target_personnel.SetStatus(STATUS::COMFIRMED);				    // ���˹�ȷ��
                }
                if (target_personnel.GetStatus() == STATUS::COMFIRMED)
                {
                    confirmed_personnels_.push_back(target_personnel);
                    confirmed_blocks.push_back(target_personnel.GetCode());		// ȷ��¥��
                }
            }
        }
        else if ((tube.GetTestResult() == TEST_RESULT::POSITIVE || tube.GetTestResult() == TEST_RESULT::SUSPECTED)
                 && tube.GetPersonnelInThisTube().size() > 1)			// ������Ի����
        {
            for (auto& personnel : tube.GetPersonnelInThisTube())
                for (auto& target_personnel : tested_personnels_)
                    if (target_personnel.GetCode() == personnel.GetCode())
                        target_personnel.SetStatus(STATUS::SUSPECTED);					// �����ԱΪ����
        }
    }
    vector<std::string> close_contact_blocks = DecideCloseContact(confirmed_blocks);
    DecideSubCloseContact(close_contact_blocks);
    DecideNegative();
}

// ȷ���ܽ�
vector<std::string> COVID_19_TestingSystem::DecideCloseContact(vector<std::string> confirmed_blocks)
{
    vector<std::string> close_contact_blocks;
    // ȷ��¥���ܽ�
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
    // ȷ���Ŷ��ܽ�
    for (auto& personnel : confirmed_personnels_)   // �������࣬ʱ�临�Ӷȸ߲���̫Ӱ��
    {
        vector<Personnel> close_contact_personnels = personnel.GetCloseContactInQueue();
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
void COVID_19_TestingSystem::DecideSubCloseContact(vector<std::string> close_contact_blocks)
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
                personnel.SetStatus(STATUS::SUB_CLOSE_CONTACT);
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


// --------------------------����5------------------------------------------------------
// �����������͵���Ա��Ϣ
void COVID_19_TestingSystem::UpdateAllTypeInfo()
{
    vector<Personnel> confirmed_personnels;
    vector<Personnel> negative_personnels;
    vector<Personnel> suspected_personnels;
    vector<Personnel> close_contact_personnels;
    vector<Personnel> sub_close_contact_personnels;
    vector<Personnel> need_to_be_uploaded_personnels;
    vector<Personnel> queueing_personnels;

    queue<Personnel> temp_queue;
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

// ���������ʾ������Ϣ
void COVID_19_TestingSystem::DisplayType(STATUS status)
{
    std::string name;
    int cnt = 1;
    vector<Personnel> temp_vector;
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
    if (temp_vector.size() == 0)
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


// --------------------------����6------------------------------------------------------
// ��ȡָ����Ա״̬
STATUS COVID_19_TestingSystem::GetPersonnelStatus(const std::string& personnel_code)
{
    queue<Personnel>temp_queue = single_testing_queue_;
    for (auto& personnel : tested_personnels_)
    {
        if (personnel.GetCode() == personnel_code)
            return personnel.GetStatus();
    }
    while (!temp_queue.empty())
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

// ��ʾָ����Ա״̬
void COVID_19_TestingSystem::DisplayStatus(const std::string& personnel_code)
{
    STATUS status = GetPersonnelStatus(personnel_code);
    std::cout << "-------------------------------------------------\n";
    if (status == STATUS::WAITING_RESULT)
    {
        std::cout << personnel_code << ": " << "���ϴ�";
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



// --------------------------����------------------------------------------------------
// ����ǰ��Ա��Ϣ����
void COVID_19_TestingSystem::SaveToFile(const std::string& file_name)
{
    //�ļ����루д���ļ��
    std::ofstream fout;
    fout.open(file_name);
    fout << tested_mixed_amount_ << " " << tested_single_amount_ << '\n';
    fout << "=========================== ��ǰ��Ա��Ϣ ==========================" << std::endl;

    fout << "��Ա����	" << "״̬\n";
    for (auto & it :tested_personnels_)
    {
        fout << it.GetCode() << "	";
        STATUS status = it.GetStatus();
        if (STATUS::CLOSE_CONTACT == status)
            fout << "�ܽ�" << std::endl;
        if (STATUS::COMFIRMED == status)
            fout << "ȷ��" << std::endl;
        if (STATUS::NEGATIVE == status)
            fout << "����" << std::endl;
        if (STATUS::NOT_TESTED == status)
            fout << "δ���" << std::endl;
        if (STATUS::QUEUING == status)
            fout << "������" << std::endl;
        if (STATUS::SUB_CLOSE_CONTACT == status)
            fout << "���ܽ�" << std::endl;
        if (STATUS::SUSPECTED == status)
            fout << "����" << std::endl;
        if (STATUS::WAITING_RESULT == status)
            fout << "�ȴ����" << std::endl;
    }
    fout.close();
}


// --------------------------��������---------------------------------------------------
// ��ʾȫ����Ա��Ϣ
void COVID_19_TestingSystem::DisplayPersonnel()
{
    std::cout << "=========================== ��ǰ��Ա��Ϣ ==========================" << std::endl;
    if (tested_personnels_.size() == 0)
        std::cout << "��ǰû����Աǰ�����\n";
    int cnt = 1;
    for (auto&personnel : tested_personnels_)
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

// ���ض����Ƿ�Ϊ��
bool COVID_19_TestingSystem::IsEmpty(QUEUE_TYPE queue_type)
{
    if (queue_type == QUEUE_TYPE::SINGLE)
        return single_testing_queue_.empty();
    else if (queue_type == QUEUE_TYPE::MIXED)
        return mixed_testing_queue_.empty();
}

// ��ʾ��Ա�Ŷ�ʱ���ܽ���
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











// =================================================================================================
// =================================================================================================
// Public

// ��ʾ�˵�
void COVID_19_TestingSystem::DisplayMenu()
{
    std::cout << "======================������ϵͳ=======================\n";
    std::cout << "==================   *0. ��ʼ���Ŷ�     =================\n";
    std::cout << "==================    1. �Ŷ�           =================\n";
    std::cout << "==================    2. ���           =================\n";
    std::cout << "==================    3. �鿴�Ŷ����   =================\n";
    std::cout << "==================    4. �Ǽǲ��Թ���Ϣ =================\n";
    std::cout << "==================    5. ������Ա��ѯ   =================\n";
    std::cout << "==================    6. ���˲�ѯ       =================\n";
    std::cout << "==================    s. �ļ�����       =================\n";
    std::cout << "==================    q. �˳�ϵͳ       =================\n";
    std::cout << "=========================================================\n";
    std::cout << "===��ǰ���������" << GetQueueAmount(QUEUE_TYPE::MIXED) << "\t";
    std::cout << "��ǰ����������" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===��ǰ�Ѽ�������" << GetTestedPersonnelAmount()
              << " <�������: " << GetTestedMixedAmount() << " ��������: " << GetTestedSingleAmount()
              << ">\n";
    std::cout << "===��ѡ���ܣ� ";
}

// ����0����ʼ������
void COVID_19_TestingSystem::InitQueue()
{
    std::string file_name = line_up_file_name_;
    LineUpFromFile(file_name);
    std::cout << "===================================================================" << std::endl;
    std::cout << "----��ʼ���ļ��������----" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "----<Info>  Uploaded from ( " + file_name + " )" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "===��ǰ���������" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "===��ǰ����������" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===================================================================" << std::endl;
}

// ����1�����������Ŷ���Ա
void COVID_19_TestingSystem::AddQueuePersonnel()
{
    Personnel personnel;
    int mixed_amount, single_amount;

    std::cout << "����������Ա�͵�����Ա������m n)��";
    std::cin >> mixed_amount >> single_amount;
    std::cout << "������/��Ա���룺\n";
    for (int i = 0; i < mixed_amount; i++)
    {
        std::cin >> personnel;
        EnQueue(QUEUE_TYPE::MIXED, personnel);
    }
    for (int i = 0; i < single_amount; i++)
    {
        std::cin >> personnel;
        EnQueue(QUEUE_TYPE::SINGLE, personnel);
    }
    system("cls");
    std::cout << "===================================================================" << std::endl;
    std::cout << "----�������----" << std::endl;
    std::cout << "===��ǰ���������" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "===��ǰ����������" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===================================================================" << std::endl;
}

// ����2�����
void COVID_19_TestingSystem::Testing()
{
    std::cout << "===================================================================" << std::endl;
    if (GetQueueAmount(QUEUE_TYPE::MIXED) == 0)
        std::cout << "<Error> ��ǰ��������Ŷӣ��޷���⡣\n";
    if (GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
        std::cout << "<Error> ��ǰ���������Ŷӣ��޷���⡣\n";
    if (GetQueueAmount(QUEUE_TYPE::MIXED) == 0
        && GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
        return;
    // ���
    while (!IsEmpty(QUEUE_TYPE::MIXED))
        DelQueue(QUEUE_TYPE::MIXED);
    std::cout << "<Info>  �����ɡ�\n";
    // ����
    while (!IsEmpty(QUEUE_TYPE::SINGLE))
        DelQueue(QUEUE_TYPE::SINGLE);
    std::cout << "<Info>  ������ɡ�\n";
    DisplayPersonnel();
    std::cout << "===================================================================" << std::endl;
}

// ����3���鿴�Ŷ����
void COVID_19_TestingSystem::ShowQueueStatus()
{
    std::cout << "===================================================================" << std::endl;
    std::cout << "=��ǰ������������" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "=��ǰ�������������" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    DisplayQueueing();
    std::cout << "===================================================================" << std::endl;
}

// ����4���Ǽ���Ա��Ϣ
void COVID_19_TestingSystem::RegisterPersonnelInfo()
{
    std::cout << "===================================================================" << std::endl;
    if (GetTestedTubeAmount() == 0)
        std::cout << "<Info>  �Ѽ�������Ϊ�գ����ȼ���Թܡ�\n";
    else
    {
        RandomizeTubeInfo();
        DisplayTube();
        DecidePersonnelStatus();
        DisplayPersonnel();
    }
    std::cout << "===================================================================" << std::endl;
}

// ����5����ʾ����������Ա��Ϣ
void COVID_19_TestingSystem::DisplayAllTypePersonnel()
{
    UpdateAllTypeInfo();
    std::cout << "=========================== ��Ա��Ϣ ==============================" << std::endl;
    for (int i = 0; i < 9; i++)
        DisplayType(static_cast<STATUS>(i));
    std::cout << "===================================================================" << std::endl;
}

// ����6����ʾָ����Ա��Ϣ
void COVID_19_TestingSystem::DisplaySpecificPersonnel()
{
    std::string personnel_code;
    std::cout << "===================================================================" << std::endl;
    std::cout << "��������Ա���루xxxyyyyz -- ¥��/�����/��ţ�:"; std::cin >> personnel_code;
    DisplayStatus(personnel_code);
    std::cout << "===================================================================" << std::endl;
}

// ����
void COVID_19_TestingSystem::Save()
{
    std::string file_name = nucleic_acid_test_file_name_;
    std::cout << "===================================================================" << std::endl;
    DisplayPersonnel();
    SaveToFile(file_name);
    std::cout << "===================================================================" << std::endl;
}