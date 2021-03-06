/******************************************************************************

                  版权所有 (C), 2001-2011, Lihang

 ******************************************************************************
  文 件 名   : keymng_shmop.h
  版 本 号   : 初稿
  作    者   : lihang
  生成日期   : 2018年12月15日
  最近修改   :
  功能描述   : keymng_shmop.c 的头文件
  函数列表   :
*
*
#ifndef __KEYMNG_SHMOP_H__
  修改历史   :
  1.日    期   : 2018年12月15日
    作    者   : lihang
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifndef __KEYMNG_SHMOP_H__
#define __KEYMNG_SHMOP_H__

#define		KeyShm_MemOut			200		//共享内存已满
#define		KeyMng_TypeErr			201		//输入类型失败




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
#define  unit_4  int 

 typedef struct NODESHMINFO
 {
 	int 			status;			//密钥状态 0-有效 1无效
	char			clientId[12];	//客户端id
	char			serverId[12];	//服务器端id	
	int				seckeyid;		//对称密钥id
	unsigned char	seckey[128];	//对称密钥 //hash1 hash256 md5
 }NODESHMINFO_STRU;

extern int Mng_shm_init(int key, int maxNodeNum, int * shmhdl);
extern int Mng_shm_write(int shmdl, int maxNodeNum, NODESHMINFO_STRU* inStruct);
extern int Mng_shm_read(int shmdl, int maxNodeNum, char* clientId, char* serverId, NODESHMINFO_STRU* outStru);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __KEYMNG_SHMOP_H__ */
