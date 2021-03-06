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
#include "ThreadSafeOprator.h"
#include "IFunctionInvoker.h"
#include "IScriptResource.h"
#include "Utility.hpp"


using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IScriptResource )
	//------------------------------------------------------------------------------------------------------
	IScriptResource::IScriptResource(void)
		:m_nHashCode(0),
		m_InstanceHandle(0),
		m_eThread(ScriptProperty::ScriptThread(-1))
	{
	}
	//------------------------------------------------------------------------------------------------------
	IScriptResource::~IScriptResource(void)
	{
		ASSERT( 0 == m_FunctionMap.size() );
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptResource::ReleaseResource()
	{
		IScriptSystem::Instance()->RecycleBin( this );
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptResource::ReleaseAll()
	{
		FOR_EACH( FuncMap, func, m_FunctionMap )
		{
			_DoReleaseFunction( m_InstanceHandle, func->second->m_pInvoker );
			SAFE_DELETE( func->second );
		}
		m_FunctionMap.clear();
		_DoRelease( m_InstanceHandle );
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptResource::SignFunciton( const eastl::string& name, uint argCount )
	{
		uint nameHash = Utility::HashCode( name );
		FuncMap::iterator funcIt = m_FunctionMap.find( nameHash );
		if ( m_FunctionMap.end() != funcIt )
		{
			ThreadSafeOprator::Increment( funcIt->second->m_User );
			return funcIt->second->m_pInvoker;
		}

		IFunctionInvoker* func = NULL;
		if ( _isFunctionExist( m_InstanceHandle, name ) )
		{
			func = _DoSignFunciton( m_InstanceHandle, name, argCount );
			func->SetFunctionID( nameHash );
			m_FunctionMap.insert( make_pair( nameHash, NEW UserFunction( func ) ));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptResource::SignFunciton( int userdata, uint argCount )
	{
		FuncMap::iterator funcIt = m_FunctionMap.find( userdata );
		if ( m_FunctionMap.end() != funcIt )
		{
			ThreadSafeOprator::Increment( funcIt->second->m_User );
			return funcIt->second->m_pInvoker;
		}
		IFunctionInvoker* func = _DoSignFunciton( m_InstanceHandle, userdata, argCount );
		func->SetFunctionID( userdata );
		m_FunctionMap.insert( make_pair( userdata, NEW UserFunction( func ) ));
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptResource::UnsignFunction( IFunctionInvoker* func )
	{
		FuncMap::iterator funcIt = m_FunctionMap.find( func->GetFunctionID() );
		ASSERT( m_FunctionMap.end() != funcIt );
		if( 0 == ThreadSafeOprator::Decrement( funcIt->second->m_User ) )
		{
			_DoReleaseFunction( m_InstanceHandle, funcIt->second->m_pInvoker );
			SAFE_DELETE( funcIt->second );
			m_FunctionMap.erase( funcIt );
		}
	}
}