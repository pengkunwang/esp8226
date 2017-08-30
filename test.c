#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define AT_RST "AT+RST\r\n"  //重启
#define AT_CWMODE2 "AT+CWMODE=2\r\n"	//设置AP模式
#define AT_CWSAP(ssid, pw, chl) "AT+CWSAP=\""#ssid"\",\""#pw"\",\""#chl"\",3\r\n"	//设置AP名称密码
#define SHOW_AP "AT+CWSAP?\r\n"	//查询AP参数 
#define AT_CIFSR "AT+CIFSR\r\n"	//查询本地IP地址
#define AT_CIPMUX_1 "AT+CIPMUX=1\r\n"  //设置多连接类型
#define CIPSERVER(port) "AT+CIPSERVER=1,"#port"\r\n" //建立TCP服务器
#define AT_CWLIF "AT+CWLIF\r\n" //查询连接到当前AP的列表


int main()
{
	printf("%s",AT_RST);
	printf("%s",AT_CWMODE2);
	printf("%s",AT_CWSAP(peng,123456,11));
	printf("%s",SHOW_AP);
	printf("%s",AT_CIFSR);
	printf("%s",AT_CIPMUX_1);
	printf("%s",CIPSERVER(8080));
	printf("%s",AT_CWLIF);

	return 0;

}
