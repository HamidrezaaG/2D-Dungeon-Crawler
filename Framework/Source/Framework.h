#ifndef __Framework_H__
#define __Framework_H__

// This header includes all files inside the framework project.
// It is meant for use by game projects ONLY.
// As new files are created, include them here so the game has easy access to them.
// It should not be included by any files inside the framework project.

#include "FrameworkPCH.h"

#include "Core/FWCore.h"
#include "Core/GameCore.h"

#include "Utility/ShaderProgram.h"
#include "Utility/Helpers.h"
#include "FWConsts.h"

#include "Utility/Color.h"
#include "Utility/Timer.h"
#include "Math/Vector.h"
#include "Math/Math.h"

#include "Objects/Mesh.h"
#include "Objects/GameObject.h"

#include "UI/ImGuiManager.h"

#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

#include "Objects/Camera.h"

#include "Objects/Collider.h"
#include "Objects/CircleCollider.h"
#include "Objects/RectangleCollider.h"

#include "Objects/SpriteSheet.h"

#include "Objects/Texture.h"

#endif //__Framework_H__
