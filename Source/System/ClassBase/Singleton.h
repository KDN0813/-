#pragma once
#include <mutex>
#include "assert.h"
#include "System/ClassBase/CopyDisable.h"

template<class T>
class Singleton : virtual public CopyDisable
{
protected:
	Singleton(T* const pInstance) 
	{
		assert(instance == nullptr);//インスタンスを複数生成しないように制限
		instance = pInstance;
	};
	virtual ~Singleton() { instance = nullptr; };
public:
	static T* Instance()noexcept { return instance; }//インスタンスの取得
	static const T* const CInstance()noexcept { return instance; }//constのインスタンスの取得
	static const std::mutex& GetInstanceMutex()noexcept { return mutex; }
private:
	inline static T* instance = nullptr;
	inline static std::mutex mutex;
};