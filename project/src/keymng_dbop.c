#include"icdbapi.h"
#include"stdio.h"
#include"string.h"
#include"stdlib.h"
#include"keymng_dbop.h"
#include"keymng_shmop.h"
#include"itcastderlog.h"

/*****************************************************************************
 函 数 名  : dbGetSckeyId
 功能描述  : 得到密钥的id
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int 成功返回0
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月22日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int dbGetSckeyId(void* dbhandle, int* keyid)
{
	unit_4 ret = 0;
	ICDBHandle handle = (ICDBHandle)dbhandle;
	// int waitTIme = 3;
	ICDBRow row = {0};
	ICDBField file = {0};
	unit_4 ikeytmp = 0;
	row.field = &file;
	row.fieldCount = 1;
	
	char sql[128] = {0};
	file.cont = (char*)&ikeytmp;
	file.contLen = sizeof(ikeytmp);
//    ret = IC_DBApi_ConnGet(&handle, waitTIme, int nsTimeout, 0);
//	if(ret != 0) 
//	{
//	    LOG_ERR_FUN(ret, "IC_DBApi_ConnGet func is err");
//		return ret;
//	}
	// 开启事务
	// IC_DBApi_BeginTran(handle);
	sprintf(sql, "select ikeysn from SECMNG.KEYSN for update");

	ret = IC_DBApi_ExecSelSql(handle, sql, &row);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_ExecSelSql func is err");
		return ret;
	}

	memset(sql, 0, 128);
	// int key = atoi(row.field->cont);
	sprintf(sql, "update KEYSN set IKEYSN = %d", ikeytmp + 1);
	
	ret = IC_DBApi_ExecNSelSql(handle, sql);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_ExecNSelSql func is err");
		return ret;
	}

	// IC_DBApi_Commit(handle);

	*keyid = ikeytmp;
	return ret;
	
}

int dbWriteClient2Sql(void* dbhandle, NODESHMINFO_STRU* shmNode)
{
    unit_4 ret = 0;
	ICDBHandle handle = (ICDBHandle)dbhandle;
	// unit_4 waitTIme = 3;
	unit_4 tmpDataLen = 0;
	char dbTime[128] = {0};
	char sql[1024] = {0};
	char tmpSeckey[1024] = {0};
	// 开启事务
	// IC_DBApi_BeginTran(handle);
	ret = IC_DBApi_GetDBTime(handle, dbTime);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_GetDBTime func is err");
		return ret;
	}

	// base64 编码 seckey
	ret = IC_DBApi_Der2Pem(shmNode->seckey, sizeof(shmNode->seckey), tmpSeckey, &tmpDataLen);
	if(ret != 0) 
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_Der2Pem func is err");
		return ret;
	}

	// 组织sql
	sprintf(sql, "insert into SECMNG.SECKYEINFO (CLIENTID, SERVERID, KEYID, CREATETIME, STATE, SECKEY) values ('%s', '%s', %d, '%s', %d, '%s')",
		shmNode->clientId, shmNode->serverId, shmNode->seckeyid, dbTime, shmNode->status, tmpSeckey);

	// 执行sql
	ret = IC_DBApi_ExecNSelSql(handle, sql);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IC_DBApi_ExecNSelSql func is err");
		return ret;
	}

	return ret;

}








