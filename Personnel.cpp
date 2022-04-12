#include "Personnel.h"
// ==========================================
// Public
// 构造函数
Personnel::Personnel(std::string personnel_code, STATUS status):personnel_code_(personnel_code), status_(status) {}


// 添加排队时的密接人员
void Personnel::AddCloseContactPersonnel(const Personnel& personnel)
{
	close_contact_in_queue_.push_back(personnel);
}

// 获取临近人员
std::vector<Personnel> Personnel::GetCloseContactInQueue() const
{
	return close_contact_in_queue_;
}

// 获取当前人员状态
STATUS Personnel::GetStatus()
{
	return status_; 
}

// 获取当前人员代码
std::string Personnel::GetCode()
{
	return personnel_code_;
}				

// 设置检测结果
void Personnel::SetStatus(STATUS status)
{
	status_ = status; 
}

// 重载赋值运算符
Personnel& Personnel::operator=(const Personnel& personnel)
{
	if (this != &personnel)
	{
		this->personnel_code_ = personnel.personnel_code_;
		this->status_ = personnel.status_;
	}
	return *this;
}