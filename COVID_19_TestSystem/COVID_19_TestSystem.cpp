#include "COVID_19_TestingSystem.h"
// =====================
// Protected

// --------------------------初始化阶段----------------------------------------------
// 更新历史信息（人数、状态）
void COVID_19_TestingSystem::UpdateHistoryInfo()
{
    std::cout << "==========================================\n";
    std::string file_name = nucleic_acid_test_file_name_;
    std::ifstream inFile(file_name);
    UpdateAmount(inFile);
    UpdateTestedResult(inFile);
    inFile.close();
    std::cout << "初始化已完成。\n";
    std::cout << "------------------------------------------\n";
    std::cout << "<Info>  Uploaded from ( " + file_name << " )\n";
    std::cout << "==========================================\n";
    system("pause");
    system("cls");
}

// 更新已检测人数信息
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

// 更新已检测结果信息
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
            if (string2 == "确诊")
                status = STATUS::COMFIRMED;
            else if (string2 == "密接")
                status = STATUS::CLOSE_CONTACT;
            else if (string2 == "次密接")
                status = STATUS::SUB_CLOSE_CONTACT;
            else if (string2 == "阴性")
                status = STATUS::NEGATIVE;
            else if (string2 == "未检测")
                status = STATUS::NOT_TESTED;
            else if (string2 == "可疑")
                status = STATUS::SUSPECTED;
            else if (string2 == "队列中")
                status = STATUS::QUEUING;
            else if (string2 == "等待结果")
                status = STATUS::WAITING_RESULT;
            personnel.SetStatus(status);
            AddTestedPersonnel(personnel);
        }
    }
}

// 增加已检测人员（用于初始化阶段文件读入）
void COVID_19_TestingSystem::AddTestedPersonnel(const Personnel& personnel)
{
    tested_personnels_.push_back(personnel);
}


// --------------------------显示菜单--------------------------------------------------
// 获取检测队列长度
int COVID_19_TestingSystem::GetQueueAmount(QUEUE_TYPE queue_type)
{
    if (queue_type == QUEUE_TYPE::SINGLE)
        return single_testing_queue_.size();
    else if (queue_type == QUEUE_TYPE::MIXED)
        return mixed_testing_queue_.size();
}

// 获取完成混检的总人数
int COVID_19_TestingSystem::GetTestedMixedAmount() const
{
    return tested_mixed_amount_;
}

// 获取完成单检的人数
int	COVID_19_TestingSystem::GetTestedSingleAmount() const
{
    return tested_single_amount_;
}

// 获取已检测人员的总人数
int COVID_19_TestingSystem::GetTestedPersonnelAmount() const
{
    return	GetTestedSingleAmount() + GetTestedMixedAmount();
}

// --------------------------初始化队列-------------------------------------------------
// 从文件初始化队列信息
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

// --------------------------功能1------------------------------------------------------
// 一个人选择检测模式，加入队尾
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

// --------------------------功能2------------------------------------------------------
// 检测核酸、出队
void COVID_19_TestingSystem::DelQueue(QUEUE_TYPE queue_type)
{
    vector<Personnel> temporary_testing_group;
    queue<Personnel> temp_queue;
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

        tested_single_amount_++;
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
            tested_mixed_amount_++;
            mixed_testing_queue_.pop();
        }
        TestNuclearAcid(queue_type, temporary_testing_group);
    }

}

// 大白核酸检测
void COVID_19_TestingSystem::TestNuclearAcid(QUEUE_TYPE queue_type, vector<Personnel>& testing_personnels)
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

// --------------------------功能3------------------------------------------------------
// 显示队列情况
void COVID_19_TestingSystem::DisplayQueueing()
{
    int column = 1;
    queue<Personnel>temp_queue = mixed_testing_queue_;

    if (mixed_testing_queue_.empty())
        std::cout << "<Error> 当前混检队列为空。\n";
    if (single_testing_queue_.empty())
        std::cout << "<Error> 当前单检队列为空。\n";
    if (mixed_testing_queue_.empty() && single_testing_queue_.empty())
        return;
    std::cout << "-----------------------------------------------------\n";

    if (!mixed_testing_queue_.empty())
    {
        std::cout << "混检队列：\n";
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
        std::cout << "单检队列：\n";
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


// --------------------------功能4------------------------------------------------------
// 获取已检测试管的数量
int COVID_19_TestingSystem::GetTestedTubeAmount()
{
    return	tested_tube_box_.size();
}

// 随机阳性管
void COVID_19_TestingSystem::RandomizeTubeInfo()
{
    double positive_num = tested_personnels_.size() * kPOSITIVE_PROBABILITY;
    if (positive_num < 1)
        positive_num = 1;
    std::random_device rd;
    std::mt19937 random(rd());
    std::uniform_int_distribution<int> distrib(0, tested_tube_box_.size() - 1); // 指定范围
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

// 填入检测结果
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

// 显示检测管信息
void COVID_19_TestingSystem::DisplayTube()
{
    std::cout << "=========================== 当前试管信息 ==========================" << std::endl;
    int cnt = 1;
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

// 决定人员状态
void COVID_19_TestingSystem::DecidePersonnelStatus()
{
    // 确定确诊楼号，标记确诊、可疑人员
    vector<std::string> confirmed_blocks;
    for (auto& tube : tested_tube_box_)
    {
        if (tube.GetTestResult() == TEST_RESULT::POSITIVE && tube.GetPersonnelInThisTube().size() == 1)		// 单人管阳性
        {
            Personnel personnel = tube.GetPersonnelInThisTube()[0];
            for (auto& target_personnel : tested_personnels_)
            {
                if (target_personnel.GetCode() == personnel.GetCode())
                {
                    target_personnel.SetStatus(STATUS::COMFIRMED);				    // 单人管确诊
                }
                if (target_personnel.GetStatus() == STATUS::COMFIRMED)
                {
                    confirmed_personnels_.push_back(target_personnel);
                    confirmed_blocks.push_back(target_personnel.GetCode());		// 确诊楼号
                }
            }
        }
        else if ((tube.GetTestResult() == TEST_RESULT::POSITIVE || tube.GetTestResult() == TEST_RESULT::SUSPECTED)
                 && tube.GetPersonnelInThisTube().size() > 1)			// 混检阳性或可疑
        {
            for (auto& personnel : tube.GetPersonnelInThisTube())
                for (auto& target_personnel : tested_personnels_)
                    if (target_personnel.GetCode() == personnel.GetCode())
                        target_personnel.SetStatus(STATUS::SUSPECTED);					// 混检人员为可疑
        }
    }
    vector<std::string> close_contact_blocks = DecideCloseContact(confirmed_blocks);
    DecideSubCloseContact(close_contact_blocks);
    DecideNegative();
}

// 确定密接
vector<std::string> COVID_19_TestingSystem::DecideCloseContact(vector<std::string> confirmed_blocks)
{
    vector<std::string> close_contact_blocks;
    // 确定楼宇密接
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
    // 确定排队密接
    for (auto& personnel : confirmed_personnels_)   // 人数不多，时间复杂度高不会太影响
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

// 确定次密接
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

// 确定阴性
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


// --------------------------功能5------------------------------------------------------
// 更新所有类型的人员信息
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

// 根据类别显示类型信息
void COVID_19_TestingSystem::DisplayType(STATUS status)
{
    std::string name;
    int cnt = 1;
    vector<Personnel> temp_vector;
    if (status == STATUS::WAITING_RESULT)
    {
        temp_vector = need_to_be_uploaded_personnels_;
        name = "待上传";
    }
    else if (status == STATUS::NEGATIVE)
    {
        temp_vector = negative_personnels_;
        name = "阴性";
    }
    else if (status == STATUS::COMFIRMED)
    {
        temp_vector = confirmed_personnels_;
        name = "确诊";
    }
    else if (status == STATUS::CLOSE_CONTACT)
    {
        temp_vector = close_contact_personnels_;
        name = "密接";
    }
    else if (status == STATUS::SUB_CLOSE_CONTACT)
    {
        temp_vector = sub_close_contact_personnels_;
        name = "次密接";
    }
    else if (status == STATUS::SUSPECTED)
    {
        temp_vector = suspected_personnels_;
        name = "可疑";
    }
    else if (status == STATUS::QUEUING)
    {
        temp_vector = queueing_personnels_;
        name = "队列中";
    }
    else
        return;
    if (temp_vector.size() == 0)
        std::cout << "<Info>  无" + name + "人员。\n";
    else
    {
        std::cout << name << ": \n";
        std::cout << "序号\t" << "人员代码\n";

        for (auto& it : temp_vector)
        {
            std::cout << cnt << '\t' << it.GetCode() << std::endl;
            cnt++;
        }
    }
    std::cout << "-------------------------------------------------------------------" << std::endl;
}


// --------------------------功能6------------------------------------------------------
// 获取指定人员状态
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

// 显示指定人员状态
void COVID_19_TestingSystem::DisplayStatus(const std::string& personnel_code)
{
    STATUS status = GetPersonnelStatus(personnel_code);
    std::cout << "-------------------------------------------------\n";
    if (status == STATUS::WAITING_RESULT)
    {
        std::cout << personnel_code << ": " << "待上传";
    }
    else if (status == STATUS::NEGATIVE)
    {
        std::cout << personnel_code << ": " << "阴性";
    }
    else if (status == STATUS::COMFIRMED)
    {
        std::cout << personnel_code << ": " << "确诊";
    }
    else if (status == STATUS::CLOSE_CONTACT)
    {
        std::cout << personnel_code << ": " << "密接";
    }
    else if (status == STATUS::SUB_CLOSE_CONTACT)
    {
        std::cout << personnel_code << ": " << "次密接";
    }
    else if (status == STATUS::SUSPECTED)
    {
        std::cout << personnel_code << ": " << "可疑";
    }
    else if (status == STATUS::NOT_FOUND)
    {
        std::cout << "<Error> " << personnel_code << ": " << "未找到该人员";
    }
    else if (status == STATUS::NOT_TESTED)
    {
        std::cout << personnel_code << ": " << "未检测";
    }
    else if (status == STATUS::QUEUING)
    {
        std::cout << personnel_code << ": " << "队列中";
    }
    std::cout << std::endl;
}



// --------------------------保存------------------------------------------------------
// 将当前人员信息保存
void COVID_19_TestingSystem::SaveToFile(const std::string& file_name)
{
    //文件输入（写到文件里）
    std::ofstream fout;
    fout.open(file_name);
    fout << tested_mixed_amount_ << " " << tested_single_amount_ << '\n';
    fout << "=========================== 当前人员信息 ==========================" << std::endl;

    fout << "人员代码	" << "状态\n";
    for (auto & it :tested_personnels_)
    {
        fout << it.GetCode() << "	";
        STATUS status = it.GetStatus();
        if (STATUS::CLOSE_CONTACT == status)
            fout << "密接" << std::endl;
        if (STATUS::COMFIRMED == status)
            fout << "确诊" << std::endl;
        if (STATUS::NEGATIVE == status)
            fout << "阴性" << std::endl;
        if (STATUS::NOT_TESTED == status)
            fout << "未检测" << std::endl;
        if (STATUS::QUEUING == status)
            fout << "队列中" << std::endl;
        if (STATUS::SUB_CLOSE_CONTACT == status)
            fout << "次密接" << std::endl;
        if (STATUS::SUSPECTED == status)
            fout << "可疑" << std::endl;
        if (STATUS::WAITING_RESULT == status)
            fout << "等待结果" << std::endl;
    }
    fout.close();
}


// --------------------------辅助功能---------------------------------------------------
// 显示全部人员信息
void COVID_19_TestingSystem::DisplayPersonnel()
{
    std::cout << "=========================== 当前人员信息 ==========================" << std::endl;
    if (tested_personnels_.size() == 0)
        std::cout << "当前没有人员前来检测\n";
    int cnt = 1;
    for (auto&personnel : tested_personnels_)
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

// 返回队列是否为空
bool COVID_19_TestingSystem::IsEmpty(QUEUE_TYPE queue_type)
{
    if (queue_type == QUEUE_TYPE::SINGLE)
        return single_testing_queue_.empty();
    else if (queue_type == QUEUE_TYPE::MIXED)
        return mixed_testing_queue_.empty();
}

// 显示人员排队时的密接者
void COVID_19_TestingSystem::DisplayPersonnelCloser()
{
    std::cout << "=====================================================================" << std::endl;
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











// =================================================================================================
// =================================================================================================
// Public

// 显示菜单
void COVID_19_TestingSystem::DisplayMenu()
{
    std::cout << "======================核酸检测系统=======================\n";
    std::cout << "==================   *0. 初始化排队     =================\n";
    std::cout << "==================    1. 排队           =================\n";
    std::cout << "==================    2. 检测           =================\n";
    std::cout << "==================    3. 查看排队情况   =================\n";
    std::cout << "==================    4. 登记测试管信息 =================\n";
    std::cout << "==================    5. 各类人员查询   =================\n";
    std::cout << "==================    6. 个人查询       =================\n";
    std::cout << "==================    s. 文件保存       =================\n";
    std::cout << "==================    q. 退出系统       =================\n";
    std::cout << "=========================================================\n";
    std::cout << "===当前混检人数：" << GetQueueAmount(QUEUE_TYPE::MIXED) << "\t";
    std::cout << "当前单检人数：" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===当前已检人数：" << GetTestedPersonnelAmount()
              << " <混检人数: " << GetTestedMixedAmount() << " 单检人数: " << GetTestedSingleAmount()
              << ">\n";
    std::cout << "===请选择功能： ";
}

// 功能0：初始化队列
void COVID_19_TestingSystem::InitQueue()
{
    std::string file_name = line_up_file_name_;
    LineUpFromFile(file_name);
    std::cout << "===================================================================" << std::endl;
    std::cout << "----初始化文件输入完成----" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "----<Info>  Uploaded from ( " + file_name + " )" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "===当前混检人数：" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "===当前单检人数：" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===================================================================" << std::endl;
}

// 功能1：自主加入排队人员
void COVID_19_TestingSystem::AddQueuePersonnel()
{
    Personnel personnel;
    int mixed_amount, single_amount;

    std::cout << "请输入混检人员和单检人员人数（m n)：";
    std::cin >> mixed_amount >> single_amount;
    std::cout << "输入混检/人员代码：\n";
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
    std::cout << "----输入完成----" << std::endl;
    std::cout << "===当前混检人数：" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "===当前单检人数：" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    std::cout << "===================================================================" << std::endl;
}

// 功能2：检测
void COVID_19_TestingSystem::Testing()
{
    std::cout << "===================================================================" << std::endl;
    if (GetQueueAmount(QUEUE_TYPE::MIXED) == 0)
        std::cout << "<Error> 当前混检无人排队，无法检测。\n";
    if (GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
        std::cout << "<Error> 当前单检无人排队，无法检测。\n";
    if (GetQueueAmount(QUEUE_TYPE::MIXED) == 0
        && GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
        return;
    // 混检
    while (!IsEmpty(QUEUE_TYPE::MIXED))
        DelQueue(QUEUE_TYPE::MIXED);
    std::cout << "<Info>  混检完成。\n";
    // 单检
    while (!IsEmpty(QUEUE_TYPE::SINGLE))
        DelQueue(QUEUE_TYPE::SINGLE);
    std::cout << "<Info>  单检完成。\n";
    DisplayPersonnel();
    std::cout << "===================================================================" << std::endl;
}

// 功能3：查看排队情况
void COVID_19_TestingSystem::ShowQueueStatus()
{
    std::cout << "===================================================================" << std::endl;
    std::cout << "=当前混检队列人数：" << GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
    std::cout << "=当前单检队列人数：" << GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
    DisplayQueueing();
    std::cout << "===================================================================" << std::endl;
}

// 功能4：登记人员信息
void COVID_19_TestingSystem::RegisterPersonnelInfo()
{
    std::cout << "===================================================================" << std::endl;
    if (GetTestedTubeAmount() == 0)
        std::cout << "<Info>  已检测管数量为空，请先检测试管。\n";
    else
    {
        RandomizeTubeInfo();
        DisplayTube();
        DecidePersonnelStatus();
        DisplayPersonnel();
    }
    std::cout << "===================================================================" << std::endl;
}

// 功能5：显示所有类型人员信息
void COVID_19_TestingSystem::DisplayAllTypePersonnel()
{
    UpdateAllTypeInfo();
    std::cout << "=========================== 人员信息 ==============================" << std::endl;
    for (int i = 0; i < 9; i++)
        DisplayType(static_cast<STATUS>(i));
    std::cout << "===================================================================" << std::endl;
}

// 功能6：显示指定人员信息
void COVID_19_TestingSystem::DisplaySpecificPersonnel()
{
    std::string personnel_code;
    std::cout << "===================================================================" << std::endl;
    std::cout << "请输入人员代码（xxxyyyyz -- 楼号/房间号/序号）:"; std::cin >> personnel_code;
    DisplayStatus(personnel_code);
    std::cout << "===================================================================" << std::endl;
}

// 保存
void COVID_19_TestingSystem::Save()
{
    std::string file_name = nucleic_acid_test_file_name_;
    std::cout << "===================================================================" << std::endl;
    DisplayPersonnel();
    SaveToFile(file_name);
    std::cout << "===================================================================" << std::endl;
}