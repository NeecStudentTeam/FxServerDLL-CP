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

#define PORT    9999    /* Listenするポート */
#define MAXDATA 1024    /* 受信バッファサイズ */

static SOCKET s;         //ソケット
static SOCKET s1;         //ソケット

EXPORT double movingAverages(double a, double b, double c, double d);

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
	){



	switch (fdwReason) {

	case DLL_PROCESS_ATTACH: // DLLがプロセスのアドレス空間にマッピングされた。
		break;

	case DLL_THREAD_ATTACH: // スレッドが作成されようとしている。
		break;

	case DLL_THREAD_DETACH: // スレッドが破棄されようとしている。

		break;

	case DLL_PROCESS_DETACH: // DLLのマッピングが解除されようとしている。
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
	//ソケット通信の終了
	closesocket(s1);
	closesocket(s);
	WSACleanup();
}

EXPORT int __stdcall InitSocket()
{


	//接続を許可するクライアント端末の情報

	struct sockaddr_in source;

	int result;


	char ret;




	//送信元の端末情報を登録する

	memset(&source, 0, sizeof(source));

	source.sin_family = AF_INET;

	//ポート番号はクライアントプログラムと共通

	source.sin_port = htons(PORT);

	source.sin_addr.s_addr = htonl(INADDR_ANY);



	//ソケット通信の開始準備

	WSADATA data;

	result = WSAStartup(MAKEWORD(2, 0), &data);

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("ソケット通信準備エラー\n");

		return 1;
	}



	//ソケットの生成

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s < 0){

		printf("%d\n", GetLastError());

		printf("ソケット生成エラー\n");
		return 2;

	}



	//ソケットのバインド

	result = bind(s, (struct sockaddr *)&source, sizeof(source));

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("バインドエラー\n");
		return 3;

	}



	//接続の許可

	result = listen(s, 1);

	if (result < 0){

		printf("接続許可エラー\n");
		return 4;

	}




	//クライアントから通信があるまで待機

	s1 = accept(s, NULL, NULL);

	if (s1 < 0){

		printf("待機エラー\n");
		return 5;
	}

	printf("接続開始\n");

	return 0;
}

EXPORT wchar_t* __stdcall ResvSocket()
{


	int result;          //戻り値
	char buffer[MAXDATA];  //受信データのバッファ領域
	memset(&buffer, '\0', MAXDATA * sizeof(char));

	//クライアントから送信されたデータの受信

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

	SOCKET s, s1;         //ソケット

	int result;          //戻り値

	//接続を許可するクライアント端末の情報

	struct sockaddr_in source;

	char buffer[1024];  //受信データのバッファ領域

	char ans[] = "送信成功";

	char ret;



	memset(&buffer, '\0', sizeof(buffer));

	//送信元の端末情報を登録する

	memset(&source, 0, sizeof(source));

	source.sin_family = AF_INET;

	//ポート番号はクライアントプログラムと共通

	source.sin_port = htons(7000);

	source.sin_addr.s_addr = htonl(INADDR_ANY);



	//ソケット通信の開始準備

	WSADATA data;

	result = WSAStartup(MAKEWORD(2, 0), &data);

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("ソケット通信準備エラー\n");

	}



	//ソケットの生成

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s < 0){

		printf("%d\n", GetLastError());

		printf("ソケット生成エラー\n");

	}



	//ソケットのバインド

	result = bind(s, (struct sockaddr *)&source, sizeof(source));

	if (result < 0){

		printf("%d\n", GetLastError());

		printf("バインドエラー\n");

	}



	//接続の許可

	result = listen(s, 1);

	if (result < 0){

		printf("接続許可エラー\n");

	}



	printf("接続開始\n");



	//クライアントから通信があるまで待機

	s1 = accept(s, NULL, NULL);

	if (s1 < 0){

		printf("待機エラー\n");

	}



	//クライアントから送信されたデータの受信

	result = recv(s1, buffer, 10, 0);

	if (result < 0){

		printf("受信エラー\n");

	}

	printf("%sを受信しました", buffer);



	//クライアントへデータを送信する

	result = send(s1, ans, 10, 0);



	printf("接続終了\n");

	closesocket(s1);



	//ソケット通信の終了

	WSACleanup();



	printf("何かキーを押して下さい\n");

	scanf("%c", &ret);



	return 0;

}*/

//-----------------------------//