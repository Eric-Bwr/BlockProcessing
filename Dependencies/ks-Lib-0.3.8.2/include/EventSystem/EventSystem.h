// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include <functional>

#define BIND_FUNC(fn) std::bind(&fn, this, std::placeholders::_1)

#include "Event/Event.h"
#include "Event/EventDispatcher.h"
#include "Event/Application/ApplicationRenderEvent.h"
#include "Event/Application/ApplicationUpdateEvent.h"
#include "Event/InputEvent/CharEvent.h"
#include "Event/InputEvent/CharModsEvent.h"
#include "Event/InputEvent/CursorEnteredEvent.h"
#include "Event/InputEvent/CursorLeavedEvent.h"
#include "Event/InputEvent/CursorPositionEvent.h"
#include "Event/InputEvent/KeyPressedEvent.h"
#include "Event/InputEvent/KeyReleasedEvent.h"
#include "Event/InputEvent/KeyRepeatEvent.h"
#include "Event/InputEvent/MouseButtonPressedEvent.h"
#include "Event/InputEvent/MouseButtonReleasedEvent.h"
#include "Event/InputEvent/MouseUpScrollEvent.h"
#include "Event/InputEvent/MouseDownScrollEvent.h"
#include "Event/WindowEvent/FramebufferSizeEvent.h"
#include "Event/WindowEvent/WindowCloseEvent.h"
#include "Event/WindowEvent/WindowDropEvent.h"
#include "Event/WindowEvent/WindowLostFocusEvent.h"
#include "Event/WindowEvent/WindowHasFocusEvent.h"
#include "Event/WindowEvent/WindowIconifyEvent.h"
#include "Event/WindowEvent/WindowRestoredEvent.h"
#include "Event/WindowEvent/WindowPositionEvent.h"
#include "Event/WindowEvent/WindowRefreshEvent.h"
#include "Event/WindowEvent/WindowSizeEvent.h"

#include "Error/ErrorEvent.h"
#include "Error/ApplicationErrorEvent.h"
#include "Error/FileStreamErrorEvent.h"
#include "Error/NetworkErrorEvent.h"
#include "Error/WindowErrorEvent.h"
#include "Error/GraphicalErrorEvent.h"