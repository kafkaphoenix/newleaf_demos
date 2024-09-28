#pragma once

#include <entt/entt.hpp>
#include <imgui.h>
#include <newleaf/application/application.h>
#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/events/mouse_event.h>
#include <newleaf/window/input.h>
#include <newleaf/window/windows_manager.h>

namespace fb {

inline bool on_mouse_moved(nl::MouseMovedEvent& e, entt::registry& registry) {
  return true;
}

inline bool on_mouse_scrolled(nl::MouseScrolledEvent& e,
                              entt::registry& registry) {
  return true;
}

inline bool on_mouse_button_pressed(nl::MouseButtonPressedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and nl::Application::get().is_debugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline bool on_mouse_button_released(nl::MouseButtonReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and nl::Application::get().is_debugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline bool on_key_pressed(nl::KeyPressedEvent& e) {
  auto& app = nl::Application::get();
  auto& windows_manager = app.get_windows_manager();
  bool debugging = app.is_debugging();
  bool paused = app.is_paused();

  if (e.get_key() == nl::Key::F3) {
    if (debugging) {
      app.debug(false);
      windows_manager.restore_cursor();
      windows_manager.update_camera_position(true);
      windows_manager.set_last_mouse_position(nl::Input::get_mouse_x(),
                                               nl::Input::get_mouse_y());
      if (nl::Application::get().should_restore_pause()) {
        nl::Application::get().pause(true);
        nl::Application::get().restore_pause(false);
      } else {
        nl::Application::get().pause(false);
      }
    } else {
      app.debug(true);
      if (nl::Application::get().is_paused()) {
        nl::Application::get().restore_pause(true);
      } else {
        nl::Application::get().pause(true);
      }
      glfwSetCursor(windows_manager.get_native_window(), nullptr);
      windows_manager.set_cursor_mode(nl::CursorMode::normal, false);
      windows_manager.update_camera_position(false);
    }
    return true;
  } else if (e.get_key() == nl::Key::Escape) {
    // TODO move to layers for flappy bird
    if (debugging) {
      windows_manager.restore_cursor();
    }
    app.close();
    return true;
  }

  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and debugging) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  if (e.get_key() == nl::Key::LeftAlt) {
    if (not debugging) {
      windows_manager.update_camera_position(false);
      windows_manager.set_cursor_mode(nl::CursorMode::normal, false);
    }
    return true;
  } else if (e.get_key() == nl::Key::P) {
    app.pause(not paused);
    return true;
  }

  return false;
}

inline bool on_key_released(nl::KeyReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = nl::Application::get();

  if (io.WantCaptureKeyboard and app.is_debugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  if (e.get_key() == nl::Key::LeftAlt and not app.is_debugging()) {
    auto& windows_manager = app.get_windows_manager();
    windows_manager.set_last_mouse_position(nl::Input::get_mouse_x(),
                                             nl::Input::get_mouse_y());
    windows_manager.update_camera_position(true);
    windows_manager.restore_cursor();
    return true;
  }

  return false;
}

inline bool on_key_typed(nl::KeyTypedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and nl::Application::get().is_debugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline void input_dispatcher(nl::Event& e, entt::registry& registry) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::MouseMovedEvent>(
    BIND_STATIC_EVENT(on_mouse_moved, registry));
  dispatcher.dispatch<nl::MouseScrolledEvent>(
    BIND_STATIC_EVENT(on_mouse_scrolled, registry));
  dispatcher.dispatch<nl::MouseButtonPressedEvent>(
    BIND_STATIC_EVENT(on_mouse_button_pressed));
  dispatcher.dispatch<nl::MouseButtonReleasedEvent>(
    BIND_STATIC_EVENT(on_mouse_button_released));

  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_STATIC_EVENT(on_key_pressed));
  dispatcher.dispatch<nl::KeyReleasedEvent>(BIND_STATIC_EVENT(on_key_released));
  dispatcher.dispatch<nl::KeyTypedEvent>(BIND_STATIC_EVENT(on_key_typed));
}
}