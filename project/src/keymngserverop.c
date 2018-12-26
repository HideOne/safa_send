#include"itcastderlog.h"
#include"keymngserverop.h"
#include"poolsocket.h"
#include"stdlib.h"
#include"stdio.h"
#include"string.h"
#include"keymngMsg.h"
#include"keymng_shmop.h"

static unit_4 seckeyid = 100;

/*****************************************************************************
 函 数 名  : mngServer_info
 功能描述  : 初始化服务器信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月10日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int mngServer_info(MngServer_Info* server_info)
{
	unit_4 ret = 0;
	strcpy(server_info->serverId, "0001");
	strcpy(server_info->serverip, "127.0.0.1");
	strcpy(server_info->dbuse, "SENMNG");
	strcpy(server_info->dbpasswd, "SENMNG");
	strcpy(server_info->dbsid, "orcl");
	server_info->serverport = 8686;
	server_info->shmhdl = 0;
	server_info->shmkey = 0x1111;
	server_info->maxnode = 1;
	server_info->dbpoolnum = 2;


	// 初始化共享内存
	ret = Mng_shm_init(server_info->shmkey, server_info->maxnode, &server_info->shmhdl);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "Mng_shm_init func is err\n");
		return ret;
	}

	// 初始化数据库
	ret = IC_DBApi_PoolInit(server_info->maxnode, server_info->dbsid, server_info->dbuse, server_info->dbpasswd);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_PoolInit func is err\n");
		return ret;
	}
	
    return 0;
}



int MngServer_Agree(MngServer_Info* server_info, MsgKey_Req* msgKeyReq, un_char** outData, unit_4* dataLen)
{
	unit_4 ret;
	unit_4 dex = 0;
	if(strcmp(msgKeyReq->serverId, server_info->serverId) != 0 ) {
		ret = MngSvr_ParamErr;
	    LOG_ERR_FUN(ret, "serverId is diffrent\n");
	}

	MsgKey_Res msgKeyRes = {0};
	NODESHMINFO_STRU shmnode = {0};
	memset(&shmnode, 0, sizeof(NODESHMINFO_STRU));
	
	strcpy(msgKeyRes.serverId, msgKeyReq->serverId);
	strcpy(msgKeyRes.clientId, msgKeyReq->clientId);

	
	// 生成随机数
	for(dex = 0; dex < 64; dex++)
	{
		msgKeyRes.r2[dex] = 'a' + dex;
	}

	msgKeyRes.rv = 0;

	
	// 写入共享内存
	strcpy(shmnode.serverId, server_info->serverId);
	strcpy(shmnode.clientId, msgKeyReq->clientId);
	
	shmnode.seckeyid = seckeyid++;
	
	
	msgKeyRes.seckeyid = shmnode.seckeyid;
	printf("密钥生成成功		  编号为%d================\n", msgKeyRes.seckeyid);
	
	ret = Mng_shm_write(server_info->shmhdl, server_info->maxnode, &shmnode);
	if ( ret != 0 )
	{
	    LOG_ERR_FUN(ret, "Mng_shm_write func err");
		return ret;
	}

	// 写入数据库
	
		
	// 编码回应报文
	ret = MsgEncode((void *) &msgKeyRes, ID_MsgKey_Res, outData, dataLen);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "MsgEncode func err");
		return ret;
	}

    return 0;
}



