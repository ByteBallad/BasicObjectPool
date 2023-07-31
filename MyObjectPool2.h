#ifndef MYOBJECTPOOL2_H
#define MYOBJECTPOOL2_H
#include<list>
#include<memory>
//using namespace std;

//第二个版本，使用可变模板参数实现带参构造函数的对象的申请、归还
template<typename TObject>
class ObjectPool
{
private:
	std::list<TObject*> pool;
public:
	template<class ... Args>
	std::shared_ptr<TObject> acquire(Args&&... args)
	{
		if (pool.empty())
		{
			return std::shared_ptr<TObject>(new TObject(std::forward<Args>(args)...),
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
