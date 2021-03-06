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
#include "Framework/RenderComponent.h"
#include "NoCullRenderQueueSystem.h"
#include "Framework/Framework.h"
#include "Framework/IInserter.h"


using namespace eastl;
using namespace BohgeEngine;
namespace NoCullRenderQueueSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	NoCullRenderQueueSystem::NoCullRenderQueueSystem(void)
		:m_pRenderMap(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	NoCullRenderQueueSystem::~NoCullRenderQueueSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::Culling( BohgeEngine::IInserter* ins )
	{
		FOR_EACH(RenderComponentMap, it, *m_pRenderMap)
		{
			if ( it->second->isRenderProperty( RenderObjectProperty::RP_SHOW ) )
			{
				ins->Pushback( it->second );
				it->second->SetVisible( true );
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::_OnRenderComponentBindChange( BohgeEngine::RenderComponent* rc )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::_OnAttachRenderComponent( BohgeEngine::RenderComponent* rc )
	{
		m_pRenderMap->insert( make_pair( rc->GetObjectID(), rc ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::_OnDetachRenderComponent( BohgeEngine::RenderComponent* rc )
	{
		m_pRenderMap->erase( rc->GetObjectID() );
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::RegistEvent( Framework& framework )
	{
		IRenderQueueSystem::RegistEvent( framework );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &NoCullRenderQueueSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &NoCullRenderQueueSystem::OnFrameworkExit ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pRenderMap = NEW RenderComponentMap;
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pRenderMap );
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::_OnInsertCullZone( const BohgeEngine::Aabbox3df& zone )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void NoCullRenderQueueSystem::_OnRemoveCullZone( const BohgeEngine::Aabbox3df& zone )
	{
	}
}