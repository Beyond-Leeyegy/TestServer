/*
	FILE:utils.cpp
	
	各种通用的函数
*/

#include "def.h"
#include <stdarg.h>

/*
	利用va_list & sprintf
*/
void log(char*buf, ...) {
	va_list args;
	char tmp_main[1024];
	char tmp_time[1024];

	FILE * fp = fopen("log.txt", "a");//a代表追加写，只写，不存在就创建该文件（所有只写的模式下文件不存在就会创建——比如“w”——覆盖写）

	//step1. get multiple arguments
	va_start(args, buf);//第一个参数是va_list类型，第二个参数是函数最后显化的参数名，在本函数中就是buf
	vsprintf(tmp_main, buf, args);
	va_end(args);	    //start和end配套使用就拿到所有隐式的参数

	struct tm *current_time;
	time_t time_secs;

	time(&time_secs);
	current_time = localtime(&time_secs);

	if (!fp)
		return;

	// file open successfully
	// step3. complete the tmp_main

	sprintf(tmp_time, "[%d/%02d/%02d %02d:%02d:%02d]:%s", current_time->tm_year + 1900,
		current_time->tm_mon,
		current_time->tm_mday,
		current_time->tm_hour,
		current_time->tm_min,
		current_time->tm_sec,
		tmp_main);
	strcpy(tmp_main, tmp_time);
	fprintf(fp, tmp_main);
	
#ifndef  WIN32
	//printf(tmp_main);
#endif // ! WIN32

	
	fclose(fp);

}

/*
	LINUX/FREEBSD:没有以毫秒为单位获取CPU时间的
	win32：timeGetTime函数可以实现
	因此在linux环境下自己实现一下timeGetTime

*/
#ifndef WIN32
DWORD timeGetTime() {
	struct timeval tv;//tv_sec tv_usec
	gettimeofday(&tv, NULL);
	DWORD value = (((tv.tv_sec - 1000000000) * 1000) + (tv.tv_usec / 1000));
	return value;

}
#endif // !WIN32
