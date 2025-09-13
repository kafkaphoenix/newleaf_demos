#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>
#include <newleaf/application/application.h>
#include <newleaf/components/camera/cActiveCamera.h>
#include <newleaf/components/camera/cCamera.h>
#include <newleaf/components/input/cActiveInput.h>
#include <newleaf/components/input/cInput.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/events/mouse_event.h>
#include <newleaf/window/input.h>
#include <newleaf/window/windows_manager.h>

namespace spz {

inline bool on_mouse_moved(nl::MouseMovedEvent& e, entt::registry& registry) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and nl::Application::get().is_debugging() or nl::Application::get().is_paused()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  // pitch (rotate around x in radians)
  // yaw (rotate around y in radians)
  // roll (rotate around z in radians)
  entt::entity camera = registry.view<nl::CCamera, nl::CActiveCamera, nl::CTransform, nl::CUUID>().front();
  APP_ASSERT(camera not_eq entt::null, "no camera found");
  nl::CCamera& cCamera = registry.get<nl::CCamera>(camera);
  APP_ASSERT(cCamera.mode not_eq nl::CCamera::Mode::_2d, "2d camera not supported in this demo");

  if (registry.all_of<nl::CActiveInput>(camera)) {
    nl::CInput& cInput = registry.get<nl::CInput>(camera);
    cCamera.right_angle += e.get_x() * cInput.mouse_sensitivity;
    cCamera.up_angle += e.get_y() * cInput.mouse_sensitivity;

    cCamera.right_angle = std::fmod(cCamera.right_angle, 360.f);
    cCamera.up_angle = std::clamp(cCamera.up_angle, -89.f, 89.f);

    // yaw mouse movement in x-direction
    glm::quat rot_y = glm::angleAxis(glm::radians(-cCamera.right_angle), glm::vec3(0, 1, 0));
    // pitch mouse movement in y-direction
    glm::quat rot_x = glm::angleAxis(glm::radians(cCamera.up_angle), glm::vec3(1, 0, 0));

    nl::CTransform& cTransform = registry.get<nl::CTransform>(camera);
    cTransform.rotation = rot_y * rot_x;
    // Normalize the rotation quaternion to prevent drift
    cTransform.rotation = glm::normalize(cTransform.rotation);
  } else {
    // TODO add static camera
    APP_ASSERT(false, "static camera not implemented yet");
    entt::entity movable = registry.view<nl::CInput, nl::CActiveInput, nl::CTransform, nl::CUUID>().front();
    APP_ASSERT(movable not_eq entt::null, "no movable found");
    nl::CInput& cInput = registry.get<nl::CInput>(movable);
  }

  return true;
}

inline bool on_mouse_scrolled(nl::MouseScrolledEvent& e, entt::registry& registry) {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = nl::Application::get();
  if (io.WantCaptureMouse and app.is_debugging() or app.is_paused()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  entt::entity camera = registry.view<nl::CCamera, nl::CActiveCamera, nl::CTransform, nl::CUUID>().front();
  APP_ASSERT(camera not_eq entt::null, "no camera found");
  nl::CCamera& cCamera = registry.get<nl::CCamera>(camera);
  APP_ASSERT(cCamera.mode == nl::CCamera::Mode::_3d, "only 3d camera mode supported in this demo");

  nl::CTransform& cTransform = registry.get<nl::CTransform>(camera);

  cCamera.zoom_factor = std::clamp(cCamera.zoom_factor + float(e.get_y()), cCamera.zoom_min, cCamera.zoom_max);

  // TODO third person camera when scrolling out
  cCamera.calculate_projection();

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
      windows_manager.set_last_mouse_position(nl::Input::get_mouse_x(), nl::Input::get_mouse_y());
      if (app.should_restore_pause()) {
        app.pause(true);
        app.restore_pause(false);
      } else {
        app.pause(false);
      }
    } else {
      app.debug(true);
      if (app.is_paused()) {
        app.restore_pause(true);
      } else {
        app.pause(true);
      }
      glfwSetCursor(windows_manager.get_native_window(), nullptr);
      windows_manager.set_cursor_mode(nl::CursorMode::normal, false);
      windows_manager.update_camera_position(false);
    }
    return true;
  } else if (e.get_key() == nl::Key::Escape) {
    if (debugging) {
      windows_manager.restore_cursor();
    }
    app.close();
    return true;
  } else if (e.get_key() == nl::Key::F12) {
    auto& settings_manager = app.get_settings_manager();
    windows_manager.toggle_fullscreen(not settings_manager.fullscreen);
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
    windows_manager.set_last_mouse_position(nl::Input::get_mouse_x(), nl::Input::get_mouse_y());
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

inline void input_handler(nl::Event& e, entt::registry& registry) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::MouseMovedEvent>(BIND_STATIC_EVENT(on_mouse_moved, registry));
  dispatcher.dispatch<nl::MouseScrolledEvent>(BIND_STATIC_EVENT(on_mouse_scrolled, registry));
  dispatcher.dispatch<nl::MouseButtonPressedEvent>(BIND_STATIC_EVENT(on_mouse_button_pressed));
  dispatcher.dispatch<nl::MouseButtonReleasedEvent>(BIND_STATIC_EVENT(on_mouse_button_released));

  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_STATIC_EVENT(on_key_pressed));
  dispatcher.dispatch<nl::KeyReleasedEvent>(BIND_STATIC_EVENT(on_key_released));
  dispatcher.dispatch<nl::KeyTypedEvent>(BIND_STATIC_EVENT(on_key_typed));
}
}