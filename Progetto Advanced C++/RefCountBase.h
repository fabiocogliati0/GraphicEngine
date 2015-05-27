/**
*
*/


#pragma once

#include <cassert>

namespace GraphicsEngine
{

	class RefCountBase
	{
	public:

		void grab() const
		{
			++mRefCount;
		}

		void release() const
		{
			assert(mRefCount > 0);
			--mRefCount;

			if (mRefCount == 0)
			{
				delete (RefCountBase *)this;
			}
		}

	protected:

		RefCountBase() : mRefCount(0)
		{
		}

		virtual ~RefCountBase()
		{
		}

	private:

		mutable int mRefCount;
	
	};
}



