#include<sys/epoll.h>
#include<sys/time.h>
#include<error.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>


#include "Epoll.h"
#include "Logger.h"
#include "Channel.h"
#include "Config.h"

int createEPoll(){
	
	//�����׽��֣����׽������������������е�epoll������
	int epollFd = epoll_create(1024);
	if (epollFd == -1){
		debug_ret("����Epoll�׽���ʧ�ܣ�file : %s, line : %d", __FILE__, __LINE__);
		return -1;
	}
	return epollFd;
}

int freeEPoll(int fd){
	if (fd >= 0)
	{
		close(fd);
	}
}

void addEvent(int epoll_fd, event* e)
{
	struct epoll_event ev;
	ev.events = e->eventFlag;
	ev.data.ptr = e;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e->fd, &ev) == -1)  {
		debug_sys("epoll_ctl add failed, file : %s, line : %d", __FILE__, __LINE__);
	}
}
void delEvent(int epoll_fd, event* e){
	struct epoll_event ev;
	ev.events = e->eventFlag;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e->fd, &ev) == -1)  {
		debug_sys("epoll_ctl del failed, file : %s, line : %d", __FILE__, __LINE__);
	}
}
//modifyҪ��ɾ��֮������ӿ�һ�㣬��Ҳ��muduo������һ���Ż�֮һ
void modifyEvent(int epoll_fd, event* e){
	struct epoll_event ev;
	ev.events = e->eventFlag;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e->fd, &ev) == -1)  {
		debug_sys("epoll_ctl del failed, file : %s, line : %d", __FILE__, __LINE__);
	}
}

struct timeval dispatchEvent(int epoll_fd, int timeout){
	struct epoll_event events[MAX_EVENTS];
	int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);

	if (nfds == -1)  {
		if (errno != EINTR)  {
			debug_sys("epoll_wait failed, file : %s, line : %d", __FILE__, __LINE__);
		}
	}

	struct timeval now;
	gettimeofday(&now, NULL);

	int i;
	event* ev;
	for (i = 0; i < nfds; i++)  {
		ev = (event*)events[i].data.ptr;
		//��ǰ������ʱ��
		ev->time = now;
		ev->active_event = events[i].events;
		eventHandler(ev);
	}

	struct timeval end;
	gettimeofday(&end, NULL);
	return end;

}


