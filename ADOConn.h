#pragma once

#include <stdlib.h> 
#include<cstdlib>
#include<ctime>
#define random(a,b) (rand()%(b-a+1)+a)
#define MAX_FILE_SIZE 1024  
#import "C:\Program Files\Common Files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") rename("Error", "adoError")

class ADOConn
{
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	ADOConn();
	virtual ~ADOConn();
	void OnInitADOConn();//初始化连接
	void ExecuteSQL(_bstr_t bstrSQL);
};



