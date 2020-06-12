#include <iostream>
#include <Windows.h>
#include <string>
#include <Shlwapi.h>
#include <Psapi.h>
#include <TlHelp32.h> //ʹ��PROCESSENTRY32�ṹ��
#include <iomanip>
#pragma comment(lib, "Shlwapi.lib")
using namespace std;
void ShowPerformanceInfo() //��ȡ������Ϣ
{
	PERFORMANCE_INFORMATION pi;
	pi.cb = sizeof(pi);
	GetPerformanceInfo(&pi, sizeof(pi));
	cout << "������Ϣ��" << endl;
	cout << left << setw(30) << "ϵͳ��ǰ�ύ��ҳ������Ϊ��" << pi.CommitTotal << endl;
	cout << left << setw(30) << "ϵͳ��ǰ���ύ�����ҳ����Ϊ��" << pi.CommitLimit << endl;
	cout << left << setw(30) << "ϵͳ��ʷ�ύҳ���ֵΪ��" << pi.CommitPeak << endl;
	cout << left << setw(30) << "��ҳ������������ڴ�Ϊ��" << pi.PhysicalTotal << endl;
	cout << left << setw(30) << "��ǰ���õ������ڴ�Ϊ��" << pi.PhysicalAvailable << endl;
	cout << left << setw(30) << "ϵͳ���������Ϊ��" << pi.SystemCache << endl;
	cout << left << setw(30) << "�ڴ�����Ϊ��" << pi.KernelTotal << endl;
	cout << left << setw(30) << "��ҳ�صĴ�СΪ��" << pi.KernelPaged << endl;
	cout << left << setw(30) << "�Ƿ�ҳ�صĴ�СΪ��" << pi.KernelNonpaged << endl;
	cout << left << setw(30) << "ҳ�Ĵ�СΪ��" << pi.PageSize / 1024 << "KB" << endl;
	cout << left << setw(30) << "�򿪵ľ������Ϊ��" << pi.HandleCount << endl;
	cout << left << setw(30) << "���̸���Ϊ��" << pi.ProcessCount << endl;
	cout << left << setw(30) << "�̸߳���Ϊ��" << pi.ThreadCount << endl;
	cout << endl
		<< endl;
	return;
}
void ShowMemoryInfo() //��ȡ�ڴ���Ϣ
{
	MEMORYSTATUSEX ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatusEx(&ms);
	cout << "�ڴ���Ϣ��" << endl;
	cout << left << setw(25) << "�ڴ��ʹ����Ϊ��" << ms.dwMemoryLoad << "%" << endl;
	cout << left << setw(25) << "�ڴ��������Ϊ��" << fixed << setprecision(2) << (float)ms.ullTotalPhys / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "���õ��ڴ�Ϊ��" << fixed << setprecision(2) << (float)ms.ullAvailPhys / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "ҳ�ļ���������Ϊ��" << fixed << setprecision(2) << (float)ms.ullTotalPageFile / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "���õ�ҳ�ļ�Ϊ��" << fixed << setprecision(2) << (float)ms.ullAvailPageFile / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "�����ڴ��������Ϊ��" << fixed << setprecision(2) << (float)ms.ullTotalVirtual / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "���õ������ڴ�Ϊ��" << fixed << setprecision(2) << (float)ms.ullAvailVirtual / 1024 / 1024 / 1024 << "GB" << endl;
	cout << left << setw(25) << "���õ���չ�����ڴ�Ϊ��" << fixed << setprecision(2) << (float)ms.ullAvailExtendedVirtual / 1024 << "KB" << endl;
	cout << endl
		<< endl;
	return;
}
void ShowSystemInfo() //���ϵͳ��Ϣ
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);
	cout << "ϵͳ��Ϣ��" << endl;
	cout << left << setw(41) << "�ڴ�ҳ�Ĵ�СΪ��" << (int)si.dwPageSize / 1024 << "KB" << endl;
	cout << left << setw(41) << "ÿ�����̿��õ�ַ�ռ����С�ڴ��ַΪ��   0x" << setw(8) << si.lpMinimumApplicationAddress << endl;
	cout << left << setw(41) << "ÿ�����̿��õ�ַ�ռ������ڴ��ַΪ��   0x" << setw(8) << si.lpMaximumApplicationAddress << endl;
	cout << left << setw(41) << "ϵͳ�䱸��CPU������Ϊ��" << si.dwNumberOfProcessors << endl;
	cout << left << setw(41) << "�ܹ�������ַ�ռ��������С��λΪ��" << si.dwAllocationGranularity / 1024 << "KB" << endl;
	cout << endl
		<< endl;
	return;
}
void ShowProcessInfo() //��ý�����Ϣ
{
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	//��ȡ��ǰ���н��̵Ŀ���
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//��ȡ��һ�����̵ľ��
	bool b = Process32First(hProcess, &pe);
	if (b == false)
		cout << "false" << endl;
	string pid = "PID";
	string pname = "��������";
	string psize = "���̹�������С";
	cout << "������Ϣ��" << endl;
	printf("\n%5s%70s%25s\n\n", pid.c_str(), pname.c_str(), psize.c_str()); //����ID���������ƣ����̹�������С
	while (b)                                  //�������н���
	{
		HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		PROCESS_MEMORY_COUNTERS pmc;
		ZeroMemory(&pmc, sizeof(pmc));
		if (GetProcessMemoryInfo(h, &pmc, sizeof(pmc)))
		{ //��ȡ���̵�ʹ�����
			printf("%5d%70ls%25.2fKB\n", pe.th32ProcessID, pe.szExeFile, ((float)pmc.WorkingSetSize) / 1024);
		}
		b = Process32Next(hProcess, &pe);
	}
	CloseHandle(hProcess);
	cout << endl
		<< endl;
	return;
}
//��ʾ������ǣ��ñ�Ǳ�ʾ����Ӧ�ó�����ڴ���з��ʵ����ͣ��ο��α�292ҳ
inline bool TestSet(DWORD dwTarget, DWORD dwMask)
{
	return ((dwTarget & dwMask) == dwMask);
}
#define SHOWMASK(dwTarget, type)        \
    if (TestSet(dwTarget, PAGE_##type)) \
    {                                   \
        cout << "," << #type;           \
    }
void ShowProtection(DWORD dwTarget)//��ʾ��ǰ��ҳ�汣����ʽ
{ //�����ҳ�汣����ʽ
	SHOWMASK(dwTarget, READONLY);//ֻ��
	SHOWMASK(dwTarget, GUARD);//����
	SHOWMASK(dwTarget, NOCACHE);//�޻���
	SHOWMASK(dwTarget, READWRITE);//��д
	SHOWMASK(dwTarget, WRITECOPY);//дʱ����
	SHOWMASK(dwTarget, EXECUTE);//ִ��
	SHOWMASK(dwTarget, EXECUTE_READ);//ִ�ж�
	SHOWMASK(dwTarget, EXECUTE_READWRITE);//ִ�ж�д
	SHOWMASK(dwTarget, EXECUTE_WRITECOPY);//ִ��дʱ����
	SHOWMASK(dwTarget, NOACCESS);//δ����
}
void ShowVirtualMemoryInfoAndWorkingSetInfo() //��ȡĳһ���̵������ַ�ռ䲼�ֺ͹�������Ϣ
{
	int pid;
	cout << "���������PID��" << endl;
	cin >> pid;
	if (pid == -1)
	{
		cout << endl << endl;
		return;
	}
	HANDLE hProcess;
	while (1) //�ж������PID�Ƿ���ȷ
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == NULL)
			cout << "PID�������������룺" << endl;
		else
			break;
		cin >> pid;
		if (pid == -1)
		{
			cout << endl << endl;
			return;
		}
	}
	cout << endl << "�鿴" << pid << "�Ž��̵�����ռ䲼�ֺ͹�������Ϣ��" << endl << endl;
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);
	MEMORY_BASIC_INFORMATION mbi;  //���������ڴ�ռ�Ļ�����Ϣ�ṹ
	ZeroMemory(&mbi, sizeof(mbi)); //���仺���������ڱ�����Ϣ

	LPCVOID pBlock = (LPCVOID)si.lpMinimumApplicationAddress;
	for (; pBlock < si.lpMaximumApplicationAddress;) //������������ĵ�ַ�ռ�
	{
		if (VirtualQueryEx(hProcess,                    //��صĽ���
			pBlock,                      //��ʼλ��
			&mbi,                        //������
			sizeof(mbi)) == sizeof(mbi)) //���ȵ�ȷ��
		{                                               //��ѯ��ַ�ռ����ڴ��ַ����Ϣ
			//�����Ľ�β���䳤��
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize; //ҳ����ַ+�������С
			TCHAR szSize[MAX_PATH];
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH); //��ʽ���ļ��Ĵ�С
			//��ʾ���ַ�ͳ���
			printf("���ַ�� %8X-%8X(%ls)     ", (DWORD)pBlock, (DWORD)pEnd, szSize);
			int l = lstrlen(szSize);
			int x = 10 - l;
			for (int i = 0; i < x; i++)//�����ʽ
				printf(" ");
			//��ʾ���״̬
			if (mbi.State == MEM_COMMIT)
				cout << "���ύ";
			else if (mbi.State == MEM_FREE)
				cout << "���е�";
			else if (mbi.State == MEM_RESERVE)
				cout << "������";
			//��ʾ����
			if (mbi.Protect == 0 && mbi.State != MEM_FREE)
				mbi.Protect = PAGE_READONLY;
			ShowProtection(mbi.Protect);
			//��ʾ����
			if (mbi.Type == MEM_IMAGE)
				cout << ",Image";
			else if (mbi.Type == MEM_PRIVATE)
				cout << ",Private";
			else if (mbi.Type == MEM_MAPPED)
				cout << ",Mapped";
			//�����ִ�е�ӳ��
			TCHAR szFilename[MAX_PATH];            //��ȡ���ڵ�ǰ��ִ���ļ����ļ���
			if (GetModuleFileName((HMODULE)pBlock, //ʵ�������ڴ��ģ����
				szFilename,      //��ȫָ�����ļ�����
				MAX_PATH) > 0)   //ʵ��ʹ�õĻ���������
			{
				PathStripPath(szFilename); //��ȥ·��
				cout << ",Module:" << szFilename;
			}
			cout << endl;
			pBlock = pEnd; //�ƶ�ָ���Ի����һ����
		}
	}
	PROCESS_MEMORY_COUNTERS p;
	ZeroMemory(&p, sizeof(p));
	GetProcessMemoryInfo(hProcess, &p, sizeof(p));
	cout << endl << "          ��������Ϣ��" << (float)p.WorkingSetSize / 1024 << "KB" << endl;
	cout << endl << endl;
}
int main()
{
	while (1)
	{
		cout << "�˵�" << endl
			<< "����������ʹ�ö�Ӧ�Ĺ��ܣ�" << endl
			<< "1���鿴������Ϣ" << endl
			<< "2���鿴�ڴ�״̬" << endl
			<< "3���鿴ϵͳ��Ϣ" << endl
			<< "4���鿴������Ϣ" << endl
			<< "5���鿴ĳһ���̵������ַ�ռ䲼�ֺ͹�������Ϣ" << endl
			<< "-1������" << endl
			<< endl;
		int in;
		cin >> in;
		if (in == 1)
			ShowPerformanceInfo();
		else if (in == 2)
			ShowMemoryInfo();
		else if (in == 3)
			ShowSystemInfo();
		else if (in == 4)
			ShowProcessInfo();
		else if (in == 5)
			ShowVirtualMemoryInfoAndWorkingSetInfo();
		else if (in == -1)
			return 0;
		else
			cout << "���벻�Ϸ��������ԣ�" << endl;
	}
	return 0;
}