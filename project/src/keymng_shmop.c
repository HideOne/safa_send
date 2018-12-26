#include <stdio.h>
#include <stdlib.h>


#include "myipc_shm.h"
#include"keymng_shmop.h"
#include"itcastderlog.h"
/*****************************************************************************
 函 数 名  : Mng_shm_init
 功能描述  : 初始化共享内存  有则打开  无则创建
 输入参数  : 	 int key         
             int maxNodeNum   // 共享内存最大数
             int * shmhdl    
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月15日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int Mng_shm_init(int key, int maxNodeNum, int * shmhdl)
{	
	unit_4 ret = 0;
    ret = IPC_OpenShm(key, maxNodeNum * sizeof(NODESHMINFO_STRU), shmhdl);
	if(ret == MYIPC_NotEXISTErr)
	{
		printf("共享内存不存在 正在创建...\n");
		ret = IPC_CreatShm(key, maxNodeNum * sizeof(NODESHMINFO_STRU), shmhdl);
		if(ret != 0)
		{
			LOG_ERR_FUN(ret, "Mng_shm_init func is err");
			return ret;
		}
		printf("共享内存创建成功...\n");
		// 清空共享内存数据
		// 关联共享内存
		void* star_addr = NULL;
		ret = IPC_MapShm(*shmhdl, &star_addr);
		if(ret != 0)
		{
			LOG_ERR_FUN(ret, "IPC_MapShm func is err");
			return ret;
		}

		memset(star_addr, 0, maxNodeNum * sizeof(NODESHMINFO_STRU));

		ret = IPC_UnMapShm(star_addr);
		if(ret != 0)
		{
			LOG_ERR_FUN(ret, "IPC_UnMapShm func is err");
			return ret;
		}
		printf("共享内存初始化完毕");
	} 
	else if (ret == 0)
	{
	    printf("共享内存存在使用旧的共享内存...\n");
	}
	else
	{
	    LOG_ERR_FUN(ret, "IPC_OpenShm func is err");
	}
	

	return ret;
	
}


/*****************************************************************************
 函 数 名  : Mng_shm_write
 功能描述  : 写入共享内存
 输入参数  : 	 int key                     
             int maxNodeNum              
             NODESHMINFO_STRU* inStruct  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月17日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int Mng_shm_write(int shmdl, int maxNodeNum, NODESHMINFO_STRU* inStruct)
{

	unit_4 ret = 0;
	unit_4 dex = 0;
	void* retNode = NULL;
	NODESHMINFO_STRU tmpNode;
	NODESHMINFO_STRU* shm_node = NULL;

	memset(&tmpNode, 0, sizeof(NODESHMINFO_STRU));
	ret = IPC_MapShm(shmdl, &retNode);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IPC_MapShm func is err in Mng_shm_write func");
		goto END;
	}

//	如果共享内存中存在 该数据改之
	for(dex = 0; dex < maxNodeNum; dex++)
	{
	    shm_node = (NODESHMINFO_STRU*)retNode + dex * sizeof(NODESHMINFO_STRU);
		if( strcmp(shm_node->clientId, inStruct->clientId) == 0 && 
			strcmp(shm_node->serverId, inStruct->serverId) == 0 )
		{
		    LOG_INFO_FUN(ret, "检测到共享内存中存在此网点信息....");
			memcpy(shm_node, inStruct, sizeof(NODESHMINFO_STRU));
			goto END;
		}
	}

//	如果没有
	for(dex = 0; dex < maxNodeNum; dex++)
	{
		shm_node = (NODESHMINFO_STRU*)retNode + dex * sizeof(NODESHMINFO_STRU);
	    if(memcmp(shm_node, &tmpNode, sizeof(NODESHMINFO_STRU)) == 0)
	    {
	        LOG_INFO_FUN(ret, "检测到共享内存有空位子....");
			memcpy(shm_node, inStruct, sizeof(NODESHMINFO_STRU));
			goto END;
	    }
	}

	if(dex == maxNodeNum)
	{
		ret = KeyShm_MemOut;
	    LOG_ERR_FUN(ret, "共享内存已满..........");
		goto END;
	}

	END:
		if(retNode)
		{
		    IPC_UnMapShm(retNode);
		}
		return ret;

}



/*****************************************************************************
 函 数 名  : Mng_shm_read
 功能描述  : 读出共享内存
 输入参数  : int key                       
             int maxNodeNum                
             NODESHMINFO_STRU** outStruct  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月17日
    作    者   : lihang
    修改内容   : 新生成函数

*****************************************************************************/
int Mng_shm_read(int shmdl, int maxNodeNum, char* clientId, char* serverId, NODESHMINFO_STRU* outStru)
{
	unit_4 ret = 0;
	unit_4 dex = 0;
	void* mapaddr = NULL;
	NODESHMINFO_STRU *shnnode = NULL;
	
	
	ret = IPC_MapShm(shmdl, &mapaddr);
	if(ret != 0)
	{
	    LOG_ERR_FUN(ret, "IPC_MapShm func is err in Mng_shm_write func");
		goto END;
	}

	for ( dex = 0 ; dex < maxNodeNum ; dex++ )
	{
		shnnode = (NODESHMINFO_STRU*)mapaddr + dex * sizeof(NODESHMINFO_STRU);
	    if( strcpy(shnnode->serverId, serverId) == 0 &&
			strcpy(shnnode->clientId, clientId) == 0)
	    {
	        LOG_INFO_FUN(ret, "已经找到共享内存的数据....");
			memcpy(outStru, shnnode, sizeof(NODESHMINFO_STRU));
			goto END;
	    }
		else
		{
		    LOG_INFO_FUN(ret, "共享内存中没有此网点信息....");
			goto END;
		}
	}


	
	END:
	if(mapaddr)
	{
	    IPC_UnMapShm(mapaddr);
	}
	return ret;
	
	
}



