#include "stdafx.h"
#include "ADOConn.h"


ADOConn::ADOConn()
{
	CoInitialize(NULL); //��ʼ��
	HRESULT hr = m_pConnection.CreateInstance(_uuidof(Connection));
}


ADOConn::~ADOConn()
{
}

void ADOConn::OnInitADOConn()
{
	try
	{	// "Data Source =.; Initial Catalog = OTCDataBase2.0; User ID = sa; pwd=123456"
		_bstr_t con_bstr = "Driver={sql server};server=.;uid=sa;pwd=123456;database=OTCDataBase2.0;";
		m_pConnection->Open(con_bstr, "", "", adModeUnknown);// adModeUnknown ȱʡ����ǰ�����Ȩδ����
	}
	catch (_com_error &e)
	{

	}
}

void ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	m_pRecordset = m_pConnection->Execute(bstrSQL, NULL, adCmdText);
}
