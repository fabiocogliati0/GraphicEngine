#pragma once

#include <cassert>

template <typename T>
struct RefCountable
{
	
	RefCountable()
		: mRefCount(0)
	{
	}

	void Grab()
	{
		++mRefCount;
	}

	void Release()
	{
		assert(mRefCount > 0);
		--mRefCount;

		if (mRefCount == 0)
		{
			delete (reinterpret_cast<T*>(this));
		}
	}

private:
	
	mutable unsigned mRefCount;

};