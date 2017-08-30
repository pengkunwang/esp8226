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

void init_tty(int fd)
{
	struct termios new_cfg, old_cfg;
	if(tcgetattr(fd, &old_cfg) != 0)
	{
		perror("getattr err!!");
		return;
	}
	bzero(&new_cfg, sizeof(new_cfg));

	new_cfg = old_cfg;
	cfmakeraw(&new_cfg);

	cfsetispeed(&new_cfg, B115200);
	cfsetospeed(&new_cfg, B115200);

	new_cfg.c_cflag |= CLOCAL | CREAD;

	new_cfg.c_cflag &= ~CSIZE;
	new_cfg.c_cflag |= CS8;

	new_cfg.c_cflag &= ~PARENB; 
	new_cfg.c_cflag &= ~CSTOPB;

	new_cfg.c_cc[VTIME] = 0;
	new_cfg.c_cc[VMIN]  = 1;  //停止位
	tcflush(fd, TCIFLUSH);

	tcsetattr(fd, TCSANOW, &new_cfg);
}

void *recv(void *arg)
{
	int fd = *(int *)arg;
	char buff[1024];
	int n;
	int i;
	while(1)
	{	
		bzero(buff, sizeof(buff));
		n = read(fd, buff, sizeof(buff));
		if(n <= 0)
		{
			perror("read fail");
			break;
		}
		printf("%s\n", buff);
		for(i=0; i<n; i++)
		{
			printf("0X%X\t", buff[i]);
		}
		printf("\n");
	}
}

int main()
{
	char *device = "/dev/ttySAC1";
	int fd = open(device, O_RDWR|O_NOCTTY);
	if(fd == -1)
	{
		fprintf(stderr, "open %s fail:%s\n", device, strerror(errno));
	}

	init_tty(fd);

	pthread_t pd;
	pthread_create(&pd, NULL, recv, (void *)&fd);

	char buff[1024];
	int n;
	while(1)
	{
		bzero(buff, sizeof(buff));
		scanf("%s", buff);
		n = write(fd, buff, strlen(buff));
		if(n <= 0)
		{
			perror("send fail!!");
			return 0;
		}
	}
		
	return 0;
}
