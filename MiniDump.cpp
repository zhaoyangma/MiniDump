#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <DbgHelp.h>

// ����Dump�ļ� 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName,EXCEPTION_POINTERS* pException)
{
	// ����Dump�ļ�
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	// Dump��Ϣ 
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// д��Dump�ļ�����
	MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hDumpFile,MiniDumpNormal,&dumpInfo,NULL,NULL);
	// �ر��ļ����
	CloseHandle(hDumpFile);
}

// ����Unhandled Exception�Ļص�����
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	// ���ﵯ��һ������Ի����˳����� 
	//FatalAppExit(-1,_T("*** Unhandled Exception!***"));
	// ����Dump�ļ�
	CreateDumpFile(_T("C:\\Test.dmp"),pException);

	return EXCEPTION_EXECUTE_HANDLER;
}
class CrashTest
{
public:
	void Test()
	{
		Crash();
	}
private:
	int Crash()
	{
		int* i=NULL;
		*i = 1;
		return 1;
	}
};

int main(int argc,_TCHAR* argv[])
{
	// ���ô���Unhandled Exception�Ļص�����
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	CrashTest test;
	test.Test();
	return 0;
}