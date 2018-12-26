#include"stdio.h"
#include"poolsocket.h"
#include"keymngserverop.h"
#include"itcastderlog.h"
#include <pthread.h>


MngServer_Info server_info;

void *start_routine (void *arg)
{
	
	
	unit_4 ret = 0;
	unit_4 connfd = (unit_4)arg;
	unit_4 timeout = 3;
	un_char *recvData = NULL;
	un_char* outData = NULL;
	unit_4 dataLen = 0;
	unit_4 type;
	MsgKey_Req* msgKey_req = NULL;

//	printf("================有客户端连接============\n");
	printf("connfd = %d\n", connfd);

	while(1)
	{
		// 接受客户端报文
	    ret = sckServer_rev(connfd, timeout, &recvData, &dataLen);
		if(ret == Sck_ErrTimeOut) {
		    continue;
		} else if(ret == Sck_ErrPeerClosed) { // 服务器关闭
			printf("客户端断开连接\n");
			break;
		} else if (ret != 0) {
		    LOG_ERR_FUN(ret, "start_routine func err ");
			break;
		}

		// 解码客户端报文
		ret = MsgDecode(recvData, dataLen, (void**)&msgKey_req, &type);
		if(ret != 0) {
		    LOG_ERR_FUN(ret, "MsgDecode func err ");
		}

		// 根据报文cmdtype
		switch (msgKey_req->cmdType)
		{
		
		    case KeyMng_NEWorUPDATE:
		        ret = MngServer_Agree(&server_info, msgKey_req, &outData, &dataLen);
				if(ret != 0)
				{
				    LOG_ERR_FUN(ret, "MngServer_Agree func err");
				}
		        break;

		    case KeyMng_Check:
		        #
		        break;

		    case KeyMng_Revoke:
		        #
		        break;

		    default:
		        ;
		        break;
		}

		ret = sckServer_send(connfd, timeout, outData, dataLen);
		if(ret != 0)
		{
		    LOG_ERR_FUN(ret, "MngServer_Agree func err");
		}
	}

	close(connfd);
	

	return NULL;
}


int main() 
{
	unit_4 ret = 0;
	unit_4 wait_time = 3;
	unit_4 listenfd = -1;
	unit_4 connfd = -1;
	pthread_t pid = -1;
	ret =  mngServer_info(&server_info);
	if(ret != 0) {
	    LOG_ERR_FUN(ret, "mngServer_info fun is err\n");
	}

	// 初始化socket
	ret = sckServer_init(server_info.serverport, &listenfd);
	if(ret != 0) {
	    LOG_ERR_FUN(ret, "sckClient_init fun is err\n");
	}

	printf("================服务器初始化成功============\n");
	


	while(1) 
	{
	    ret = sckServer_accept(listenfd, wait_time, &connfd);
		if(ret == Sck_ErrTimeOut) {
		    
		    continue;
		} else if(ret != Sck_Ok) {
		   	LOG_ERR_FUN(ret, "sckServer_accept fun is err\n");
			return ret;
		}
		printf("================有客户端连接============\n");
		
		 ret = pthread_create(&pid, NULL, start_routine, (void*)connfd);
	}
	
	

	


	return 0;
}
