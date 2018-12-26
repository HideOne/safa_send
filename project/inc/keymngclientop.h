#ifndef __KEYMNGCLIENTOP_H__
#define __KEYMNGCLIENTOP_H__

#include"stdio.h"

#define		MngClt_OK				0			//正确
#define		MngClt_ParamErr			301			//输入参数失败
#define		MngClt_NoNetPointErr	302			//共享内存中，没有找到网点信息
#define		MngClt_NodeMaxCount		303			//共享内存中，超过最大网点

typedef struct _MngClient_Info
{
	char			clientId[12];	//客户端编号
	char			AuthCode[16];	//认证码
	char			serverId[12];	//服务器端编号
	
	char			serverip[32];
	int 			serverport;
	
	int				maxnode; 		//最大网点数 客户端默认1个
	int 			shmkey;	 		//共享内存keyid 创建共享内存时使用	 
	int 			shmhdl; 		//共享内存句柄	
}MngClient_Info;

int MngClient_InitInfo(MngClient_Info *pCltInfo);

void menuPage();

int MsgAgree(MngClient_Info *pCltInfo);

int MsgChcek(MngClient_Info *pCltInfo);

int MsgDestroy(MngClient_Info *pCltInfo);

int MsgKeyLook(MngClient_Info *pCltInfo);




#endif
