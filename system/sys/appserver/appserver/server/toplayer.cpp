/*
 *  The Syllable application server
 *	Copyright (C) 2005 - Syllable Team
 *  Copyright (C) 1999 - 2001 Kurt Skauen
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU Library
 *  General Public License as published by the Free Software
 *  Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 

#include <gui/window.h>
#include "toplayer.h"
#include "ddriver.h"
#include "bitmap.h"
#include "sprite.h"

using namespace os;

TopLayer::TopLayer( SrvBitmap* pcBitmap ) : Layer( pcBitmap )
{
}



/** Reallocates the backbuffer of a layer.
 * \par Description:
 * Called by layers which are attached to the top layer (the window borders
 * and the window selector) when their frame has changed. The method will reallocate
 * the backbuffer if necessary.
 * \param pcChild - Pointer to the layer.
 * \param cFrame - New frame of the layer.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::LayerFrameChanged( Layer* pcChild, IRect cFrame )
{
	SrvBitmap* pcBackbuffer = NULL;
	
	if( pcChild->GetWindow() != NULL && ( pcChild->GetWindow()->GetFlags() & WND_SINGLEBUFFER ) )
		return;
	
	assert( m_pcBitmap != NULL );
	
	/* Do not reallocate the backbuffer if there is already one with a matching size */
	if( pcChild->m_pcBackbuffer != NULL  && cFrame.IsValid() )
		if( abs( cFrame.Width() -  ( pcChild->m_pcBackbuffer->m_nWidth - 11 ) ) < 10
			&& abs( cFrame.Height() - ( pcChild->m_pcBackbuffer->m_nHeight - 11 ) ) < 10 &&
			m_pcBitmap->m_eColorSpc == pcChild->m_pcBackbuffer->m_eColorSpc )
	{
		pcChild->SetBitmap( pcChild->m_pcBackbuffer );
		return;
	}


	if( cFrame.IsValid() )
	{
		/* Allocate the new backbuffer */
		pcBackbuffer = m_pcBitmap->m_pcDriver->AllocateBitmap( cFrame.Width() + 11, cFrame.Height() + 11, 
													m_pcBitmap->m_eColorSpc );
		if( pcBackbuffer != NULL )
		{
			pcBackbuffer->m_pcDriver = m_pcBitmap->m_pcDriver;
			assert( pcBackbuffer->m_pRaster != NULL );
			//dbprintf( "Reallocated backbuffer %i %i\n", cFrame.Width() + 1, cFrame.Height() + 1 );
		}
	}
	if( pcChild->m_pcBackbuffer != NULL )
	{
		/* Copy and release the old one */
		SrvBitmap* pcOldBuffer = pcChild->m_pcBackbuffer;
		if( pcBackbuffer != NULL )
		{
			IRect cBlitRect = IRect( 0, 0, pcOldBuffer->m_nWidth - 1, pcOldBuffer->m_nHeight - 1 )
						& IRect( 0, 0, pcBackbuffer->m_nWidth - 1, pcBackbuffer->m_nHeight - 1 );
			pcBackbuffer->m_pcDriver->BltBitmap( pcBackbuffer, pcOldBuffer, cBlitRect, IRect( IPoint( 0, 0 ), cBlitRect.Size() ), DM_COPY, 0xff );
			/* Fill new areas with the default color */
			if( pcBackbuffer->m_nWidth > pcOldBuffer->m_nWidth )
				pcBackbuffer->m_pcDriver->FillRect( pcBackbuffer, os::IRect( pcOldBuffer->m_nWidth, 0, pcBackbuffer->m_nWidth - 1, pcBackbuffer->m_nHeight ),
													os::get_default_color( COL_NORMAL ), DM_COPY );
			if( pcBackbuffer->m_nHeight > pcOldBuffer->m_nHeight )
				pcBackbuffer->m_pcDriver->FillRect( pcBackbuffer, os::IRect( 0, pcOldBuffer->m_nHeight, pcBackbuffer->m_nWidth - 1, pcBackbuffer->m_nHeight ),
													os::get_default_color( COL_NORMAL ), DM_COPY );													
		}
		pcOldBuffer->Release();
		//dbprintf( "Released old backbuffer\n" );
	}
	pcChild->m_pcBackbuffer = pcBackbuffer;
	if( pcBackbuffer == NULL )
		pcChild->SetBitmap( m_pcBitmap );
	else
		pcChild->SetBitmap( pcBackbuffer );
}

/** Frees the backbuffer of all layers.
 * \par Description:
 * Called before a screen mode change to free all backbuffers. This is necessary
 * if the videodriver uses the memory manager of the DisplayDriver class. If we
 * wouldn’t do this then this allocation could fail.
 * been changed.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::FreeBackbuffers( void )
{
	Layer* pcChild;
	
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		if( pcChild->m_pcBackbuffer != NULL )
		{
			pcChild->m_pcBackbuffer->Release();
			pcChild->m_pcBackbuffer = NULL;
			pcChild->SetBitmap( NULL );
		}
	}
}

/** Updates the content of one layer.
 * \par Description:
 * Called from various places. The method will iterate through the parent layers
 * until it has found the one that is directly attached to the top layer. It will
 * then calculate the visible region of the layer and blit the content of the backbuffer
 * to the framebuffer.
 * \param pcChild - Pointer to the layer.
 * \param bUpdateChildren - If true then also the child areas will be updated.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::UpdateLayer( Layer* pcChild, bool bUpdateChildren )
{
	ClipRect* pcWindowClip;
	ClipRect* pcLayerClip;
	SrvBitmap* pcBackbuffer = NULL;
	Layer* pcLayer = NULL;
	
	
	if( pcChild->m_nHideCount > 0 || pcChild->m_pcBitmapReg == NULL ) 
		return;
		
	if( pcChild->GetWindow() != NULL && pcChild->GetWindow()->HasPendingSizeEvents( this ) )
	{
		return;
	}
	
	IPoint cTopLeft( 0, 0 );
		
	/* Find the parent layer which is a direct child of us */
	pcLayer = pcChild;
	while( pcLayer->GetParent() != NULL )
	{		
		cTopLeft += pcLayer->GetIFrame().LeftTop();
		if( pcLayer->GetParent() == this && pcLayer->m_pcBackbuffer != NULL 
			&& pcLayer->m_pcVisibleFullReg != NULL )
		{
			pcBackbuffer = pcLayer->m_pcBackbuffer;
			break;
		}
		pcLayer = pcLayer->GetParent();
	}
	
	if( pcBackbuffer == NULL )
		return;

	assert( m_pcBitmap != NULL );
	
	SrvSprite::Hide( pcChild->GetIBounds() + cTopLeft );
	
	/* Intersect the visible region of the directly connected layer with the visible region of the layer */		
	ENUMCLIPLIST( &pcLayer->m_pcVisibleFullReg->m_cRects, pcWindowClip )
	{
		ENUMCLIPLIST( ( bUpdateChildren ? &pcChild->m_pcBitmapFullReg->m_cRects : &pcChild->m_pcBitmapReg->m_cRects ), pcLayerClip )
		{
			IRect cDstRect = ( pcWindowClip->m_cBounds ) & ( pcLayerClip->m_cBounds + cTopLeft );
			cDstRect &= GetIBounds();
			IRect cSrcRect = cDstRect - pcLayer->GetIFrame().LeftTop();
			
			if( cSrcRect.IsValid() && cDstRect.IsValid() )
			{
				m_pcBitmap->m_pcDriver->BltBitmap( m_pcBitmap, pcLayer->m_pcBackbuffer, cSrcRect, cDstRect, DM_COPY, 0xff );
			}
		}
	}
	
	SrvSprite::Unhide();
}


/** Reallocates the backbuffer of all layers.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will
 * reallocate the backbuffer of all layers because the colorspace might have
 * been changed.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::SetFrame( const os::Rect& cRect )
{
	Layer::SetFrame( cRect );
	Layer* pcChild;
	
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		LayerFrameChanged( pcChild, pcChild->GetIFrame() );
	}
}


static int SortCmp( const void *pNode1, const void *pNode2 )
{
	ClipRect *pcClip1 = *( ( ClipRect ** ) pNode1 );
	ClipRect *pcClip2 = *( ( ClipRect ** ) pNode2 );


	if( pcClip1->m_cBounds.left > pcClip2->m_cBounds.right && pcClip1->m_cBounds.right < pcClip2->m_cBounds.left )
	{
		if( pcClip1->m_cMove.x < 0 )
		{
			return ( pcClip1->m_cBounds.left - pcClip2->m_cBounds.left );
		}
		else
		{
			return ( pcClip2->m_cBounds.left - pcClip1->m_cBounds.left );
		}
	}
	else
	{
		if( pcClip1->m_cMove.y < 0 )
		{
			return ( pcClip1->m_cBounds.top - pcClip2->m_cBounds.top );
		}
		else
		{
			return ( pcClip2->m_cBounds.top - pcClip1->m_cBounds.top );
		}
	}
}


/** Updates moved layers.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will move the
 * content of moved layers. Instead of blitting the content from the layers backbuffer
 * it will perform framebuffer to framebuffer blits. This will accelerate it if the
 * graphics card does not support backbuffers in video memory but does support framebuffer
 * to framebuffer blits.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::MoveChilds( void )
{
	Layer* pcChild;
	ClipRect* pcClip;
	
	/* Copy the contents of the moved layers. We use framebuffer to framebuffer blits here */
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		if( pcChild->m_nHideCount == 0 && pcChild->m_cDeltaMove != IPoint( 0, 0 ) )
		{
			if( pcChild->m_bHasInvalidRegs && pcChild->m_pcVisibleFullReg != NULL 
				&& pcChild->m_pcPrevVisibleFullReg != NULL )
			{
				Region cCopy;
				Region cDamage( *pcChild->m_pcVisibleFullReg );
				ClipRect* pcOldClip = NULL;
				ClipRect* pcNewClip = NULL;
				int nCount = 0;
				IPoint cChildMove( pcChild->m_cDeltaMove );
				
				ENUMCLIPLIST( &pcChild->m_pcPrevVisibleFullReg->m_cRects, pcOldClip )
				{
					ENUMCLIPLIST( &pcChild->m_pcVisibleFullReg->m_cRects, pcNewClip )
					{
						IRect cRect = ( pcOldClip->m_cBounds + cChildMove ) & pcNewClip->m_cBounds;
						if( cRect.IsValid() )
						{
							nCount++;
							cCopy.AddRect( cRect );
							cDamage.Exclude( cRect );
						}
					}
				}
				
				if( nCount == 0 )
				{
					continue;
				}
				
				ClipRect **apsClips = new ClipRect *[nCount];

				for( int i = 0; i < nCount; ++i )
				{
					apsClips[i] = cCopy.m_cRects.RemoveHead();
					assert( apsClips[i] != NULL );
				}
				qsort( apsClips, nCount, sizeof( ClipRect * ), SortCmp );

				for( int i = 0; i < nCount; ++i )
				{
					ClipRect *pcClip = apsClips[i];

					m_pcBitmap->m_pcDriver->BltBitmap( m_pcBitmap, m_pcBitmap, pcClip->m_cBounds - cChildMove, pcClip->m_cBounds, DM_COPY, 0xff );
					Region::FreeClipRect( pcClip );
				}
				delete[]apsClips;
				
				if( pcChild->m_pcBackbuffer == NULL )
					continue;
				
				cDamage.Optimize();
			
				if( !cDamage.IsEmpty() )
				{
					/* Update the layer */
					ENUMCLIPLIST( &cDamage.m_cRects, pcClip )
					{
						IRect cDstRect = pcClip->m_cBounds;
						cDstRect &= GetIBounds();
						IRect cSrcRect = cDstRect - pcChild->GetIFrame().LeftTop();
					
						if( cSrcRect.IsValid() && cDstRect.IsValid() )
						{
							m_pcBitmap->m_pcDriver->BltBitmap( m_pcBitmap, pcChild->m_pcBackbuffer, cSrcRect, cDstRect, DM_COPY, 0xff );
						}
					}
				}
			}
		}
	}
	
	delete( m_pcPrevBitmapFullReg );
	m_pcPrevBitmapFullReg = NULL;
	
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		pcChild->MoveChilds();
	}
}


/** Updates newly visible areas.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will update newly
 * visible areas of directly connected layer by using backbuffer to framebuffer blits.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::InvalidateNewAreas( void )
{
	Layer* pcChild;
	ClipRect* pcClip;
	
	bool bSpritesHidden = false;
	
	/* Update the contents of layers which have not been moved */
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		if( pcChild->m_cDeltaMove == IPoint( 0, 0 ) && pcChild->m_nHideCount == 0 && 
			pcChild->m_bHasInvalidRegs && pcChild->m_pcVisibleFullReg != NULL && pcChild->m_pcBackbuffer != NULL )
		{
			/* Calculate newly visible areas */
			Region cDamage( *pcChild->m_pcVisibleFullReg );
			
			if( pcChild->m_pcPrevVisibleFullReg != NULL )
				cDamage.Exclude( *pcChild->m_pcPrevVisibleFullReg );
				
			cDamage.Optimize();
				
			assert( m_pcBitmap != NULL );
			
			if( !cDamage.IsEmpty() )
			{
				/* Update the layer */
				ENUMCLIPLIST( &cDamage.m_cRects, pcClip )
				{
					IRect cDstRect = pcClip->m_cBounds;
					cDstRect &= GetIBounds();
					IRect cSrcRect = cDstRect - pcChild->GetIFrame().LeftTop();
					
					if( cSrcRect.IsValid() && cDstRect.IsValid() )
					{						
						if( bSpritesHidden == false )
						{
							if( SrvSprite::DoIntersect( cDstRect ) )
							{
								SrvSprite::Hide();
								bSpritesHidden = true;
							}
						}
						m_pcBitmap->m_pcDriver->BltBitmap( m_pcBitmap, pcChild->m_pcBackbuffer, cSrcRect, cDstRect, DM_COPY, 0xff );
					}
				}
			}
		}
		delete( pcChild->m_pcPrevVisibleFullReg );
		pcChild->m_pcPrevVisibleFullReg = NULL;
	}
	
	if( bSpritesHidden )
		SrvSprite::Unhide();
	
	Layer::InvalidateNewAreas();
}


/** Updates the visible areas of the connected layer.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will rebuild
 * the visible areas of the connected layers.
 * \param bForce - Not used here. Just passed to the base implementation.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::RebuildRegion( bool bForce )
{
	Layer* pcChild;
	Layer* pcSibling;
	
	Layer::RebuildRegion( bForce );
	
	for( pcChild = m_pcBottomChild; NULL != pcChild; pcChild = pcChild->m_pcHigherSibling )
	{
		assert( pcChild->m_pcPrevVisibleFullReg == NULL );
		pcChild->m_pcPrevVisibleFullReg = pcChild->m_pcVisibleFullReg;
		pcChild->m_pcVisibleFullReg = NULL;
		
		if( pcChild->m_nHideCount > 0 )
		{
			delete( pcChild->m_pcPrevVisibleFullReg );
			pcChild->m_pcPrevVisibleFullReg = NULL;
			continue;
		}
		
		/* Build the visible region of the layer */
		pcChild->m_pcVisibleFullReg = new Region( ( pcChild->GetIFrame() & GetIBounds() ) );
		for( pcSibling = pcChild->m_pcHigherSibling; NULL != pcSibling; pcSibling = pcSibling->m_pcHigherSibling )
		{
			if( pcSibling->m_nHideCount == 0 )
				pcChild->m_pcVisibleFullReg->Exclude( ( pcSibling->GetIFrame() & GetIBounds() ) );
		}
		pcChild->m_pcVisibleFullReg->Optimize();
	}
}


/** Adds a child to the toplayer.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will call the
 * base implementation but will also create a backbuffer for the layer.
 * \param pcChild - Pointer to the child.
 * \param bTopmost - Just passed to the base implementation.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::AddChild( Layer * pcChild, bool bTopmost )
{
	/* Allocate the backbuffer for the child if it is added */
	//dbprintf( "Added %s\n", pcChild->GetName() );
	Layer::AddChild( pcChild, bTopmost );

	LayerFrameChanged( pcChild, pcChild->GetIFrame() );
}


/** Removes a layer from the toplayer.
 * \par Description:
 * This method overrides the implementation in the Layer class. It will call the
 * base implementation but will also delete the visible region of the child.
 * The backbuffer is not deleted here (it is deleted in the destructor of the layer
 * class) because if a view is moved to top it is first removed and then readded.
 * In this case we want to keep the backbuffer.
 * \param pcChild - Pointer to the child.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::RemoveChild( Layer * pcChild )
{
	//dbprintf( "Removed %s\n", pcChild->GetName() );
	Layer::RemoveChild( pcChild );
	/* Delete the visible areas of the child */
	delete( pcChild->m_pcVisibleFullReg );
	pcChild->m_pcVisibleFullReg = NULL;
	
}


/** Does the necessary steps to update a layer.
 * \par Description:
 * This method overrides the implementation in the Layer class. It works like the
 * base implementation but also draws visible areas of the top layer.
 * \param bForce - If set then the regions will be recalculated even if the 
 *					m_bHasInvalidRegs flag is not set.
 * \author Arno Klenke
 *****************************************************************************/
void TopLayer::UpdateRegions( bool bForce )
{
	RebuildRegion( bForce );
	MoveChilds();
	InvalidateNewAreas();
	
	if( m_bHasInvalidRegs && m_pcParent == NULL && m_pcBitmap != NULL && m_pcDamageReg != NULL )
	{
		if( m_pcBitmap == g_pcScreenBitmap )
		{
			/* Update the content of the top layer */
			ClipRect *pcClip;
			Color32_s sColor( 0, 0, 0, 255 );
			IPoint cTopLeft( ConvertToRoot( Point( 0, 0 ) ) );

			Region cDrawReg( *m_pcBitmapReg );

			cDrawReg.Intersect( *m_pcDamageReg );

			ENUMCLIPLIST( &cDrawReg.m_cRects, pcClip )
			{
				m_pcBitmap->m_pcDriver->FillRect( m_pcBitmap, pcClip->m_cBounds + cTopLeft, sColor, DM_COPY );
			}
		}
		delete m_pcDamageReg;

		m_pcDamageReg = NULL;
	}
	
	UpdateIfNeeded();
	ClearDirtyRegFlags();
}









