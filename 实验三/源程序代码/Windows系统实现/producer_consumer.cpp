#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

struct buffer//���建���� 
{
	int s[3];
	int head;
	int tail;
	int is_empty;
};

struct sharedmemory//���干���ڴ� 
{
	struct buffer data;
	HANDLE full;
	HANDLE empty;
	HANDLE mutex; 
};
//��ʾ����������
void showdata(struct sharedmemory* a)
{
	printf("��ǰ���������ݣ�");
	for(int i=a->data.head; ; )
	{
		printf("%d ",a->data.s[i]);
		i++;
		i%=3;
		if(i==a->data.tail)
		{
			printf("\n\n");
			return ;
		}
	}
}
//������¡���� 
PROCESS_INFORMATION StartClone(int id)
{
	TCHAR szFilename[MAX_PATH];
	TCHAR szCmdLine[MAX_PATH];
	PROCESS_INFORMATION pi;
	GetModuleFileName(NULL,szFilename,MAX_PATH);
	sprintf(szCmdLine,"\"%s\" %d",szFilename,id);
	STARTUPINFO si;   
	ZeroMemory(reinterpret_cast<void*>(&si),sizeof(si));
	si.cb=sizeof(si);
	//�����ӽ���
	BOOL bCreatOK=CreateProcess(szFilename,szCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	return pi; 
	
}
HANDLE MakeSharedFile()	//���������ڴ�
{
	//����һ����ʱ�ļ�ӳ�����
	HANDLE hMapping=CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,PAGE_READWRITE,0,sizeof(struct sharedmemory),"BUFFER");
	if(hMapping==NULL)
	{
		printf("CreateFileMapping error!\n");
		exit(0);
	}
	//���ļ�ӳ���ϴ�����ͼ��������ʼ���ַ
	LPVOID pData=MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(pData==NULL)
	{
		printf("MapViewOfFile error!\n");
		exit(0);
	}
	if(pData!=NULL)
		ZeroMemory(pData, sizeof(struct sharedmemory));
	UnmapViewOfFile(pData);  
	return(hMapping);
}
//��ȡһ��nλ�������
int getrandomnumber(int n)
{
	
	int temp=1;
	while(n--)
		temp*=10;
	int rn=rand();
	return rn%temp;
}
int main(int argc,char* argv[])
{
	int subid=0;
	HANDLE hMapping;
	if(argc>1)
	{
		//����в�������Ϊ�ӽ���id
		sscanf(argv[1],"%d",&subid); 
	 } 
	if(subid==0)//������ 
	{
		hMapping=MakeSharedFile();
		//���ļ�ӳ�� 
		HANDLE hFileMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"BUFFER");
    	if(hFileMapping==NULL) 
		{
        	printf("OpenFileMapping error!\n");
        	exit(0);
    	}
    	LPVOID pFile=MapViewOfFile(hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);
    	if(pFile==NULL) 
		{
        	printf("MapViewOfFile error!\n");
        	exit(0);
    	}
		struct sharedmemory *addr=(struct sharedmemory*)(pFile);
    	addr->data.head=0;
    	addr->data.tail=0;
    	addr->data.is_empty=1;
    	addr->empty=CreateSemaphore(NULL,3,3,"EMPTY");
    	addr->full=CreateSemaphore(NULL,0,3,"FULL");
    	addr->mutex=CreateSemaphore(NULL,1,1,"MUTEX");
    	UnmapViewOfFile(pFile);
    	pFile=NULL;
    	CloseHandle(hFileMapping);
    	//�����ӽ��� 
    	PROCESS_INFORMATION sub[5];
    	for(int i=0;i<5;i++)
			sub[i]=StartClone(++subid);
		//�ȴ��ӽ��̽���
		for(int i=0;i<5;i++)
			WaitForSingleObject(sub[i].hProcess,INFINITE);
		//�ر��ӽ��̾��
		for(int i=0;i<5;i++)
			CloseHandle(sub[i].hProcess);
	}
	else if(subid==1||subid==2)//������
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"BUFFER"); 
		if(hMap==NULL) 
		{
        	printf("OpenFileMapping error!\n");
        	exit(0);
    	}
    	LPVOID pFile=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);
    	if(pFile==NULL) 
		{
        	printf("MapViewOfFile error!\n");
        	exit(0);
    	}
		struct sharedmemory *addr=(struct sharedmemory*)(pFile);
    	addr->full=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    	addr->empty=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    	addr->mutex=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"MUTEX");	
    	for(int i=0;i<6;i++)
    	{
    		Sleep(getrandomnumber(3));
    		WaitForSingleObject(addr->empty,INFINITE);//P(empty)
    		WaitForSingleObject(addr->mutex,INFINITE);//P(mutex)
    		//�򻺳����������
    		int num=getrandomnumber(3);
			addr->data.s[addr->data.tail]=num;
			addr->data.tail=(addr->data.tail+1)%3;
			addr->data.is_empty=0;
			SYSTEMTIME time;
			GetLocalTime(&time);
			printf("ʱ�䣺%04d��%02d��%02d��%02dʱ%02d��%02d��\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
			printf("%d�������߽�%d���뻺������\n",subid-1,num);
			showdata(addr);
			ReleaseSemaphore(addr->mutex,1,NULL);//V(mutex)
			ReleaseSemaphore(addr->full,1,NULL);//V(full)
		}
		UnmapViewOfFile(pFile);
		pFile=NULL;
		CloseHandle(hMap);
	}
	else if(subid==3||subid==4||subid==5)//������
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"BUFFER"); 
		if(hMap==NULL) 
		{
        	printf("OpenFileMapping error!\n");
        	exit(0);
    	}
    	LPVOID pFile=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);
    	if(pFile==NULL) 
		{
        	printf("MapViewOfFile error!\n");
        	exit(0);
    	}
		struct sharedmemory *addr=(struct sharedmemory*)(pFile);
    	addr->full=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    	addr->empty=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    	addr->mutex=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"MUTEX");	
    	for(int i=0;i<4;i++)
    	{
    		Sleep(getrandomnumber(3));
    		WaitForSingleObject(addr->full,INFINITE);//P(full)
    		WaitForSingleObject(addr->mutex,INFINITE);//P(mutex)
    
    		int num=addr->data.s[addr->data.head];
			addr->data.head=(addr->data.head+1)%3;
			if(addr->data.head==addr->data.tail)
				addr->data.is_empty=1;
			else
				addr->data.is_empty=0;
			SYSTEMTIME time;
			GetLocalTime(&time);
			printf("ʱ�䣺%04d��%02d��%02d��%02dʱ%02d��%02d��\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
			printf("%d�������߽�%d�ӻ�����ȡ��\n",subid-3,num);
			if(addr->data.is_empty==0)
				showdata(addr);
			else
				printf("��ǰ������Ϊ��\n\n");
			ReleaseSemaphore(addr->mutex,1,NULL);//V(mutex)
			ReleaseSemaphore(addr->empty,1,NULL);//V(empty)
		}
		UnmapViewOfFile(pFile);
		pFile=NULL;
		CloseHandle(hMap);
	}
	CloseHandle(hMapping);
	hMapping=INVALID_HANDLE_VALUE;
//	printf("end\n");
	return 0;
}
