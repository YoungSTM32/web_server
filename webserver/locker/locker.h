#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <mutex>
#include <semaphore.h>
#include <exception>	
class sem{
	private:
		sem_t m_sem;
	public:
		sem(){
			if(sem_init(&m_sem,0,0)!=0){
				throw std::execption();
			}
		}
		sem(int num){
			if(sem_init(&m_sem,0,num)!=0){
				throw std::execption();
			}
		}
		~sem(){
			sem_destory(&m_sem);
		}
		bool wait(){
			return sem_wait(&m_sem)==0;
		}
		bool post(){
			return sem_post(&m_sem)==0;
		}
};

class locker{
public:
	locker(){
		if(pthread_mutex_init(&m_mutex,NULL)!=0){
			throw std::exception();
		}
	}
	~locker(){
		if(pthread_mutex_destroy(&m_mutex)){
			throw std::exception();
		}
	}
	bool lock(){
		return pthread_mutex_lock(&m_mutex)==0;
	}
	bool unlock(){
		return pthread_mutex_unlock(&m_mutex)==0;
	}
	pthread_mutex_t* get(){
		return &m_mutex;
	}

private:
pthread_mutex_t m_mutex;
};

class cond{
private:
	pthread_cond_t m_cond;
public:
	cond(){
		if(pthread_cond_init(&m_cond,NULL)){
			throw std::exception();
		}
	}

	~cond(){
		if(pthread_cond_destroy(&m_cond)==0){
			throw std::exception();
		}
	}

	bool wait(pthread_mutex_t* m_mutex){
		int ret=0;
		ret=pthread_cond_wait(&m_cond,m_mutex)
		return ret == 0;
	}
	
	bool signal(){
		
		return pthread_cond_signal(&m_cond)==0;
	}
	
	bool broadcost(){
		return pthread_cond_broadcast(&m_cond)==0;
	}
};


#endif
