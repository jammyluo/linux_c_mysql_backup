#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

//待备份的数据表文件（一个数据库一行）
#define DB_FILE "./db_list"
//最多可以备份的数据库数量
#define NUM 20
//一个数据库名字的最长字符数
#define LEN 128
//保存从DB_FILE中读取到的数据库
char *db_list[NUM];
//从DB_FILE文件中读取到的数据库数量
int read_num;
//请求内存函数
void malloc_dblist();
//释放内存函数
void free_dblist();
//读取数据库文件
void readDbFile();

int main(int argc, char *argv[]) {
	pid_t pid;
	int i;
	char buf[LEN];

	//从文件读取数据库信息
	readDbFile();
	
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	
	switch (pid) {
		case -1:
			fprintf(stderr, "fork failed\n");
			exit(1);
		case 0:
			//子进程进行数据库的备份
			for (i = 0; i < read_num; i++) {
				memset(buf, '\0', LEN);
				sprintf(buf, "%s%s%s%s%s", "mysqldump -uroot ", db_list[i], " > ", db_list[i], ".sql");
				system(buf);
				printf("%d,%s\n", i, buf);
			}
			break;
	}
	//等待子进程的结束
	if (pid > 0) {
		int stat_val;
		pid_t child_pid;
		
		child_pid = wait(&stat_val);
		
		if (!WIFEXITED(stat_val)) {
			fprintf(stdout, "Child terminated abnormaly\n");
		}
		exit(1);
		
	}
	
	free_dblist();
	
	exit(0);
	
}

void malloc_dblist()
{
	int i = 0;
	//malloc for db_list
	for (i = 0; i < NUM; i++) {
		db_list[i] = malloc(LEN);
		memset(db_list[i], '\0', LEN);
	}
}
void free_dblist()
{
	int i;
	//free db_list's memory
	for (i = 0; i < NUM; i++) {
		free(db_list[i]);
	}
}

void readDbFile()
{
	FILE *fp;
	int i, j, c, isnewline;
	char *rs;
	char tmpbuf[LEN];
	
	fp = fopen(DB_FILE, "r");
	if (!fp) {
		fprintf(stderr, "%s not found\n", DB_FILE);
	}
	else {
		malloc_dblist();
	
		read_num = 1;
		j = 0;
		isnewline = 0;
		while (!feof(fp)) {
			c = fgetc(fp);
			//空字符或者字符串结束字符
			if (c == ' ' || c == '\0') {
				continue;
			}
			//回车字符
			if (c == '\r') {
				continue;
			}
			//换行字符
			if (c == '\n') {
				j = 0;
				isnewline = 1;
				continue;
			}
			//不可打印字符
			if (!isprint(c)) {
				continue;
			}
			if (isnewline) {
				read_num++;
				isnewline = 0;
			}
			db_list[read_num-1][j] = c;
			j++;
		}
		
		fclose(fp);	
	}
	
}
