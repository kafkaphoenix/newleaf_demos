#include "imgui_layer.h"

#include <imgui.h>

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cFBO.h>

static const char* CFBO_ModeNames[] = {"normal",
                                       "inverse",
                                       "greyscale",
                                       "blur",
                                       "edge",
                                       "sharpen",
                                       "nightvision",
                                       "emboss",
                                       "sepia",
                                       "comic",
                                       "one_bit_shading",
                                       "one_bit_dithered",
                                       "ps1",
                                       "cell_shader",
                                       "thermal",
                                       "cyanotype",
                                       "heatmap",
                                       "crt",
                                       "chromatic_aberration",
                                       "swirl",
                                       "horizontal_glitch",
                                       "wave",
                                       "kaleidoscope",
                                       "datamosh",
                                       "barrel_distortion",
                                       "ripple",
                                       "plasma",
                                       "pixelate",
                                       "mirror"};

namespace spz {

ImGuiLayer::ImGuiLayer() : Layer("imgui_layer") {}

void ImGuiLayer::on_imgui_update() {
  auto& app = nl::Application::get();
  bool debugging = app.is_debugging();
  if (!debugging)
    return;

  auto fbo = app.get_scene_manager().get_entity("fbo");
  if (fbo != entt::null) {
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Post Process Settings");
    auto& fbo_c = app.get_scene_manager().get_registry().get<nl::CFBO>(fbo);
    int current_mode = static_cast<int>(fbo_c.mode);
    if (ImGui::Combo("Mode", &current_mode, CFBO_ModeNames, IM_ARRAYSIZE(CFBO_ModeNames))) {
      fbo_c.mode = static_cast<nl::CFBO::Mode>(current_mode);
      fbo_c._mode = CFBO_ModeNames[current_mode];
    }
    ImGui::SliderFloat("Time", &fbo_c.time, 0.0f, 100.0f);
    ImGui::End();
  }
}


std::unique_ptr<nl::Layer> ImGuiLayer::create() { return std::make_unique<ImGuiLayer>(); }
}