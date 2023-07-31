#ifndef MYOBJECTPOOL4_H
#define MYOBJECTPOOL4_H
#include<list>
#include<memory>
#include<mutex>
#include<condition_variable>
//using namespace std;

//第四个版本，保证多线程下对象池的使用安全，引入互斥锁和条件变量
template<typename TObject>
class ObjectPool
{
private:
	std::list<TObject*> pool;
	size_t MaxTotal;
	size_t MaxIdle;
	size_t ObjectTotal = 0;
	std::mutex cv_mtx;
	std::condition_variable cv;

	void release(TObject* ptr)
	{
		std::unique_lock<mutex> lock(cv_mtx);
		if (pool.size() < MaxIdle)
		{
			pool.push_back(ptr);
		}
		else
		{
			delete ptr;
			--ObjectTotal;
			cout << "驱逐对象" << endl;
		}
		cv.notify_all();
	}
public:
	ObjectPool(size_t maxtotal = 4, size_t maxidle = 2)
		:MaxTotal(maxtotal), MaxIdle(maxidle)
	{}
	template<class ... Args>
	std::shared_ptr<TObject> acquire(Args&&... args)
	{
		std::unique_lock<mutex> lock(cv_mtx);
		/*while (ObjectTotal >= MaxTotal && pool.empty())
		{
			cv.wait(lock);
		}*/
		//下面这条语句，先对lambda进行判断，当lambda返回值为true时，直接返回，不会对锁进行释放操作，继续向下执行
		cv.wait(lock, [&] {return ObjectTotal < MaxTotal || !pool.empty(); });
		//如果是超时，采取相应动作
		if (std::cv_status::timeout == cv.wait_for(lock,
			std::chrono::seconds(5),
			[&] {return ObjectTotal < MaxTotal || !pool.empty(); }))
		{
			return std::shared_ptr<TObject>(nullptr);
		}
		if (!pool.empty())
		{
			auto ptr = pool.front();
			pool.pop_front();
			return std::shared_ptr<TObject>(ptr, [&](TObject* p) {release(p); });
		}
		++ObjectTotal;
		return std::shared_ptr<TObject>(new TObject(std::forward<Args>(args)...),
			[&](TObject* p) {release(p); });
	}
	~ObjectPool()
	{
		clear();
	}
	ObjectPool(const ObjectPool&) = delete;
	ObjectPool& operator=(const ObjectPool&) = delete;
	size_t getIdleSize() const { return pool.size(); }
	size_t getObjectTotal() const { return ObjectTotal; }
	size_t getActivateObjSize() const { return ObjectTotal - pool.size(); }
	void clear()
	{
		//清除节点指向的资源
		for (auto& pobj : pool)
		{
			delete pobj;
			pobj = nullptr;
		}
		ObjectTotal = 0;
		//清除节点
		pool.clear();
	}
};
#endif
