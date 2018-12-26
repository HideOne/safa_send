
// keymngserverop.h
#ifndef _KEYMNG_ServerOp_H_
#define _KEYMNG_ServerOp_H_

#include"keymngMsg.h"

#ifdef __cplusplus
extern "C" {
#endif

//keymngserver 错误码

#define		MngSvr_OK				0		//正确
#define		MngSvr_ParamErr			301		//输入参数失败
#define		MngSvr_NoNetPointErr	302		//共享内存中，没有找到网点信息
#define		MngSvr_NodeMaxCount		303		//共享内存中，超过最大网点
#define		MngSvr_CheckErr			304		//共享内存中，超过最大网点


typedef struct _MngServer_Info
{
	char			serverId[12];	//服务器端编号
	
	//数据库连接池句柄	
	char			dbuse[24]; 		//数据库用户名
	char			dbpasswd[24]; 	//数据库密码
	char			dbsid[24]; 		//数据库sid
	int				dbpoolnum; 		//数据库池 连接数
	
	char			serverip[24];
	int 			serverport;
	
	//共享内存配置信息
	int				maxnode; //最大网点树 客户端默认1个
	int 			shmkey;	 //共享内存keyid 创建共享内存时使用	 
	int 			shmhdl; //共享内存句柄	
}MngServer_Info;


int mngServer_info(MngServer_Info* server_info);

int MngServer_Agree(MngServer_Info* server_info, MsgKey_Req* msgKeyReq, un_char** outData, unit_4* dataLen);




#ifdef __cplusplus
}
#endif

#endif