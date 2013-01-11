#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <DbgHelp.h>

// 创建Dump文件 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName,EXCEPTION_POINTERS* pException)
{
	// 创建Dump文件
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	// Dump信息 
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// 写入Dump文件内容
	MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hDumpFile,MiniDumpNormal,&dumpInfo,NULL,NULL);
	// 关闭文件句柄
	CloseHandle(hDumpFile);
}

// 处理Unhandled Exception的回调函数
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	// 这里弹出一个错误对话框并退出程序 
	//FatalAppExit(-1,_T("*** Unhandled Exception!***"));
	// 创建Dump文件
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
	// 设置处理Unhandled Exception的回调函数
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	CrashTest test;
	test.Test();
	return 0;
}