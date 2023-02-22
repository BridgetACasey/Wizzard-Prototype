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

//Accessibility
#include "wizzard/screenreading/ScreenReaderSupport.h"

//User Interface
#include "wizzard/ui/ImGuiLayer.h"
#include "wizzard/ui/ImGuiScreenReading.h"

//Entity Component System
#include "wizzard/scene/Entity.h"
#include "wizzard/scene/component/TagComponent.h"
#include "wizzard/scene/component/TransformComponent.h"
#include "wizzard/scene/component/SpriteComponent.h"
#include "wizzard/scene/component/CameraComponent.h"
#include "wizzard/scene/component/RigidBody2DComponent.h"
#include "wizzard/scene/component/BoxCollider2DComponent.h"
#include "wizzard/scene/Scene.h"

//Audio
#include "wizzard/audio/Audio.h"
#include "wizzard/audio/AudioSource.h"

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