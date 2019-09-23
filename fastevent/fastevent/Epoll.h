#ifndef __EPOLL_H__
#define __EPOLL_H__

typedef struct SChannel event;

//����epoll����sizeĬ��Ϊ1024������ʾepoll�к����������������Ϊһ���ο�
/**
.epoll_create�Ĳ�����linux2.6�ں�֮���û��ʲô�����ˣ���������ᱻ���ԣ����Ǳ���Ҫ����0��
*/
int createEPoll();
int freeEPoll(int fd);

void addEvent(int fd, event* ev);
void delEvent(int fd, event* ev);
void modifyEvent(int fd,event* ev);

struct timeval dispatchEvent(int fd,int timeout);



#endif