#ifndef MYOBJECTPOOL3_H
#define MYOBJECTPOOL3_H
#include<list>
#include<memory>
//using namespace std;

//第三个版本，限制对象池中的对象总数量、最大空闲数量
template<typename TObject>
class ObjectPool
{
private:
	std::list<TObject*> pool;
	size_t MaxTotal;
	size_t MaxIdle;
	size_t ObjectTotal = 0;
	void release(TObject* ptr)
	{
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
	}
public:
	ObjectPool(size_t maxtotal = 4, size_t maxidle = 2)
		:MaxTotal(maxtotal), MaxIdle(maxidle)
	{}
	template<class ... Args>
	std::shared_ptr<TObject> acquire(Args&&... args)
	{
		if (ObjectTotal < MaxTotal && pool.empty())
		{
			++ObjectTotal;
			return std::shared_ptr<TObject>(new TObject(std::forward<Args>(args)...),
				[&](TObject* p) {release(p); });
		}
		else if(!pool.empty())
		{
			auto ptr = pool.front();
			pool.pop_front();
			return std::shared_ptr<TObject>(ptr, [&](TObject* p) {release(p); });
		}
		return std::shared_ptr<TObject>(nullptr);
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
