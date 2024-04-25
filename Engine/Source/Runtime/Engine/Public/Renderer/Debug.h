#pragma once

#include "BatchRenderer.h"


namespace Debug
{

	static void DrawLine( const FDebugLineRenderInfo& Info );
}

#if HE_DEBUG
#	define HE_DRAW_DEBUG_LINE(LineInfo) Debug::DrawLine( LineInfo );
#else
#	define HE_DRAW_DEBUG_LINE(Point1, Point2, Color, Thickness) 
#endif