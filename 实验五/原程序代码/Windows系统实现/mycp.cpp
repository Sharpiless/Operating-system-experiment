#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

void CopyFile(char *sourcefile, char *objectfile) //�����ļ�
{
	WIN32_FIND_DATA lpfindfiledata;
	HANDLE hfindfile = FindFirstFile(sourcefile, &lpfindfiledata);
	HANDLE hsource = CreateFile(sourcefile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//�ļ���������ģʽ������ģʽ����ȫ���ԣ���δ��������ԣ����
	HANDLE hobject = CreateFile(objectfile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LONG size = lpfindfiledata.nFileSizeLow - lpfindfiledata.nFileSizeHigh; //���㳤��
	//�����ļ�
	int *buffer = new int[size];
	DWORD temp;
	ReadFile(hsource, buffer, size, &temp, NULL);
	WriteFile(hobject, buffer, size, &temp, NULL);
	//����ʱ������һ��
	SetFileTime(hobject, &lpfindfiledata.ftCreationTime, &lpfindfiledata.ftLastAccessTime, &lpfindfiledata.ftLastWriteTime);
	CloseHandle(hfindfile); //�رվ��
	CloseHandle(hsource);
	CloseHandle(hobject);
}

void CopyDirectory(char *sourcefile, char *objectfile) //����Ŀ¼
{
	WIN32_FIND_DATA lpfindfiledata;
	char source[1000], object[1000];
	strcpy(source, sourcefile);
	strcpy(object, objectfile);
	strcat(source, "\\*.*");
	strcat(object, "\\");
	HANDLE hfindfile = FindFirstFile(source, &lpfindfiledata);
	for (; FindNextFile(hfindfile, &lpfindfiledata) != 0;) //���������ļ�
	{
		if (lpfindfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ //��ΪĿ¼
			if (strcmp(lpfindfiledata.cFileName, ".") != 0 && strcmp(lpfindfiledata.cFileName, "..") != 0)
			{ //���Ȳ��ǵ�ǰĿ¼Ҳ���Ǹ�Ŀ¼���򴴽���Ŀ¼
				memset(source, 0, sizeof(source));
				strcpy(source, sourcefile);
				strcat(source, "\\");
				strcat(source, lpfindfiledata.cFileName);
				strcat(object, lpfindfiledata.cFileName);
				CreateDirectory(object, NULL);
				CopyDirectory(source, object); //�ݹ����,������Ŀ¼��������
				//����ʱ������һ��
				HANDLE hsourcefile = CreateFile(source, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
				HANDLE hobjectfile = CreateFile(object, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
				FILETIME creationtime, accesstime, writetime;
				GetFileTime(hsourcefile, &creationtime, &accesstime, &writetime);
				SetFileTime(hobjectfile, &creationtime, &accesstime, &writetime);
				strcpy(object, objectfile);
				strcat(object, "\\");
			}
		}
		else //��Ϊ�ļ�
		{
			memset(source, 0, sizeof(source));
			strcpy(source, sourcefile);
			strcat(source, "\\");
			strcat(source, lpfindfiledata.cFileName);
			strcat(object, lpfindfiledata.cFileName);
			CopyFile(source, object);
			strcpy(object, objectfile);
			strcat(object, "\\");
		}
	}
	CloseHandle(hfindfile);
}

int main(int argc, char *argv[])
{
	WIN32_FIND_DATA lpfindfiledata;
	if (FindFirstFile(argv[1], &lpfindfiledata) == INVALID_HANDLE_VALUE) //�����ļ�
	{
		printf("findfirstfile error!\n");
		exit(0);
	}
	if (FindFirstFile(argv[2], &lpfindfiledata) == INVALID_HANDLE_VALUE)
	{
		CreateDirectory(argv[2], NULL); //����Ŀ¼�ļ�
		printf("createdirectory succeed!\n");
	}
	CopyDirectory(argv[1], argv[2]);
	//����Ŀ¼ʱ������һ��
	HANDLE hsourcefile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	HANDLE hobjectfile = CreateFile(argv[2], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	FILETIME creationtime, accesstime, writetime;
	GetFileTime(hsourcefile, &creationtime, &accesstime, &writetime);
	SetFileTime(hobjectfile, &creationtime, &accesstime, &writetime);
	printf("copy completed!\n");
	return 0;
}
