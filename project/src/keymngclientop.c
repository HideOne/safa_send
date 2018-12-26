#include"string.h"
#include"stdlib.h"

#include"keymngMsg.h"
#include"keymngclientop.h"
#include"itcastderlog.h"
#include"poolsocket.h"
#include"keymng_shmop.h"
void menuPage()
{
	system("clear");
	printf("==============================\n");
	printf("=      1.密钥协商            =\n");
	printf("=      2.密钥校验            =\n");
	printf("=      3.密钥注销            =\n");
	printf("=      4.密钥查看            =\n");
	printf("=      0.退出系统            =\n");
	printf("==============================\n");
}



int MngClient_InitInfo (MngClient_Info *pCltInfo) 
{
	int ret = 0;
	strcpy(pCltInfo->AuthCode, "000000");
	strcpy(pCltInfo->clientId, "1111");
	strcpy(pCltInfo->serverId, "0001");
	strcpy(pCltInfo->serverip, "127.0.0.1");
	pCltInfo->serverport = 8686;
	pCltInfo->maxnode = 1;
	pCltInfo->shmkey = 0x1111;
	pCltInfo->shmhdl = 0;


	// 初始化共享内存
	ret = Mng_shm_init(pCltInfo->shmkey, pCltInfo->maxnode, &pCltInfo->shmhdl);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "Mng_shm_init func is err\n");
		return ret;
	}

	return ret;
}



/*****************************************************************************
 函 数 名  : MsgAgree
 功能描述  : 密钥协商
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月8日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int MsgAgree( MngClient_Info *pCltInfo  )
{
    int ret = 0;
	int dex = 0;
	un_char *reqData = NULL;
	unit_4 reqDataLen = 0;
	un_char *recvData = NULL;
	unit_4 recvDataLen = 0;
	unit_4 time = 3;
	unit_4 connfd = -1;
	unit_4 msgType = 0;
	MsgKey_Res* recvRes;
	NODESHMINFO_STRU shmData = {0};
	
	memset(&shmData, 0, sizeof(NODESHMINFO_STRU));
	
	// 客户端请求报文
	MsgKey_Req req;
	strcpy(req.AuthCode, pCltInfo->AuthCode);
	strcpy(req.clientId, pCltInfo->clientId);
	strcpy(req.serverId, pCltInfo->serverId);
	
	req.cmdType = KeyMng_NEWorUPDATE;
	for ( dex = 0 ; dex < 64 ; dex++ )
	{
	    req.r1[dex] = 'a' + dex;
	}

	
	ret = MsgEncode(&req, ID_MsgKey_Req, &reqData, &reqDataLen);
	if(ret != 0) {
	    LOG(4, ret, "MsgEncode fun err\n");
		goto END;
	}
	

	
	// 初始化socket
	ret = sckClient_init();
	if(ret != 0) {
	    LOG(4, ret, "sckClient_init fun err\n");
		goto END;
	}
	
	// 连接socket
	ret = sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, time, &connfd);
	if(ret != 0) {
	    LOG(4, ret, "sckClient_connect fun err\n");
		goto END;
	}

	// 发送报文
	ret = sckClient_send(connfd, time, reqData, reqDataLen);
	if(ret != 0) {
	    LOG(4, ret, "sckClient_send fun err\n");
		goto END;
	}

	// 等待服务器返回报文
	ret = sckClient_rev(connfd, time, &recvData, &recvDataLen);
	if(ret == Sck_ErrTimeOut) {
	    LOG(4, ret, "server recv time out  fun in sckClient_rev\n");
		goto END;
	} else if (ret != 0) {
	    LOG(4, ret, "sckClient_rev fun Err\n");
	}
	

	// 解码报文
	ret = MsgDecode(recvData, recvDataLen,(void **)&recvRes, &msgType);
	if(ret != 0)
	{
	    LOG(4, ret, "MsgDecode func err\n");
		goto END;
	}
	


	// 生成密钥
	strcpy(shmData.clientId, pCltInfo->clientId);
	strcpy(shmData.serverId, pCltInfo->serverId);
	for ( dex = 0 ; dex < 64 ; dex++ )
	{
	    shmData.seckey[dex*2] = req.r1[dex];
		shmData.seckey[dex*2+1] = recvRes->r2[dex];
	}


	// 写入共享内存
	ret = Mng_shm_write(pCltInfo->shmhdl, pCltInfo->maxnode, &shmData);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "Mng_shm_write func err\n");
		goto END;
	}
	printf("网点信息已写入共享内存...\n");

	printf("================agree key success==============\n");
	sckClient_destroy();
	
	
	
	return ret;
	

	END:
		return ret;
}

/*****************************************************************************
 函 数 名  : MsgChcek
 功能描述  : 密钥校验
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月8日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int  MsgChcek( MngClient_Info *pCltInfo  )
{
    return 0;
}

/*****************************************************************************
 函 数 名  : 密钥注销
 功能描述  : int
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月8日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int  MsgDestroy(MngClient_Info *pCltInfo  )
{
   return 0; 
}

int MsgKeyLook(MngClient_Info *pCltInfo)
{

	return 0;
}
