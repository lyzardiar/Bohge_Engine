//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IResourceProperty.h"
#include "IAsyncSource.h"



#include <EASTL/list.h>
#include <EASTL/string.h>

namespace BohgeEngine
{
	class ISharedResource;
	class IResourceManager;
	class BOHGE_FRAMEWORK ISharedSource : public IAsyncSource //资源的原始数据
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( ISharedSource, IAsyncSource )
	private:
		IResourceProperty::SharedSourceType				m_eResType;
		bool											m_isJustDepends;//当这个source的resource已经加载，设置这个是只加载依赖项
	public:
		ISharedSource( IResourceProperty::SharedSourceType sst, IResourceProperty::DispatchThread dt, bool hf );
		virtual ~ISharedSource();
	private:
		virtual void _DoLoadResource() = 0;//加载资源
	public:
		virtual ISharedResource* MakeResource() = 0;//将资源数据制作为实体资源，制作资源需要在主线程进行
		virtual void _DoJob( SmartPtr<IJob>& self );
	public:
		BOHGE_FORCEINLINE void SetJustDepends( )
		{
			m_isJustDepends = true;
		}
		BOHGE_FORCEINLINE bool isJustDepends()//只加载依赖项
		{
			return m_isJustDepends;
		}
		BOHGE_FORCEINLINE IResourceProperty::SharedSourceType GetGPUSourceType()
		{
			return m_eResType;
		}
	};
}




