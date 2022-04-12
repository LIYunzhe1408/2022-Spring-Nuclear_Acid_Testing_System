#include "Personnel.h"
// ==========================================
// Public
// ���캯��
Personnel::Personnel(std::string personnel_code, STATUS status):personnel_code_(personnel_code), status_(status) {}


// ����Ŷ�ʱ���ܽ���Ա
void Personnel::AddCloseContactPersonnel(const Personnel& personnel)
{
	close_contact_in_queue_.push_back(personnel);
}

// ��ȡ�ٽ���Ա
std::vector<Personnel> Personnel::GetCloseContactInQueue() const
{
	return close_contact_in_queue_;
}

// ��ȡ��ǰ��Ա״̬
STATUS Personnel::GetStatus()
{
	return status_; 
}

// ��ȡ��ǰ��Ա����
std::string Personnel::GetCode()
{
	return personnel_code_;
}				

// ���ü����
void Personnel::SetStatus(STATUS status)
{
	status_ = status; 
}

// ���ظ�ֵ�����
Personnel& Personnel::operator=(const Personnel& personnel)
{
	if (this != &personnel)
	{
		this->personnel_code_ = personnel.personnel_code_;
		this->status_ = personnel.status_;
	}
	return *this;
}