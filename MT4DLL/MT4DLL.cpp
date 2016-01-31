#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#include "picojson.h"

//http://blog.livedoor.jp/akf0/archives/51585502.html

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

#define PORT    9999    /* Listen����|�[�g */
#define MAXDATA 1024    /* ��M�o�b�t�@�T�C�Y */

static SOCKET s;         //�\�P�b�g
static SOCKET s1;         //�\�P�b�g

EXPORT double movingAverages(double a, double b, double c, double d);

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
	){



	switch (fdwReason) {

	case DLL_PROCESS_ATTACH: // DLL���v���Z�X�̃A�h���X��ԂɃ}�b�s���O���ꂽ�B
		break;

	case DLL_THREAD_ATTACH: // �X���b�h���쐬����悤�Ƃ��Ă���B
		break;

	case DLL_THREAD_DETACH: // �X���b�h���j������悤�Ƃ��Ă���B

		break;

	case DLL_PROCESS_DETACH: // DLL�̃}�b�s���O����������悤�Ƃ��Ă���B
		break;

	}

	return TRUE;
}

EXPORT double movingAverages(double a, double b, double c, double d) {
	return((a + b + c + d) / 4);
}

EXPORT wchar_t* __stdcall my_function(wchar_t* x, wchar_t* y)
{
	wchar_t dest[1024];

	swprintf_s(dest, 1024, L"%s + %s", x, y);

	return dest;
}

EXPORT int __stdcall my_function2(wchar_t* x)
{
	return sizeof(x);
}

EXPORT void __stdcall DeInitSocket()
{
	//�\�P�b�g�ʐM�̏I��
	closesocket(s1);
	closesocket(s);
	WSACleanup();
}

EXPORT int __stdcall InitSocket()
{


	//�ڑ���������N���C�A���g�[���̏��

	struct sockaddr_in source;

	int result;


	char ret;




	//���M���̒[������o�^����

	memset(&source, 0, sizeof(source));

	source.sin_family = AF_INET;

	//�|�[�g�ԍ��̓N���C�A���g�v���O�����Ƌ���

	source.sin_port = htons(PORT);

	source.sin_addr.s_addr = htonl(INADDR_ANY);



	//�\�P�b�g�ʐM�̊J�n����

	WSADATA data;

	result = WSAStartup(MAKEWORD(2, 0), &data);

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("�\�P�b�g�ʐM�����G���[\n");

		return 1;
	}



	//�\�P�b�g�̐���

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s < 0){

		printf("%d\n", GetLastError());

		printf("�\�P�b�g�����G���[\n");
		return 2;

	}



	//�\�P�b�g�̃o�C���h

	result = bind(s, (struct sockaddr *)&source, sizeof(source));

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("�o�C���h�G���[\n");
		return 3;

	}



	//�ڑ��̋���

	result = listen(s, 1);

	if (result < 0){

		printf("�ڑ����G���[\n");
		return 4;

	}




	//�N���C�A���g����ʐM������܂őҋ@

	s1 = accept(s, NULL, NULL);

	if (s1 < 0){

		printf("�ҋ@�G���[\n");
		return 5;
	}

	printf("�ڑ��J�n\n");

	return 0;
}

EXPORT wchar_t* __stdcall ResvSocket()
{


	int result;          //�߂�l
	char buffer[MAXDATA];  //��M�f�[�^�̃o�b�t�@�̈�
	memset(&buffer, '\0', MAXDATA * sizeof(char));

	//�N���C�A���g���瑗�M���ꂽ�f�[�^�̎�M

	result = recv(s1, buffer, sizeof(buffer), 0);

	if (result < 0){

		return L"ERROR";

	}

	wchar_t ret_buffer[MAXDATA];
	size_t ret;


	mbstowcs_s(&ret, ret_buffer, buffer, MAXDATA);

	return ret_buffer;
}



EXPORT int __stdcall SendSocket(wchar_t* msg)
{
	int result;
	char msg_char[MAXDATA];
	size_t size;

	wcstombs_s(&size, msg_char, msg, MAXDATA);

	result = send(s1, msg_char, sizeof(char) * MAXDATA, 0);

	return result;
}

//-----------sockets-----------//



/*
EXPORT int init_socket(){

	SOCKET s, s1;         //�\�P�b�g

	int result;          //�߂�l

	//�ڑ���������N���C�A���g�[���̏��

	struct sockaddr_in source;

	char buffer[1024];  //��M�f�[�^�̃o�b�t�@�̈�

	char ans[] = "���M����";

	char ret;



	memset(&buffer, '\0', sizeof(buffer));

	//���M���̒[������o�^����

	memset(&source, 0, sizeof(source));

	source.sin_family = AF_INET;

	//�|�[�g�ԍ��̓N���C�A���g�v���O�����Ƌ���

	source.sin_port = htons(7000);

	source.sin_addr.s_addr = htonl(INADDR_ANY);



	//�\�P�b�g�ʐM�̊J�n����

	WSADATA data;

	result = WSAStartup(MAKEWORD(2, 0), &data);

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("�\�P�b�g�ʐM�����G���[\n");

	}



	//�\�P�b�g�̐���

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s < 0){

		printf("%d\n", GetLastError());

		printf("�\�P�b�g�����G���[\n");

	}



	//�\�P�b�g�̃o�C���h

	result = bind(s, (struct sockaddr *)&source, sizeof(source));

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("�o�C���h�G���[\n");

	}



	//�ڑ��̋���

	result = listen(s, 1);

	if (result < 0){

		printf("�ڑ����G���[\n");

	}



	printf("�ڑ��J�n\n");



	//�N���C�A���g����ʐM������܂őҋ@

	s1 = accept(s, NULL, NULL);

	if (s1 < 0){

		printf("�ҋ@�G���[\n");

	}



	//�N���C�A���g���瑗�M���ꂽ�f�[�^�̎�M

	result = recv(s1, buffer, 10, 0);

	if (result < 0){

		printf("��M�G���[\n");

	}

	printf("%s����M���܂���", buffer);



	//�N���C�A���g�փf�[�^�𑗐M����

	result = send(s1, ans, 10, 0);



	printf("�ڑ��I��\n");

	closesocket(s1);



	//�\�P�b�g�ʐM�̏I��

	WSACleanup();



	printf("�����L�[�������ĉ�����\n");

	scanf("%c", &ret);



	return 0;

}*/

//-----------------------------//