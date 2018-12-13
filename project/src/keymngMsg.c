#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "keymngMsg.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"
#define  MEMWATCH


int Teacher_Encode(Teacher* pstruct, ITCAST_ANYBUF** pOutData)
{
	ITCAST_INT ret = 0;
	ITCAST_ANYBUF *pTmpString = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF *tmpbuf = NULL, *tmpOut = NULL;
	// 灏哻har* 杞寲涓篒TCAST_ANYBUF绫诲瀷
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpString, pstruct->name, strlen(pstruct->name));
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ITCAST_String_To_AnyBuf ERR");
		return ret;
	}
	

	// 杩涜TLV鎿嶄綔
	// name
	ret = DER_ItAsn1_WritePrintableString(pTmpString, &pHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmpString);
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WritePrintableString ERR");
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmpString);

	// age
	tmpbuf = pHeadBuf;
	ret = DER_ItAsn1_WriteInteger(pstruct->age, &tmpbuf->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteInteger ERR");
		return ret;
	}
	
	// p
	tmpbuf = tmpbuf->next;
	ret = EncodeChar(pstruct->p, pstruct->plen, &tmpbuf->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		ret = KeyMng_FuncErr;
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	// plen
	tmpbuf = tmpbuf->next;
	ret = DER_ItAsn1_WriteInteger(pstruct->plen, &tmpbuf->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		ret = KeyMng_FuncErr;
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &tmpOut);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		ret = KeyMng_FuncErr;
		LOG_INFO("EncodeChar ERR");
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadBuf);

	*pOutData = tmpOut;
	return ret;

}

int Teacher_Decode(ITCAST_ANYBUF* pData, Teacher** teacher)
{
	ITCAST_INT ret = 0, dataLen = 0;
	ITCAST_ANYBUF* pHead = NULL, *tmpBuf = NULL;
	ITCAST_ANYBUF* tmpData;
	Teacher* tmpTeacher = NULL;
	// 瑙ｅ嚭浜岀骇閾捐〃
	ret = DER_ItAsn1_ReadSequence(pData, &pHead);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_ReadSequence ERR");
		return ret;
	}

	// 瑙ｇ爜閾捐〃
	tmpBuf = pHead;
	
	// 瑙ｅ嚭name
	ret = DER_ItAsn1_ReadPrintableString(pHead, &tmpData);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_ReadPrintableString ERR");
		return ret;
	}
	tmpTeacher = (Teacher*)calloc(1, sizeof(Teacher));
	if (!tmpTeacher)
	{
		ret = KeyMng_MallocErr;
		DER_ITCAST_FreeQueue(pHead);
		LOG_INFO("calloc ERR");
		return ret;
	}
	strncpy(tmpTeacher->name, tmpData->pData, tmpData->dataLen);
	DER_ITCAST_FreeQueue(tmpData);

	// 瑙ge
	tmpBuf = tmpBuf->next;
	ret = DER_ItAsn1_ReadInteger(tmpBuf, (ITCAST_UINT32 *)&tmpTeacher->age);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(pHead);
		LOG_INFO("DER_ItAsn1_ReadInteger ERR");
		return ret;
	}

	// p
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &(tmpTeacher->p), &dataLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		ret = KeyMng_FuncErr;
		LOG_INFO("DecodeChar ERR");
		return ret;
	}

	// 瑙?plen
	tmpBuf = tmpBuf->next;
	ret = DER_ItAsn1_ReadInteger(tmpBuf, (ITCAST_UINT32 *)&tmpTeacher->plen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_ReadInteger ERR");
		return ret;
	}
	DER_ITCAST_FreeQueue(pHead);

	*teacher = tmpTeacher;

	return ret;
}


/*
typedef struct  _MsgKey_Res
{
	int					rv;				//杩斿洖鍊?
	char				clientId[12];	//瀹㈡埛绔紪鍙?
	char				serverId[12];	//鏈嶅姟鍣ㄧ紪鍙?
	unsigned char		r2[64];			//鏈嶅姟鍣ㄧ闅忔満鏁?
}MsgKey_Res;
*/

int MsgKey_Res_Encode(MsgKey_Res* pstruct, ITCAST_ANYBUF** pOutData)
{
	ITCAST_ANYBUF* tmpBuf = NULL, *headBuf = NULL;
	ITCAST_INT ret = 0;

	// 鎵撳寘 rv
	ret = DER_ItAsn1_WriteInteger(pstruct->rv, &headBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteInteger ERR");
		return ret;
	}

	// clientId
	tmpBuf = headBuf;
	ret = EncodeChar(pstruct->clientId, sizeof(pstruct->clientId), &(tmpBuf->next));
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	// serverId
	tmpBuf = tmpBuf->next;
	ret = EncodeChar(pstruct->serverId, sizeof(pstruct->serverId), &(tmpBuf->next));
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	// r2
	tmpBuf = tmpBuf->next;
	ret = EncodeChar(pstruct->r2, sizeof(pstruct->r2), &(tmpBuf->next));
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}
	ret = DER_ItAsn1_WriteSequence(headBuf, pOutData);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	DER_ITCAST_FreeQueue(headBuf);
	return ret;
}


/*
typedef struct  _MsgKey_Res
{
int					rv;				//杩斿洖鍊?
char				clientId[12];	//瀹㈡埛绔紪鍙?
char				serverId[12];	//鏈嶅姟鍣ㄧ紪鍙?
unsigned char		r2[64];			//鏈嶅姟鍣ㄧ闅忔満鏁?
}MsgKey_Res;
*/
MsgKey_Res_Decode(ITCAST_ANYBUF* pData, MsgKey_Res** pOutData)
{
	ITCAST_ANYBUF *tmpBuf = NULL, *headBuf = NULL;
	// ITCAST_ANYBUF* tmpData = NULL;
	ITCAST_INT ret = 0;
	MsgKey_Res* struct_Msg = NULL;
	struct_Msg = (MsgKey_Res*)calloc(1, sizeof(MsgKey_Res));
	if (struct_Msg == NULL)
	{
		ret = KeyMng_MallocErr;
		LOG_INFO("calloc ERR");
		return ret;
	}

	// 瑙ｉ摼琛?
	ret = DER_ItAsn1_ReadSequence(pData, &headBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	// 瑙v
	tmpBuf = headBuf;
	ret = DER_ItAsn1_ReadInteger(tmpBuf, (ITCAST_UINT32 *)&struct_Msg->rv);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	char* buf = NULL;
	// 瑙?char* clientId
	tmpBuf = tmpBuf->next;
	int dataLen = 0;
	ret = DecodeChar(tmpBuf, &buf, &dataLen);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DecodeChar ERR");
		return ret;
	}
	strcpy(struct_Msg->clientId, buf);
	MsgMemFree((void **)&buf, 0);

	// serverId
	tmpBuf = tmpBuf->next;
	
	ret = DecodeChar(tmpBuf, &buf, &dataLen);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DecodeChar ERR");
		return ret;
	}
	strcpy(struct_Msg->serverId, buf);
	MsgMemFree((void **)&buf, 0);

	// r2
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &buf, &dataLen);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DecodeChar ERR");
		return ret;
	}
	strcpy(struct_Msg->r2, buf);
	MsgMemFree((void **)&buf, 0);
	DER_ITCAST_FreeQueue(headBuf);
	*pOutData = struct_Msg;
	return ret;
}


/*
typedef struct _MsgKey_Req
{
	//1 瀵嗛挜鏇存柊  	//2 瀵嗛挜鏍￠獙; 	//3 瀵嗛挜娉ㄩ攢
	int				cmdType;		//鎶ユ枃鍛戒护鐮?
	char			clientId[12];	//瀹㈡埛绔紪鍙?
	char			AuthCode[16];	//璁よ瘉鐮?
	char			serverId[12];	//鏈嶅姟鍣ㄧI缂栧彿
	char			r1[64];		//瀹㈡埛绔殢鏈烘暟

}MsgKey_Req;*/

MsgKey_Req_Encode(MsgKey_Req* pData, ITCAST_ANYBUF** pOutData)
{
	// TLV cmdType
	ITCAST_INT ret = 0;
	ITCAST_ANYBUF* headBuf = NULL, *tmpBuf = NULL;
	ret = DER_ItAsn1_WriteInteger(pData->cmdType, &headBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteInteger ERR");
		return ret;
	}

	// clientId
	tmpBuf = headBuf;
	ret = EncodeChar(pData->clientId, strlen(pData->clientId), &tmpBuf->next);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	//AuthCode
	tmpBuf = tmpBuf->next;
	ret = EncodeChar(pData->AuthCode, strlen(pData->AuthCode), &tmpBuf->next);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	//serverId
	tmpBuf = tmpBuf->next;
	ret = EncodeChar(pData->serverId, strlen(pData->serverId), &tmpBuf->next);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	//r1
	tmpBuf = tmpBuf->next;
	ret = EncodeChar(pData->r1, strlen(pData->r1), &tmpBuf->next);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("EncodeChar ERR");
		return ret;
	}

	ret = DER_ItAsn1_WriteSequence(headBuf, pOutData);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	DER_ITCAST_FreeQueue(headBuf);
	return ret;
}

/*
typedef struct _MsgKey_Req
{
//1 瀵嗛挜鏇存柊  	//2 瀵嗛挜鏍￠獙; 	//3 瀵嗛挜娉ㄩ攢
int				cmdType;		//鎶ユ枃鍛戒护鐮?
char			clientId[12];	//瀹㈡埛绔紪鍙?
char			AuthCode[16];	//璁よ瘉鐮?
char			serverId[12];	//鏈嶅姟鍣ㄧI缂栧彿
char			r1[64];		//瀹㈡埛绔殢鏈烘暟

}MsgKey_Req;*/
int MsgKey_Req_Decode(ITCAST_ANYBUF* pData, MsgKey_Req** pOutData)
{
	ITCAST_INT ret = 0;
	ITCAST_ANYBUF *headBuf = NULL, *tmpBuf = NULL;
	MsgKey_Req* req = NULL;
	char* buf = NULL;
	req = (MsgKey_Req*)calloc(1, sizeof(MsgKey_Req));
	if (!req)
	{
		ret = KeyMng_MallocErr;
		LOG_INFO("calloc ERR");
		return ret;
	}

	ret = DER_ItAsn1_ReadSequence(pData, &headBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	// 瑙mdType
	tmpBuf = headBuf;
	ret = DER_ItAsn1_ReadInteger(tmpBuf, (ITCAST_UINT32 *)&req->cmdType);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}

	// 瑙lientId
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &buf, NULL);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}
	strcpy(req->clientId, buf);
	MsgMemFree((void **)&buf, 0);


	//		AuthCode[16];	//璁よ瘉鐮?
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &buf, NULL);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}
	strcpy(req->AuthCode, buf);
	MsgMemFree((void **)&buf, 0);



	//		serverId[12];	//鏈嶅姟鍣ㄧI缂栧彿
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &buf, NULL);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}
	strcpy(req->serverId, buf);
	MsgMemFree((void **)&buf, 0);
	//		r1[64];		//瀹㈡埛绔殢鏈烘暟
	tmpBuf = tmpBuf->next;
	ret = DecodeChar(tmpBuf, &buf, NULL);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(headBuf);
		LOG_INFO("DER_ItAsn1_WriteSequence ERR");
		return ret;
	}
	strcpy(req->r1, buf);
	MsgMemFree((void **)&buf, 0);

	DER_ITCAST_FreeQueue(headBuf);

	*pOutData = req;

	return ret;
}

int MsgEncode(
	void			*pStruct, /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen)
{

	ITCAST_ANYBUF *pHeadBuf = NULL, *pDataBuf = NULL;
	ITCAST_INT ret = 0;
	ret = DER_ItAsn1_WriteInteger(type, &pHeadBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		LOG_INFO("DER_ItAsn1_WriteInteger ERR");
		return ret;
	}

	switch (type)
	{
	case ID_MsgType_Teacher:
		ret = Teacher_Encode(pStruct, &(pHeadBuf->next));
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(pHeadBuf);
			LOG_INFO("Teacher_Encode ERR");
			return ret;
		}
		break;
	case ID_MsgKey_Req:
		ret = MsgKey_Req_Encode(pStruct, &(pHeadBuf->next));
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(pHeadBuf);
			LOG_INFO("MsgKey_Req_Encode ERR");
			return ret;
		}
		break;
	case ID_MsgKey_Res:
		ret = MsgKey_Res_Encode(pStruct, &(pHeadBuf->next));
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(pHeadBuf);
			LOG_INFO("Teacher_Encode ERR");
			return ret;
		}
		break;
	default:
		break;
	}


	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pDataBuf);
	if (ret != 0)
	{
		ret = KeyMng_FuncErr;
		DER_ITCAST_FreeQueue(pHeadBuf);
		LOG_INFO("DER_ItAsn1_WriteInteger ERR");
		return ret;
	}

	DER_ITCAST_FreeQueue(pHeadBuf);

	*outData = malloc(pDataBuf->dataLen);
	memset(*outData, 0, pDataBuf->dataLen);

	memcpy(*outData, pDataBuf->pData, pDataBuf->dataLen);
	*outLen = pDataBuf->dataLen;
	
	return 0;
}



int MsgDecode(
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/)
{
	ITCAST_INT ret = 0;
	ITCAST_UINT32 outType = 0;
	ITCAST_ANYBUF *tmpBuf = NULL, *tmpHeadBuf = NULL;
	Teacher* outData = NULL;
	tmpBuf = (ITCAST_ANYBUF*)malloc(sizeof(ITCAST_ANYBUF));
	if (!tmpBuf)
	{
		ret = KeyMng_MallocErr;
		return ret;

	}
	memset(tmpBuf, 0, sizeof(ITCAST_ANYBUF));

	tmpBuf->pData = (unsigned char *)malloc(inLen);
	if (!tmpBuf->pData)
	{
		free(tmpBuf);
		ret = KeyMng_MallocErr;
		return ret;

	}
	memset(tmpBuf->pData, 0, sizeof(inLen));
	memcpy(tmpBuf->pData, inData, inLen);
	tmpBuf->dataLen = inLen;

	// 瑙ｅご閮ㄧ殑type
	ret = DER_ItAsn1_ReadSequence(tmpBuf, &tmpHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(tmpBuf);
		ret = KeyMng_FuncErr;
		return ret;

	}
	DER_ITCAST_FreeQueue(tmpBuf);


	ret = DER_ItAsn1_ReadInteger(tmpHeadBuf, &outType);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(tmpHeadBuf);
		ret = KeyMng_FuncErr;
		return ret;
	}

	switch (outType)
	{
	case ID_MsgType_Teacher:
		ret = Teacher_Decode(tmpHeadBuf->next, (Teacher**)pStruct);
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(tmpHeadBuf);
		}
		break;
	case ID_MsgKey_Req:
		ret = MsgKey_Req_Decode(tmpHeadBuf->next, (MsgKey_Req**)pStruct);
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(tmpHeadBuf);
		}
		break;
	case ID_MsgKey_Res:
		ret = MsgKey_Res_Decode(tmpHeadBuf->next, (MsgKey_Res**)pStruct);
		if (ret != 0)
		{
			ret = KeyMng_FuncErr;
			DER_ITCAST_FreeQueue(tmpHeadBuf);
		}
		break;
	default:
		break;
	}
	DER_ITCAST_FreeQueue(tmpHeadBuf);
	// (Teacher*)*pStruct = outData;
	*type = outType;

	return 0;
}

void Teacher_free(Teacher** pStruct)
{
	Teacher* pTmp = *pStruct;
	if (pTmp)
	{
		if (pTmp->p)
		{
			free(pTmp->p);
		}
		free(pTmp);
		*pStruct = NULL;
	}
	
}

void MsgKey_Res_Free(MsgKey_Res** pStruct)
{
	MsgKey_Res* pTmp = *pStruct;
	if (pTmp)
	{
		free(pTmp);
		*pStruct = NULL;
	}
}

void MsgKey_Req_Free(MsgKey_Req** pStruct)
{
	MsgKey_Req* pTmp = *pStruct;
	if (pTmp)
	{
		free(pTmp);
		*pStruct = NULL;
	}
}



int MsgMemFree(void **point, int type)
{
	if (type == 0)
	{
		if (*point)
		{
			free(*point);
		}
		*point = NULL;
	}
	else
	{

		switch (type)
		{
		case ID_MsgType_Teacher:
			Teacher_free((Teacher **)point);
			break;
		case ID_MsgKey_Req:
			MsgKey_Res_Free((MsgKey_Res **)point);
			break;
		case ID_MsgKey_Res:
			MsgKey_Req_Free((MsgKey_Req **)point);
			break;
		default: 
			break;
		}
	}
	return 0;
}