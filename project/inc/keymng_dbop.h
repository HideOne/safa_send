/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : keymng_dbop.h
  版 本 号   : 初稿
  作    者   : lihang
  生成日期   : 2018年12月25日
  最近修改   :
  功能描述   : keymng_dbop.c 的头文件
  函数列表   :
*
*
#ifndef __KEYMNG_DBOP_H__
  修改历史   :
  1.日    期   : 2018年12月25日
    作    者   : lihang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __KEYMNG_DBOP_H__
#define __KEYMNG_DBOP_H__
#include"keymng_shmop.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */
#define unit_4 int

/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/




/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

extern int dbGetSckeyId(void* dbhandle, int* keyid);
extern int dbWriteClient2Sql(void* dbhandle, NODESHMINFO_STRU* shmNode);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __KEYMNG_DBOP_H__ */



