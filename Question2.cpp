// *
// @Author: 李昀哲 20123101
// Created at Apr.12.2022
// Attention.
// *
#include "Personnel/Personnel.h"
#include "TestTube/TestTube.h"
#include "COVID_19_TestSystem/COVID_19_TestingSystem.h"
int main()
{
    /// 流程
    /// Input人员的信息，personnel_code
    /// 去排队做核酸
    /// 等结果（编辑测试管结果）
    /// 根据结果确定检测人员状态（密接、次密接、确诊、阴性等）
    /// 输出结果
    COVID_19_TestingSystem Covid19_TestingSystem;

    while (true)
    {
        std::cout << "======================核酸检测系统=======================\n";
        std::cout << "==================    1. 排队           =================\n";
        std::cout << "==================    2. 检测           =================\n";
        std::cout << "==================    3. 查看排队情况   =================\n";
        std::cout << "==================    4. 登记测试管信息 =================\n";
        std::cout << "==================    5. 各类人员查询   =================\n";
        std::cout << "==================    6. 个人查询       =================\n";
        std::cout << "==================    q. 退出系统       =================\n";
        std::cout << "=========================================================\n";
        std::cout << "===当前混检人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::MIXED) << "\t";
        std::cout << "当前单检人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
        std::cout << "===当前已检人数：" << Covid19_TestingSystem.GetTestedPersonnelAmount() << std::endl;
        std::cout << "===请选择功能： " ;
        char mode_chooser;
        std::cin >> mode_chooser;
        system("cls");

        // ==========================================================================
        // choose mode
        switch (mode_chooser) {
            case '0':
            {

                break;
            }
                // ==============================
                // 加入排队人员
            case '1':
            {
                Personnel personnel;
                int mixed_amount, single_amount;

                std::cout << "请输入混检人员和单检人员人数（m n)：";
                std::cin >> mixed_amount >> single_amount;
                std::cout << "输入混检人员代码：\n";
                for (int i = 0; i < mixed_amount; i++)
                {
                    std::cin >> personnel;
                    Covid19_TestingSystem.EnQueue(QUEUE_TYPE::MIXED, personnel);
                }
                for (int i = 0; i < single_amount; i++)
                {
                    std::cin >> personnel;
                    Covid19_TestingSystem.EnQueue(QUEUE_TYPE::SINGLE, personnel);
                }
                system("cls");
                std::cout << "===================================================================" << std::endl;
                std::cout << "----输入完成----" << std::endl;
                std::cout << "===当前混检人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::MIXED) << std::endl;
                std::cout << "===当前单检人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
                std::cout << "===================================================================" << std::endl;

                system("pause");
                break;
            }
                // ==============================
                // 检测
            case '2':
            {
                std::cout << "===================================================================" << std::endl;
                if (Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::MIXED) == 0)
                    std::cout << "<Error> 当前混检无人排队，无法检测。\n";
                if (Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
                    std::cout << "<Error> 当前单检无人排队，无法检测。\n";
                if (Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::MIXED) == 0
                    && Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::SINGLE) == 0)
                    break;
                // 混检
                while (!Covid19_TestingSystem.IsEmpty(QUEUE_TYPE::MIXED))
                    Covid19_TestingSystem.DelQueue(QUEUE_TYPE::MIXED);
                std::cout << "<Info>  混检完成。\n";
                // 单检
                while (!Covid19_TestingSystem.IsEmpty(QUEUE_TYPE::SINGLE))
                    Covid19_TestingSystem.DelQueue(QUEUE_TYPE::SINGLE);
                std::cout << "<Info>  单检完成。\n";
                Covid19_TestingSystem.DisplayPersonnel();
                std::cout << "===================================================================" << std::endl;
                system("pause");
                break;
            }
                // ==============================
                // 查看排队情况
            case '3':
            {
                std::cout << "===================================================================" << std::endl;
                std::cout << "=当前混检队列人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::MIXED)  << std::endl;
                std::cout << "=当前单检队列人数：" << Covid19_TestingSystem.GetQueueAmount(QUEUE_TYPE::SINGLE) << std::endl;
                Covid19_TestingSystem.DisplayQueueing();
                std::cout << "===================================================================" << std::endl;
                system("pause");
                break;
            }
                // ==============================
                // 登记测试管信息
            case '4':
            {
                std::cout << "===================================================================" << std::endl;
                if (Covid19_TestingSystem.GetTestedTubeAmount() == 0)
                    std::cout << "<Info>  已检测管数量为空，请先检测试管。\n";
                else
                {
                    Covid19_TestingSystem.RandomizeTubeInfo();
                    Covid19_TestingSystem.DisplayTube();
                    Covid19_TestingSystem.DecidePersonnelStatus();
                    Covid19_TestingSystem.DisplayPersonnel();
                }
                std::cout << "===================================================================" << std::endl;
                system("pause");
                break;
            }
                // ==============================
                // 显示各类人群信息
            case '5':
            {
                Covid19_TestingSystem.DisplayAllTypePersonnel();
                system("pause");
                break;
            }
                // ==============================
                // 显示指定人信息
            case '6':
            {
                std::string code;
                std::cout << "===================================================================" << std::endl;
                std::cout << "请输入人员代码（xxxyyyz -- 楼号/房间号/序号）:"; std::cin >> code;
                Covid19_TestingSystem.DisplayStatus(code);
                std::cout << "===================================================================" << std::endl;
                system("pause");
                break;
            }
            default:
                break;
        }
        system("cls");
        if (mode_chooser == 'q')
            break;
    }


    /*
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
//	TestingSystem.SetTubeResult(positive_num, TEST_RESULT::POSITIVE);
//  TestingSystem.SetTubeResult(negative_num, TEST_RESULT::NEGTIVE);
    TestingSystem.DecidePersonnelStatus();


    // ==============================================
    // 输出
    TestingSystem.DisplayPersonnelCloser();
    TestingSystem.DisplayTube();
    TestingSystem.DisplayPersonnel();
    */
    return 0;
}
