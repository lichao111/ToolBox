#This is a simulator of terminal in the Electricity collection
#The cons of this simulator is that it can only simulats 1024 terminals ones time ,this is because of the select function's feature!

// 预连接请求

// 登陆
// 68 1e 00 81 05 05 00 00 00 00 00 00 a4 d9 01 01 00 01 2c 07 e3 01 18 04 0d 1c 11 00 00 1d b1 16

// 心跳
// 68 1E 00 81 05 05 00 00 00 00 00 00 A4 D9 01 01 01 01 2C 07 E3 02 15 04 13 2C 12 00 00 41 66 16
#include "netdb.h"
#include "sys/socket.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>
#include <sys/time.h>
#include <sstream>  
#include <vector>
#include <math.h>  

using namespace std;

#define PPPINITFCS16 0xffff 	/* Initial FCS value */
#define PPPGOODFCS16 0xf0b8

#define DEBUG 1
//#define DEBUG_ALL_RECV 1

typedef unsigned short u16;

typedef struct stAddr //地址域
{
	int m_nAddrType;  //服务器地址类型
	int m_nLogicAddr;	//服务器逻辑地址
	string m_strAddr;	//服务器地址
	int m_nClientAddr;	//客户机地址
}tstAddr;

typedef struct stControlDomain //控制域
{
	int m_nDIR_PRM;  //DIR_PRM
	int m_nMultiFrameFlag;	//分帧标识
	int m_nFunctionCode;	//功能码
}tstControlDomain;

//生成date_time格式时间
int AddDateTime(unsigned char *p_uchBuf,int &p_nPos,const struct timeval *p_Tv)
{
	string t_strHexDateTimes = "";
	string t_strDateTimes = "";
	struct tm t_Tm;
	localtime_r(&p_Tv->tv_sec,&t_Tm);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_year + 1900)>>8);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_year + 1900)&0xFF);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_mon + 1)&0xFF);
	p_uchBuf[p_nPos++] = t_Tm.tm_mday;
	p_uchBuf[p_nPos++] = t_Tm.tm_wday;
	p_uchBuf[p_nPos++] = t_Tm.tm_hour;
	p_uchBuf[p_nPos++] = t_Tm.tm_min;
	p_uchBuf[p_nPos++] = t_Tm.tm_sec;
	p_uchBuf[p_nPos++] = (p_Tv->tv_usec/1000)>>8;
	p_uchBuf[p_nPos++] = (p_Tv->tv_usec/1000)&0xFF;

	//t_strHexDateTimes = string(t_chTempbuf);

	//t_strDateTimes = FormatHexstrToStr(t_strHexDateTimes,0,t_strHexDateTimes.length());

	return 0;
}

int AddDateTime_s(unsigned char *p_uchBuf,int &p_nPos,const struct timeval *p_Tv)
{
	string t_strHexDateTimes = "";
	string t_strDateTimes = "";
	struct tm t_Tm;
	localtime_r(&p_Tv->tv_sec,&t_Tm);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_year + 1900)>>8);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_year + 1900)&0xFF);
	p_uchBuf[p_nPos++] = ((t_Tm.tm_mon + 1)&0xFF);
	p_uchBuf[p_nPos++] = t_Tm.tm_mday;
	//p_uchBuf[p_nPos++] = t_Tm.tm_wday;
	p_uchBuf[p_nPos++] = t_Tm.tm_hour;
	p_uchBuf[p_nPos++] = t_Tm.tm_min;
	p_uchBuf[p_nPos++] = t_Tm.tm_sec;
	// p_uchBuf[p_nPos++] = (p_Tv->tv_usec/1000)>>8;
	// p_uchBuf[p_nPos++] = (p_Tv->tv_usec/1000)&0xFF;

	//t_strHexDateTimes = string(t_chTempbuf);

	//t_strDateTimes = FormatHexstrToStr(t_strHexDateTimes,0,t_strHexDateTimes.length());

	return 0;
}

u16 pppfcs16(u16 fcs,unsigned char* cp, int len)
{
	u16 fcstab[256]={
		0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
		0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
		0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
		0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
		0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
		0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
		0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
		0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
		0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
		0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
		0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
		0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
		0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
		0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
		0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
		0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
		0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
		0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
		0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
		0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
		0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
		0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
		0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
		0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
		0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
		0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
		0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
		0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
		0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
		0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
		0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
		0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
	};

	assert(sizeof (u16) == 2);
	assert(((u16) -1) > 0);
	while (len--)
		fcs=(fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
	fcs ^= 0xFFFF;
	return (fcs);
}

string UnsignedLongToString(unsigned long p_ulValue)
{
	char t_chBuf[30];  //长度改
	sprintf(t_chBuf,"%ld",p_ulValue);
	return string(t_chBuf);
}
/************************************
【函数名称】	FormatStrToHexstr
【函数功能】	  string 转 hexString
【参数】		string content	[in] 待转换字符串
【参数】		int start	[in] 开始转换位置
【参数】		int len	[in] 需转换长度
【全局变量】	无
【返回值】	std::string  hexString
【备注】
【开发者及日期】秦力 2017/5/24
【版本】 1.0
************************************/
string FormatStrToHexstr(string content,int start,int len)
{
	string ret = "";
	for (int i= start;i<start+ len - 1;i = i+2)
	{
		int val;
		if ((content[i] >= 'A') && (content[i] <= 'F'))
		{
			val = (int)(content[i] - 'A' + 10);
		}
		else
		{
			if ((content[i] >= 'a') && (content[i] <= 'f'))
			{
				val = (int)(content[i] - 'a' + 10);
			}
			else
			{
				val =(int)(content[i] - '0');
			}
		}
		int val1;
		if ((content[i+1] >= 'A') && (content[i+1] <= 'F'))
		{
			val1 = (int)(content[i+1] - 'A' + 10);
		}
		else
		{
			if ((content[i+1] >= 'a') && (content[i+1] <= 'f'))
			{
				val1 = (int)(content[i+1] - 'a' + 10);
			}
			else
			{
				val1 =(int)(content[i+1] - '0');
			}
		}
		ret.append(1,val*16+val1);
	}
	return ret;
}

//地址域结构体转698hex
int FormatAddrTstTo6981(tstAddr p_oTstAddr,string &p_strHexAddr)
{
	int t_nRet = 0;
	int t_nNowPos = 0;
	unsigned char t_uHeadByte = 0;
	int t_nAddrLen = 0;
	string t_strHexServerAddr = "";
	char t_chHexAddr[50] = {0};
	do{


		//地址类型
		if(0 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = 0x00;
		else if(1 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = 0x40;
		else if(2 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = 0x80;
		else if(3 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = 0xC0;
		else
		{
			cout<<"非法地址类型， AddrType = "<<p_oTstAddr.m_nAddrType;
			t_nRet = 1;
			break;
		}

		//逻辑地址
		if(0 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = t_uHeadByte | 0x00;
		else if(1 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = t_uHeadByte | 0x10;
		else if(2 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = t_uHeadByte + 0x20;
		else if(3 == p_oTstAddr.m_nAddrType)
			t_uHeadByte = t_uHeadByte + 0x30;
		else
		{
			cout<<"非法逻辑地址， LogicAddr = "<<p_oTstAddr.m_nLogicAddr;
			t_nRet = 2;
			break;
		}

		//地址长度
		t_nAddrLen  = p_oTstAddr.m_strAddr.length()/2 - 1; //一个字节占2个显示位,0表示长度为1
		if(t_nAddrLen > 16)
		{
			cout<<"非法地址长度， AddrLen = "<<t_nAddrLen;
			t_nRet = 3;
			break;
		}else
		{
			t_uHeadByte = t_uHeadByte + (t_nAddrLen & 0xFF);
		}
		//地址域首字节
		//sprintf(&t_chHexAddr[t_nNowPos++],"%02X",t_uHeadByte);
		t_chHexAddr[t_nNowPos++] = t_uHeadByte & 0xFF;

		//服务器地址
		t_strHexServerAddr=FormatStrToHexstr(p_oTstAddr.m_strAddr,0,p_oTstAddr.m_strAddr.length());
		reverse(t_strHexServerAddr.begin(), t_strHexServerAddr.end());
		for (unsigned int i = 0;i<t_strHexServerAddr.length();i++)
		{
			t_chHexAddr[t_nNowPos++] = t_strHexServerAddr[i];
		}

		//客户机地址
		t_chHexAddr[t_nNowPos++] = p_oTstAddr.m_nClientAddr & 0xFF;


		for (int i=0;i<t_nNowPos;i++)
		{
			p_strHexAddr += t_chHexAddr[i];
		}

	}while(false);
	return t_nRet;
}

int GetControlDomain(tstControlDomain P_oTstControlDomain,int &p_nControl)
{
	int t_nRet = 0;
	int t_nControlDomain = -1;
	//string t_strControlDomain = "";

	t_nControlDomain = P_oTstControlDomain.m_nDIR_PRM + \
		P_oTstControlDomain.m_nMultiFrameFlag * 32 + P_oTstControlDomain.m_nFunctionCode;
	p_nControl = t_nControlDomain;
	return t_nRet;
}

int GetPIIDNum(int p_nSeed)
{
	srand(time(NULL));
	return  rand() % 64;
}

int GenerateCRC16(unsigned char* p_chMsg, int p_nLen, unsigned short &p_usHexCRC16)
{
	int t_nRet = 0;
	p_usHexCRC16 =  pppfcs16(PPPINITFCS16,p_chMsg,p_nLen);
	return t_nRet;
}



int CreateLinkRequestFrame(int p_nClientAddr,int p_nFrameType,int p_nHbCycle,string &p_strHexFrame)
{
	int t_nRet = 0;
	int t_nNowPos = 0; //报文当前位置
	int t_nFrameHead = 0; //报文头长度位置
	unsigned short t_usCRC16; //校验码
	int t_nControl = 0;//控制域值
    ostringstream os;
    os<<p_nClientAddr;
	string  t_strASN = os.str();
	string t_strHexASN = "";

	string t_strHexAddr = "";//地址域

	unsigned char t_chFrame[512] = {0};

	t_chFrame[t_nNowPos++] = 0x68;
	t_chFrame[t_nNowPos++] = 0x00;//L
	t_chFrame[t_nNowPos++] = 0x00;//L

	//控制域
	tstControlDomain t_oTstControlDomain;
	t_oTstControlDomain.m_nDIR_PRM = 0x80;
	t_oTstControlDomain.m_nMultiFrameFlag = 0x00;
	t_oTstControlDomain.m_nFunctionCode = 0x01;
	GetControlDomain(t_oTstControlDomain,t_nControl);
	t_chFrame[t_nNowPos++] = t_nControl & 0xFF;

	//地址域
	tstAddr t_oTstAddr;
	t_oTstAddr.m_nAddrType = 0;		//单地址
	t_oTstAddr.m_nLogicAddr = 0;	//逻辑地址
	//t_oTstAddr.m_strAddr = 000000000015;    //服务器地址

	int t_nLen = t_strASN.length();
	for(int i = 1;i<=12-t_nLen;i++)
	{
		t_strASN = "0" + t_strASN;
	}
	t_oTstAddr.m_strAddr = t_strASN;//服务器地址

	t_oTstAddr.m_nClientAddr = 0;//客户机地址
	FormatAddrTstTo6981(t_oTstAddr,t_strHexAddr);

	int t_nStrLenTmp = 0;
	t_nStrLenTmp = t_strHexAddr.length();
	for (int i=0;i<t_nStrLenTmp;i++)
	{
		t_chFrame[t_nNowPos++] = t_strHexAddr[i];
	}
	t_nFrameHead = t_nNowPos;

	t_chFrame[t_nNowPos++] = 0x00; //HCS
	t_chFrame[t_nNowPos++] = 0x00; //HCS

	//APDU
	t_chFrame[t_nNowPos++] = 0x01;//LINK-Request
	t_chFrame[t_nNowPos++] = GetPIIDNum(0) & 0xFF;//PIID-ACD
	t_chFrame[t_nNowPos++] = p_nFrameType & 0xFF;//请求类型 0:建立连接,1:心跳,2:断开连接

	t_chFrame[t_nNowPos++] = (p_nHbCycle>>8)& 0xFF;//心跳周期  单位：秒
	t_chFrame[t_nNowPos++] = p_nHbCycle & 0xFF;//心跳周期


	struct timeval tv;
	gettimeofday(&tv,NULL);
	AddDateTime(t_chFrame,t_nNowPos,&tv);//请求时间

	//修改长度
	t_chFrame[1] = (t_nNowPos + 1)&0xFF;
	t_chFrame[2] = ((t_nNowPos + 1)>>8)&0xFF;

	//修改HCS
	GenerateCRC16(&t_chFrame[1],t_nFrameHead-1,t_usCRC16);
	t_chFrame[t_nFrameHead] = t_usCRC16%256;
	t_chFrame[t_nFrameHead+1] = t_usCRC16/256;

	//计算FCS
	GenerateCRC16(&t_chFrame[1],t_nNowPos-1,t_usCRC16);
	t_chFrame[t_nNowPos++] = t_usCRC16%256;; //FCS
	t_chFrame[t_nNowPos++] = t_usCRC16/256; //FCS
	t_chFrame[t_nNowPos++] = 0x16;

	for (int i=0;i<t_nNowPos;i++)
	{
		p_strHexFrame += t_chFrame[i];
	}


	// //添加44字节报文头
	// tstInternalProtocol t_oTstInternalProtocol;
	// t_oTstInternalProtocol.m_ulMN = 305419896;
	// t_oTstInternalProtocol.m_bCON = false;
	// t_oTstInternalProtocol.m_nProtocolType = 1;
	// t_oTstInternalProtocol.m_nTaskType = 1;
	// t_oTstInternalProtocol.m_strSA = t_oTstAddr.m_strAddr;
	// t_oTstInternalProtocol.m_uchOverDT = 0;
	// t_oTstInternalProtocol.m_uchSendNum = 0;
	// t_oTstInternalProtocol.m_bStoreFlag = false;//是否入库
	// t_oTstInternalProtocol.m_ulASN = ReadConfig::m_nASN; //本解析服务的编号，从配置文件中读取
	// t_oTstInternalProtocol.m_ulSeq = 1;
	// t_oTstInternalProtocol.m_ulTSN = 0;

	//string t_tmp = "";
	//AddInternalProtocol(t_oTstInternalProtocol,p_strHexFrame,t_tmp);

	//p_strHexFrame = t_tmp;


	return t_nRet;
}

int CreateOneTerminal(const char *ip,const char *port,int type = 0)
{
    /*步骤1：创建socket*/
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(atoi(port));

    //主机字节序转换成网络字节序
    inet_pton(AF_INET,ip,
            &serveraddr.sin_addr.s_addr);

    /*步骤2:客户端调用connect函数连接到服务器

    */
    if(connect(sockfd,(struct sockaddr*)&serveraddr,
                sizeof(serveraddr))<0)
    {
        perror("connect error");
        exit(1);
    }

    /*步骤3：调用IO函数(read/write)和服务器端双向通信*/
    unsigned char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    size_t size;

    string t_strsend = "";
   if(type!=0) 
    CreateLinkRequestFrame(sockfd+1,0,60,t_strsend);
    else
    CreateLinkRequestFrame(sockfd,0,60,t_strsend);
   // cout<<t_strsend<<endl;
    memcpy(buffer,t_strsend.c_str(),t_strsend.size());
    size = t_strsend.size();
    if(write(sockfd,buffer,size)!=size)
    {
        perror("write error");
    }
	printf("登陆：");
	for(int i = 0;i<t_strsend.size();i++)
	{
		printf("%02X ",buffer[i]);
	}
	printf("\n");
    memset(buffer,0,sizeof(buffer));
    if((size=read(sockfd,
                    buffer,sizeof(buffer)))<0)
    {
        perror("read error");
    }
    else
    {
        //#ifdef DEBUG_BEAT
         printf("登陆响应： ");
         for(int i =0;i<size;i++)
             printf("%02X ",buffer[i]);
         printf("\n");
        //#endif
    }

    return sockfd;
}

int HeadBeat(int sockfd,int type = 0)
{
    unsigned char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    size_t size;
    string t_strsend = "";
    if(type!=0) 
    CreateLinkRequestFrame(sockfd+1,1,60,t_strsend);
    else
    CreateLinkRequestFrame(sockfd,1,60,t_strsend);
   // cout<<t_strsend<<endl;
    memcpy(buffer,t_strsend.c_str(),t_strsend.size());
    size = t_strsend.size();
    if(write(sockfd,buffer,size)!=size)
    {
        perror("write error");
    }
	printf("心跳：");
	for(int i = 0;i<t_strsend.size();i++)
	{
		printf("%02X ",buffer[i]);
	}
	printf("\n");
    memset(buffer,0,sizeof(buffer));
    if((size=read(sockfd,
                    buffer,sizeof(buffer)))<0)
    {
        perror("read error");
    }
    else
    {
		printf("心跳响应：");
		for(int i = 0;i<t_strsend.size();i++)
		{
			printf("%02X ",buffer[i]);
		}
		printf("\n");
    }
}





//回复安全模式参数

//Read
//68 17 00 43 05 05 00 00 00 00 00 0a 8b 26 05 01 3a f1 01 02 00 00 88 f1 16

//Reply
//68 21 00 c3 05 05 00 00 00 00 00 0a e4 84 85 01 2b f1 01 02 00 00 00 00 ba 70 16

int reply_security(int fd,unsigned char *buf,int len)
{
	int ret = -1;
	// unsigned char read[1024];
	// memset(read,0,1024);
	// sprintf(read,"%s",buf);
	string Read="";
	char tmp[1024];
	memset(tmp,0,1024);
	printf("Recv: ");
	for(int i=0;i<len;i++)
	{
		printf("%02x ",buf[i]);
		sprintf(tmp+2*i,"%02x",(char *)buf[i]);
	}
    printf("\n");
	Read += tmp;
	size_t Read_Flag = Read.find("f1010200");
	if(Read_Flag==string::npos)
	{
		//printf("not read terminal clock frame!!\n");
		return -1;
	}
	else
	{
		/*if(Read.substr(Read_Flag-6,4)!="0501")
		{
			return -1;
		}
		*/
	}
	//构造读取时钟响应报文
	int t_nRet = 0;
	int t_nNowPos = 0; //报文当前位置
	int t_nFrameHead = 0; //报文头长度位置
	unsigned short t_usCRC16; //校验码
	int t_nControl = 0;//控制域值
	string t_strHexAddr = "";//地址域
	
	unsigned char t_chFrame[512] = {0};

	t_chFrame[t_nNowPos++] = 0x68;
	t_chFrame[t_nNowPos++] = 0x00;//L
	t_chFrame[t_nNowPos++] = 0x00;//L

	//控制域
	tstControlDomain t_oTstControlDomain;
	t_oTstControlDomain.m_nDIR_PRM = 0xc0;
	t_oTstControlDomain.m_nMultiFrameFlag = 0x00;
	t_oTstControlDomain.m_nFunctionCode = 0x3;
	GetControlDomain(t_oTstControlDomain,t_nControl);
	t_chFrame[t_nNowPos++] = t_nControl & 0xFF;

	//地址域
	tstAddr t_oTstAddr;
	t_oTstAddr.m_nAddrType = 0;		//单地址
	t_oTstAddr.m_nLogicAddr = 0;	//逻辑地址
	//t_oTstAddr.m_strAddr = 000000000015;    //服务器地址

	for(int i = 5;i>=0;i--)
	{
		t_oTstAddr.m_strAddr += Read.substr(10+2*i,2);//服务器地址
	}
	t_oTstAddr.m_nClientAddr = atoi(Read.substr(22,2).c_str());//客户机地址
	FormatAddrTstTo6981(t_oTstAddr,t_strHexAddr);

	int t_nStrLenTmp = 0;
	t_nStrLenTmp = t_strHexAddr.length();
	for (int i=0;i<t_nStrLenTmp;i++)
	{
		t_chFrame[t_nNowPos++] = t_strHexAddr[i];
	}
	t_nFrameHead = t_nNowPos;

	t_chFrame[t_nNowPos++] = 0x00; //HCS
	t_chFrame[t_nNowPos++] = 0x00; //HCS

	//APDU
	t_chFrame[t_nNowPos++] = 0x85;// read respone
	t_chFrame[t_nNowPos++] = 0x01;
	//t_chFrame[t_nNowPos++] = GetPIIDNum(0) & 0xFF;//PIID-ACD
	//t_chFrame[t_nNowPos++] = (atoi(Read.substr(Read_Flag-2,2).c_str()))&0xFF;//FIX: the respone PIID is equal to the request PIID
	int PIID = 0;
	
	const char *PIID_RECV  = Read.substr(Read_Flag-2,2).c_str();
	
	/***********conver for visible hex to dec*********************/
	if(PIID_RECV[0]>='a')
	{
		PIID +=( (PIID_RECV[0] - 'a' +10) * pow(16,1) );
	}
	else{
		if(PIID_RECV[0]>='A')
		{
			PIID +=( (PIID_RECV[0] - 'A' + 10) * pow(16,1) );
		}
		else
		{
			PIID += ((PIID_RECV[0] - '0') * pow(16,1));
		}
	}
	
	if(PIID_RECV[1]>='a')
	{
		PIID +=( (PIID_RECV[1] - 'a' + 10) * pow(16,0) );
	}
	else{
		if(PIID_RECV[1]>='A')
		{
			PIID +=( (PIID_RECV[1] - 'A' + 10) * pow(16,0) );
		}
		else
		{
			PIID +=( (PIID_RECV[1] - '0') * pow(16,0) );
		}
	}
	
	t_chFrame[t_nNowPos++] = PIID;
    t_chFrame[t_nNowPos++] = 0xf1;//
	t_chFrame[t_nNowPos++] = 0x01;
	t_chFrame[t_nNowPos++] = 0x02;
	t_chFrame[t_nNowPos++] = 0x00;
	t_chFrame[t_nNowPos++] = 0x01;
	t_chFrame[t_nNowPos++] = 0x16;
	t_chFrame[t_nNowPos++] = 0x00;
//不开启安全模式

	t_chFrame[t_nNowPos++] = 0x00;//无跟随上报信息域
	t_chFrame[t_nNowPos++] = 0x00;//无时间标签

	//修改长度
	t_chFrame[1] = (t_nNowPos + 1)&0xFF;
	t_chFrame[2] = ((t_nNowPos + 1)>>8)&0xFF;

	//修改HCS
	GenerateCRC16(&t_chFrame[1],t_nFrameHead-1,t_usCRC16);
	t_chFrame[t_nFrameHead] = t_usCRC16%256;
	t_chFrame[t_nFrameHead+1] = t_usCRC16/256;


	//计算FCS
	GenerateCRC16(&t_chFrame[1],t_nNowPos-1,t_usCRC16);

	t_chFrame[t_nNowPos++] = t_usCRC16%256;; //FCS
	t_chFrame[t_nNowPos++] = t_usCRC16/256; //FCS
	t_chFrame[t_nNowPos++] = 0x16;

	//t_chFrame[t_nNowPos++] = 0x16;

	string p_strHexFrame = "";

    #ifdef DEBUG
    printf("Send: ");
    #endif 
	for (int i=0;i<t_nNowPos;i++)
	{
        #ifdef DEBUG
		printf("%02x ",t_chFrame[i]);
        #endif
		p_strHexFrame += t_chFrame[i];
	}
    #ifdef DEBUG
    printf("\n");
    #endif
	write(fd,p_strHexFrame.c_str(),p_strHexFrame.size());
	return ret;
}


//回复终端时钟

//Read
//68 17 00 43 05 05 00 00 00 00 00 0a 8b 26 05 01 3a 40 00 02 00 00 88 f1 16

//Reply
//68 21 00 c3 05 05 00 00 00 00 00 0a e4 84 85 01 2b 40 00 02 00 01 1c 07 e3 02 18 12 00 1f 00 00 ba 70 16
int reply_clock(int fd,unsigned char *buf,int len)
{
	int ret = -1;
	// unsigned char read[1024];
	// memset(read,0,1024);
	// sprintf(read,"%s",buf);
	string Read="";
	char tmp[1024];
	memset(tmp,0,1024);
	printf("Recv: ");
	for(int i=0;i<len;i++)
	{
		printf("%02x ",buf[i]);
		sprintf(tmp+2*i,"%02x",(char *)buf[i]);
	}
    printf("\n");
	Read += tmp;
	size_t Read_Flag = Read.find("40000200");
	if(Read_Flag==string::npos)
	{
		//printf("not read terminal clock frame!!\n");
		return -1;
	}
	else
	{
		if(Read.substr(Read_Flag-6,4)!="0501")
		{
			return -1;
		}
	}
	//构造读取时钟响应报文
	int t_nRet = 0;
	int t_nNowPos = 0; //报文当前位置
	int t_nFrameHead = 0; //报文头长度位置
	unsigned short t_usCRC16; //校验码
	int t_nControl = 0;//控制域值
	string t_strHexAddr = "";//地址域
	

	unsigned char t_chFrame[512] = {0};

	t_chFrame[t_nNowPos++] = 0x68;
	t_chFrame[t_nNowPos++] = 0x00;//L
	t_chFrame[t_nNowPos++] = 0x00;//L

	//控制域
	tstControlDomain t_oTstControlDomain;
	t_oTstControlDomain.m_nDIR_PRM = 0xc0;
	t_oTstControlDomain.m_nMultiFrameFlag = 0x00;
	t_oTstControlDomain.m_nFunctionCode = 0x3;
	GetControlDomain(t_oTstControlDomain,t_nControl);
	t_chFrame[t_nNowPos++] = t_nControl & 0xFF;

	//地址域
	tstAddr t_oTstAddr;
	t_oTstAddr.m_nAddrType = 0;		//单地址
	t_oTstAddr.m_nLogicAddr = 0;	//逻辑地址
	//t_oTstAddr.m_strAddr = 000000000015;    //服务器地址

	for(int i = 5;i>=0;i--)
	{
		t_oTstAddr.m_strAddr += Read.substr(10+2*i,2);//服务器地址
	}
	t_oTstAddr.m_nClientAddr = atoi(Read.substr(22,2).c_str());//客户机地址
	FormatAddrTstTo6981(t_oTstAddr,t_strHexAddr);

	int t_nStrLenTmp = 0;
	t_nStrLenTmp = t_strHexAddr.length();
	for (int i=0;i<t_nStrLenTmp;i++)
	{
		t_chFrame[t_nNowPos++] = t_strHexAddr[i];
	}
	t_nFrameHead = t_nNowPos;

	t_chFrame[t_nNowPos++] = 0x00; //HCS
	t_chFrame[t_nNowPos++] = 0x00; //HCS

	//APDU
	t_chFrame[t_nNowPos++] = 0x85;// read respone
	t_chFrame[t_nNowPos++] = 0x01;
	//t_chFrame[t_nNowPos++] = GetPIIDNum(0) & 0xFF;//PIID-ACD
	//t_chFrame[t_nNowPos++] = (atoi(Read.substr(Read_Flag-2,2).c_str()))&0xFF;//FIX: the respone PIID is equal to the request PIID
	int PIID = 0;
	
	const char *PIID_RECV  = Read.substr(Read_Flag-2,2).c_str();
	
	/***********conver for visible hex to dec*********************/
	if(PIID_RECV[0]>='a')
	{
		PIID +=( (PIID_RECV[0] - 'a' +10) * pow(16,1) );
	}
	else{
		if(PIID_RECV[0]>='A')
		{
			PIID +=( (PIID_RECV[0] - 'A' + 10) * pow(16,1) );
		}
		else
		{
			PIID += ((PIID_RECV[0] - '0') * pow(16,1));
		}
	}
	
	if(PIID_RECV[1]>='a')
	{
		PIID +=( (PIID_RECV[1] - 'a' + 10) * pow(16,0) );
	}
	else{
		if(PIID_RECV[1]>='A')
		{
			PIID +=( (PIID_RECV[1] - 'A' + 10) * pow(16,0) );
		}
		else
		{
			PIID +=( (PIID_RECV[1] - '0') * pow(16,0) );
		}
	}
	
	t_chFrame[t_nNowPos++] = PIID;
    t_chFrame[t_nNowPos++] = 0x40;//
	t_chFrame[t_nNowPos++] = 0x00;
	t_chFrame[t_nNowPos++] = 0x02;
	t_chFrame[t_nNowPos++] = 0x00;
	t_chFrame[t_nNowPos++] = 0x01;
	t_chFrame[t_nNowPos++] = 0x1c;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	AddDateTime_s(t_chFrame,t_nNowPos,&tv);//终端时间

	t_chFrame[t_nNowPos++] = 0x00;//无跟随上报信息域
	t_chFrame[t_nNowPos++] = 0x00;//无时间标签

	//修改长度
	t_chFrame[1] = (t_nNowPos + 1)&0xFF;
	t_chFrame[2] = ((t_nNowPos + 1)>>8)&0xFF;

	//修改HCS
	GenerateCRC16(&t_chFrame[1],t_nFrameHead-1,t_usCRC16);
	t_chFrame[t_nFrameHead] = t_usCRC16%256;
	t_chFrame[t_nFrameHead+1] = t_usCRC16/256;


	//计算FCS
	GenerateCRC16(&t_chFrame[1],t_nNowPos-1,t_usCRC16);

	t_chFrame[t_nNowPos++] = t_usCRC16%256;; //FCS
	t_chFrame[t_nNowPos++] = t_usCRC16/256; //FCS
	t_chFrame[t_nNowPos++] = 0x16;

	//t_chFrame[t_nNowPos++] = 0x16;

	string p_strHexFrame = "";

    #ifdef DEBUG
    printf("Send: ");
    #endif 
	for (int i=0;i<t_nNowPos;i++)
	{
        #ifdef DEBUG
		printf("%02x ",t_chFrame[i]);
        #endif
		p_strHexFrame += t_chFrame[i];
	}
    #ifdef DEBUG
    printf("\n");
    #endif
	write(fd,p_strHexFrame.c_str(),p_strHexFrame.size());
	return ret;
}

int reply_station(int fd,unsigned char *buf,int len)
{
	int ret = -1;
	// unsigned char read[1024];
	// memset(read,0,1024);
	// sprintf(read,"%s",buf);
	string Read="";
	char tmp[1024];
	memset(tmp,0,1024);
	printf("Recv: ");
	for(int i=0;i<len;i++)
	{
		printf("%02x ",buf[i]);
		sprintf(tmp+2*i,"%02x",(char *)buf[i]);
	}
    printf("\n");
	Read += tmp;
	size_t Read_Flag = Read.find("40000200");
	if(Read_Flag!=string::npos)
	{
		reply_clock(fd,buf,len);
	}
	
	Read_Flag = Read.find("f1010200");
	if(Read_Flag!=string::npos)
	{
		reply_security(fd,buf,len);
	}
	return 0;
}

int main(int argc,char *argv[])
{
    if(argc<3)
    {
        printf("usage:%s ip port TerminalNumber \n",argv[0]);
        exit(1);
    }
    
    int t_nTerminalNum = atoi(argv[3]);
    vector<int> Terminal;
    Terminal.clear();

    const char *ip = argv[1];
    const char *port = argv[2];

    fd_set  rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    //登陆
    int fd=0;
    for(int i = 0;i<t_nTerminalNum;i++)
    {
        fd = CreateOneTerminal(ip,port);
        Terminal.push_back( fd );

    }

    time_t start = time(NULL);
    int k = 0;

/***************************************************/
    bool Onlin = false;//模拟断开之后重连
    bool SendClock = true;//模拟抄终端时钟
    bool report = false;//上报终端时钟，目前是上报心跳
/***************************************************/

	//心跳
     while(1)
    {

        if(Onlin)
        {
            sleep(10);
            if(k%2 == 0 )//全部心跳
            {
                if(!Onlin)
                {
                    Terminal.push_back( CreateOneTerminal(ip,port) );

                }
                for(int i =0;i<t_nTerminalNum;i++)
                {
                    HeadBeat(Terminal[i]);
                }
            }
            else
            {//第一个终端不心跳，断开
                close(Terminal[0]);
                Terminal.erase(Terminal.begin());
                Onlin = false;
                for(int i =1;i<t_nTerminalNum;i++)
                {
                 HeadBeat(Terminal[i]);
                }
            }
            k++;
            // getchar();
            // close(Terminal[0]);
            // Terminal.erase(Terminal.begin());
            // Terminal.push_back(CreateOneTerminal(ip,port,1) );
            // HeadBeat(Terminal[0],1);
        }
        if(SendClock)
        {
            while(1)
            {
            	unsigned char buf1[1024];
            	memset(buf1,0,sizeof(buf1));
				for(int i=0;i<t_nTerminalNum;i++)
				{
					FD_SET(Terminal[i],&rset);//只监听读事件
				}
            	int ret = select(fd+1,&rset,&wset,NULL,&tv);
            	if(ret==-1)
            	{
                	printf("select error\n");
					sleep(1);
					continue;
            	}

            	if(ret!=0)
            	{
					for(int i = 0;i<t_nTerminalNum;i++)
            		{
                		if(FD_ISSET(Terminal[i],&rset))
                		{
							int len = recv(Terminal[i],buf1,1024,0);
							if(len<=0)
                        	{
								perror("recv() error!\n");
								if(len==0)
								{
									printf("close Terminal[i]\n");
									close(Terminal[i]);
									Terminal.erase(Terminal.begin()+i);
								}
								continue;
								
							}
                            #ifdef DEBUG_ALL_RECV
                            printf("Recv: ");
                            for(int j = 0;j<len;j++)
                                printf("%02x ",buf1[j]);
                            printf("\n");
                            #endif
							reply_station(Terminal[i],buf1,len);//回复时钟
               			}
            		}
            	}

				if((time(NULL)-start)>=300)//需要心跳
				{
					start = time(NULL);
					for(int i=0;i<t_nTerminalNum;i++)
					{
                 		HeadBeat(Terminal[i]);
					}
				}
            }
        }
        if(report)
        {
            for(int i =0;i<t_nTerminalNum;i++)
            {
                HeadBeat(Terminal[i]);
            }
            sleep(1);
        }

        
    }

    return -1;
}
