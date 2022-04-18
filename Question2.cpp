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
    /// 构造核酸检测系统（构造时从文件初始化已检测信息）
    /// 初始化队列人员信息
    /// 排队做核酸
    /// 试管检测
    /// 根据检测结果确定检测人员状态（密接、次密接、确诊、阴性等）
    /// 输出结果
    std::string line_up_file_name = "line_up.txt", nucleic_acid_test_file_name = "nucleic_acid_test.txt";

    COVID_19_TestingSystem Covid19_TestingSystem(line_up_file_name, nucleic_acid_test_file_name);
    while (true)
    {
        Covid19_TestingSystem.DisplayMenu();

        char mode_chooser;
        std::cin >> mode_chooser;
        system("cls");

        switch (mode_chooser) {
            case '0':  // 加入初始化文件中的排队人员
            {
                Covid19_TestingSystem.InitQueue();
                break;
            }
            case '1':  // =======自主加入排队人员（默认不重复）=======
            {
                Covid19_TestingSystem.AddQueuePersonnel();break;
            }
            case '2':  // =============检测=============
            {
                Covid19_TestingSystem.Testing();break;
            }
            case '3': // ===========查看排队情况=============
            {
                Covid19_TestingSystem.ShowQueueStatus();break;
            }
            case '4': // ==========登记测试管信息============
            {
                Covid19_TestingSystem.RegisterPersonnelInfo();break;
            }
            case '5': // ========显示各类人群信息=============
            {
                Covid19_TestingSystem.DisplayAllTypePersonnel();break;
            }
            case '6':  // ==========显示指定人信息==============
            {
                Covid19_TestingSystem.DisplaySpecificPersonnel();break;
            }
            case 's':   // ===============保存=================
            {
                Covid19_TestingSystem.Save();break;
            }
            default:
                break;
        }
        system("pause");
        system("cls");
        if (mode_chooser == 'q')
            break;
    }
    return 0;
}
