#ifndef CHAR_BUFFER_IOCTL_H
#define CHAR_BUFFER_IOCTL_H

#include <linux/ioctl.h>

#define CHAR_BUFFER_IOC_MAGIC 'c'
#define CHAR_BUFFER_CLEAR _IO(CHAR_BUFFER_IOC_MAGIC, 1)
#define CHAR_BUFFER_GET_SIZE _IOR(CHAR_BUFFER_IOC_MAGIC, 2, unsigned int)

#endif
