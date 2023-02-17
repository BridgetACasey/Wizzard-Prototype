//@BridgetACasey

#pragma once

//Base
#include "wizzard/base/TimeStep.h"

//Core
#include "wizzard/core/Application.h"
#include "wizzard/core/Layer.h"
#include "wizzard/core/Log.h"
#include "wizzard/core/OrthographicCameraController.h"

//Input
#include "wizzard/input/Input.h"
#include "wizzard/input/KeyCode.h"
#include "wizzard/input/MouseCode.h"

//User Interface
#include "wizzard/ui/ImGuiLayer.h"

//Accessibility
#include "wizzard/screenreading/ImGuiScreenReading.h"

//Entity Component System
#include "wizzard/scene/Entity.h"
#include "wizzard/scene/TagComponent.h"
#include "wizzard/scene/TransformComponent.h"
#include "wizzard/scene/SpriteComponent.h"
#include "wizzard/scene/Scene.h"

//Renderer
#include "wizzard/rendering/Renderer.h"
#include "wizzard/rendering/Renderer2D.h"
#include "wizzard/rendering/RenderCommand.h"
#include "wizzard/rendering/Buffer.h"
#include "wizzard/rendering/Framebuffer.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/rendering/VertexArray.h"
#include "wizzard/rendering/Texture.h"
#include "wizzard/rendering/OrthographicCamera.h"

//Audio
#include "wizzard/audio/Audio.h"
#include "wizzard/audio/AudioSource.h"