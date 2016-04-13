#ifndef ADCREADER
#define ADCREADER
//#include <QDebug>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "gz_clk.h"
#include "gpio-sysfs.h"
#include <QThread>
 
class ADCreader : public QThread
{
public:
	ADCreader(uint8_t init1);
	uint8_t rdch;
	void quit();
	void run();
	int dat;
	
	
	int ret;
	int fd;
	int sysfs_fd;
	bool running; 
	int no_tty;
	


};

#endif
