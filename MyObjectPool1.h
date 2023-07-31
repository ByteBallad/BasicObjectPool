#ifndef MYOBJECTPOOL1_H
#define MYOBJECTPOOL1_H
#include<list>
#include<memory>
//using namespace std;

//第一个版本，实现基本的申请对象、归还对象
template<typename TObject>
class ObjectPool
{
private:
	std::list<TObject*> pool;
public:
	std::shared_ptr<TObject> acquire()
	{
		if (pool.empty())
		{
			return std::shared_ptr<TObject>(new TObject(),
				[&](TObject* p) {pool.push_back(p); });
		}
		else
		{
			auto ptr = pool.front();
			pool.pop_front();
			return std::shared_ptr<TObject>(ptr, [&](TObject* p) {pool.push_back(p); });
		}
	}
public:
	ObjectPool() = default;

	~ObjectPool()
	{
		clear();
	}

	ObjectPool(const ObjectPool&) = delete;

	ObjectPool& operator=(const ObjectPool&) = delete;

	size_t getIdleSize() const { return pool.size(); }

	void clear()
	{
		//清除节点指向的资源
		for (auto& pobj : pool)
		{
			delete pobj;
			pobj = nullptr;
		}
		//清除节点
		pool.clear();
	}
};
#endif
