#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/keymngMsg.h"
#define  MEMWATCH


/*
typedef struct _teacher
{
	char	name[64];
	int		age;
	char	*p;
	int		plen;

} Teacher;
*/


/*
// 加密
int Teacher_Encode(Teacher* pstruct, unsigned char** outData, int* dataLen)
{

	return NULL;
}

int Teacher_Decode(unsigned char* data, int dataLen, Teacher** teacherData)
{

	return NULL;
}*/


int main()
{
	// ready data
	Teacher teacher;
	strcpy(teacher.name, "yuehuanvshen");
	teacher.p = (char*)malloc(1024);
	strcpy(teacher.p, "I love you huanYueHua MeMeDa");
	teacher.age = 21;
	teacher.plen = strlen(teacher.p + 1);

	char*	outData = NULL;
	int		dataLen = 0;

	// encode data
	/*int ret = MsgEncode(&teacher, ID_MsgType_Teacher, &outData, &dataLen);
	// LOG(1, 20, "I love you huanYueHua MeMeDa %s", "yue yue")
	if (ret != 0)
	{
		LOG(1, 20, "数据有问题")
	}
	Teacher* tea = NULL;
	unsigned int type = 0;
	MsgDecode(outData, dataLen, &tea, &type);
	MsgMemFree(&tea, ID_MsgType_Teacher);
	MsgMemFree(&outData, 0);*/
	/*unsigned int type = 0;
	MsgKey_Res f_msg;
	f_msg.rv = 10;
	strcpy(f_msg.clientId, "yueyuehua");
	strcpy(f_msg.serverId, "yuehuaaiwo");
	strcpy(f_msg.r2, "huangyuehuazhedeaiwo");

	MsgKey_Res* msg = NULL;
	MsgEncode(&f_msg, ID_MsgKey_Res, &outData, &dataLen);
	MsgDecode(outData, dataLen, &msg, &type);*/

	MsgKey_Req f_msg;
	unsigned int type = 0;
	f_msg.cmdType = 1;
	strcpy(f_msg.clientId, "yueyuehua");
	strcpy(f_msg.serverId, "yuehuaaiwo");
	strcpy(f_msg.r1, "huangyuehuazhedeaiwo");
	strcpy(f_msg.AuthCode, "huangyuehua");

	MsgKey_Req* msg = NULL;
	MsgEncode(&f_msg, ID_MsgKey_Req, &outData, &dataLen);
	MsgDecode(outData, dataLen, &msg, &type);
	printf("\n make: %s", msg->clientId);
	printf("\n");

	//system("pause");

	return 0;
}
