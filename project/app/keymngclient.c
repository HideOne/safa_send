#include"stdio.h"
#include"poolsocket.h"
#include"keymngclientop.h"
#include"itcastderlog.h"
#define KeyAgree 1
#define KeyCheck 2
#define KeyDestroy 3
#define KeyLook 4
#define Quit 0


int main()
{
	int cmdNum = -1;
	int ret = 0;
	
	MngClient_Info pCltInfo;
	

	ret = MngClient_InitInfo(&pCltInfo);
	if(ret != 0) {
		LOG(4, ret, "sckClient_init err:%d", ret);
	}

	
	menuPage();	
	while(1)
	{
		
		scanf("%d", &cmdNum);
		while(getchar()!='\n'){}

		switch ( cmdNum )
		{
		    case KeyAgree:
				printf("密钥协商\n");
				MsgAgree(&pCltInfo);
		        break;

		    case KeyCheck:
				printf("密钥校验\n");
				MsgChcek(&pCltInfo);
		        break;

		    case KeyDestroy:
				printf("密钥注销\n");
				MsgDestroy(&pCltInfo);
		        break;
			case KeyLook:
				printf("密钥查看\n");
				break;

		    case Quit:
		    	return 0;
		      	break;


		    default:
		        printf("没有相应的命令\n");
		        break;
		}

	}

	return 0;
}
