#include"itcastderlog.h"
#include"keymngserverop.h"
#include"poolsocket.h"
#include"stdlib.h"
#include"stdio.h"
#include"string.h"
#include"keymngMsg.h"
#include"keymng_shmop.h"
#include"keymng_dbop.h"
#include"icdbapi.h"


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
	strcpy(server_info->dbuse, "SECMNG");
	strcpy(server_info->dbpasswd, "SECMNG");
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
	ret = IC_DBApi_PoolInit(1, server_info->dbsid, server_info->dbuse, server_info->dbpasswd);
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
	unit_4 seckeyid = 0;
	ICDBHandle handle = {0};
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

	
	
	strcpy(shmnode.serverId, server_info->serverId);
	strcpy(shmnode.clientId, msgKeyReq->clientId);


	// 获得数据库句柄
	ret = IC_DBApi_ConnGet(&handle, 3, 0);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_ConnGet func is err\n");
		return ret;
	}
	
	// 开启事务
	ret = IC_DBApi_BeginTran(handle);
	if(ret != 0)
	{
	   	LOG_ERR_FUN(ret, "IC_DBApi_ConnGet func is err\n");
		return ret; 
	}

	// 

	
	ret = dbGetSckeyId(handle, &seckeyid);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "dbGetSckeyId func is err\n");
		return ret; 
	}

	shmnode.seckeyid = seckeyid;
	
	
	msgKeyRes.seckeyid = shmnode.seckeyid;
	printf("密钥生成成功		  编号为%d================\n", msgKeyRes.seckeyid);

	// 写入共享内存
	ret = Mng_shm_write(server_info->shmhdl, server_info->maxnode, &shmnode);
	if ( ret != 0 )
	{
	    LOG_ERR_FUN(ret, "Mng_shm_write func err");
		return ret;
	}

	// 写入数据库
	ret = dbWriteClient2Sql(handle, &shmnode);
	if(ret != 0)
	{
		IC_DBApi_Rollback(handle);
	    LOG_ERR_FUN(ret, "dbWriteClient2Sql func err");
		return ret;
	}

	IC_DBApi_Commit(handle);
	IC_DBApi_ConnFree(handle, 0);
		
	// 编码回应报文
	ret = MsgEncode((void *) &msgKeyRes, ID_MsgKey_Res, outData, dataLen);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "MsgEncode func err");
		return ret;
	}

    return 0;
}


int MngServer_Check(MngServer_Info* server_info, MsgKey_Req* msgKeyReq)
{

	if(server_info == NULL || msgKeyReq == NULL)
	{
	    LOG_ERR_FUN(ret, "server_info is null");
		return -1;
	}
	
	NODESHMINFO_STRU shmnode = {0};
    ret = Mng_shm_read(server_info->shmhdl, server_info->maxnode, msgKeyReq->clientId, server_info->serverId, &shmnode);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "Mng_shm_read func err");
		return ret;
	}

	
	if(strncmp(msgKeyReq->AuthCode, shmnode->seckey, sizeof(msgKeyReq->AuthCode) == 0)
	{
	    printf("==============认证成功 密匙检验成功==============\n");
		LOG_INFO_FUN(ret, "认证成功 密匙检验成功");
	}
	else
	{
		printf("==============认证失败 密匙检验不一致==============\n");
	    LOG_ERR_FUN(ret, "====密匙检验不一致=====");
		return -1;
	}

	
}



