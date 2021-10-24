#pragma once
#define SOKOL_GLCORE33
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol_audio.h"

#include "imgui/imgui.h"
#include "sokol_imgui.h"
static bool initialized = false;
static struct
{
	sg_pipeline pip;
	sg_bindings bind;
	sg_pass_action pass_action;
	uint64_t laptime;
	struct
	{
		float mouse_x;
		float mouse_y;
	} input_state;
} state;


static void update() {

}
void init(void)
{
	sg_desc desc = {};
	desc.context = sapp_sgcontext();
	sg_setup(&desc);
	stm_setup();
	simgui_desc_t simgui_desc = {};
	simgui_setup(&simgui_desc);
	
	// initial clear color
	/*state.pass_action.colors[0].action = SG_ACTION_CLEAR;
	state.pass_action.colors[0].value.r = 0.0f;
	state.pass_action.colors[0].value.g = 0.5f;
	state.pass_action.colors[0].value.b = 0.7f;
	state.pass_action.colors[0].value.a = 1.0f;*/
	initialized = true;
}

void frame(void)
{
	const int width = sapp_width();
	const int height = sapp_height();
	const double delta_time = stm_sec(stm_round_to_common_refresh_rate(state.laptime)) + 0.001;
	simgui_new_frame(width, height, delta_time);

	/*=== UI CODE STARTS HERE ===*/
	
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 });
	ImGui::SetNextWindowSize(ImVec2(100, 200), ImGuiCond_Once);
	ImGui::Begin("Raven starter", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
	ImGui::End();

	/*=== UI CODE ENDS HERE ===*/

	sg_begin_default_pass(&state.pass_action, width, height);
	simgui_render();
	sg_end_pass();
	sg_commit();
}

void cleanup(void)
{
	saudio_shutdown();
	sg_shutdown();
}

void input(const sapp_event* event)
{
	simgui_handle_event(event);
	if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT && event->type == SAPP_EVENTTYPE_MOUSE_DOWN)
	{
		state.input_state.mouse_x = event->mouse_x / sapp_width() - 0.5f;
		state.input_state.mouse_y
			= ((sapp_height() - event->mouse_y) / sapp_height() - 0.5f);
		// state.input_state.mouse_y = 0;
	}
}

static void audio_update(float* buffer, int num_frames, int num_channels) {
	if (!initialized) return;
	assert(1 == num_channels);
	static uint32_t count = 0;

	for (int i = 0; i < num_frames; i++) {
		//buffer[i] = float(i | num_frames);
	}
}
sapp_desc sokol_main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	sapp_desc desc = {};
	desc.init_cb = init;
	desc.frame_cb = frame;
	desc.cleanup_cb = cleanup;
	desc.event_cb = input;
	desc.width = 320;
	desc.height = 240;
	
	desc.window_title = "Raven";
	saudio_desc audio_desc = {};
	audio_desc.stream_cb = audio_update;
	saudio_setup(&audio_desc);

	return desc;

}