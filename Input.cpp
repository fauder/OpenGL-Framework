// Project Includes.
#include "Input.h"
#include "Platform.h"

namespace Framework::Input
{
	void Process()
	{
		Platform::PollEvents();
		
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_ESCAPE ) )
			Platform::SetShouldClose( true );
	}
}
