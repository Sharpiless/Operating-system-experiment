#include<stdio.h>
#include<windows.h>

int main(int argc,char *argv[])
{   
	SYSTEMTIME start,end;
	TCHAR szFilename[MAX_PATH];
	TCHAR szCmdLine[MAX_PATH]; 
	sprintf(szFilename,"%s",argv[1]);
	GetModuleFileName(NULL,szFilename,MAX_PATH);//��õ�ǰ��ִ���ļ����ļ� 
	STARTUPINFO si;
	ZeroMemory(reinterpret_cast<void*>(&si),sizeof(si));
	si.cb=sizeof(si);
	//ʹ��˫���ţ�ת����������ִ���ļ���ȫ·���Ա�ִ֤����ȷ���ļ� 
	//��t������program2һ�𴫽�ȥ 
	sprintf(szCmdLine,"\"%s\" %s",argv[1],argv[2]);
	
	PROCESS_INFORMATION pi;//printf("%s\n",szCmdLine);
	BOOL bCreateOK=CreateProcess(NULL,szCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	if(!bCreateOK)
	{
		printf("create failed\n");
		exit(1);
	}
	GetSystemTime(&start); 
	printf("create succeed!\n");
	WaitForSingleObject(pi.hProcess,INFINITE);
	GetSystemTime(&end);
	//����ʱ��
	int ms=end.wMilliseconds-start.wMilliseconds;
	int s=end.wSecond-start.wSecond;
	int min=end.wMinute-start.wMinute;
	int h=end.wHour-start.wHour;
	if(ms<0)
	{
		s--;
		ms+=1000;
	}
	if(s<0)
	{
		min--;
		s+=60;
	}
	if(min<0)
	{
		h--;
		min+=60;
	}
	if(h<0) 
	{
		h+=24;	
	} 
	printf("%dСʱ%d����%d��%d����0΢��\n",h,min,s,ms);
	return 0;
}
