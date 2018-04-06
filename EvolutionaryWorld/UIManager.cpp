#include "UIManager.h"

#include <SDL.h>
#include <SDL_syswm.h>
#include "IMGUI\imgui.h"
#include <GL\glew.h>

#include "UIWidget.h"
#include "FLog.h"
#include "Matrix4.h"


UIManager::UIManager(SDL_Window * window)
	:uiShader("uiShader")
{
	// Setup Imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	// Set special keys bindings
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.SetClipboardTextFn = [](void*, const char* text) {SDL_SetClipboardText(text); };
	io.GetClipboardTextFn = [](void*) -> const char* {return SDL_GetClipboardText(); };
	io.ClipboardUserData = NULL;

	#ifdef _WIN32
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);
		io.ImeWindowHandle = wmInfo.info.win.window;
	#else
		(void)window;
	#endif

	// Setup style
	ImGui::StyleColorsDark();

	// Setup ui texture
	uiShader.loadFromDisk("uiShader");

	// Load font texture
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);  

	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)fontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &vboBuffer);
	glGenBuffers(1, &elementBuffer);
}

UIManager::~UIManager()
{
	glDeleteTextures(1, &fontTexture);
	glDeleteBuffers(1, &vboBuffer);
	glDeleteBuffers(1, &elementBuffer);

	// Delete manually rest of widgets
	for (auto& it : widgets_map)
	{
		delete it.second;
	}
}

void UIManager::processInputEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
		io.MousePos = ImVec2(event->motion.x, event->motion.y);
		break;
	case SDL_MOUSEWHEEL:
	{
		if (event->wheel.x > 0) io.MouseWheelH += 1;
		if (event->wheel.x < 0) io.MouseWheelH -= 1;
		if (event->wheel.y > 0) io.MouseWheel += 1;
		if (event->wheel.y < 0) io.MouseWheel -= 1;
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		if (event->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
		if (event->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
		if (event->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		if (event->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
		if (event->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
		if (event->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
		break;
	}
	case SDL_TEXTINPUT:
	{
		io.AddInputCharactersUTF8(event->text.text);
		break;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		int key = event->key.keysym.scancode;
		IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
		io.KeysDown[key] = (event->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
		break;
	}
	}
}

void UIManager::processAndRenderWidgets(float deltaTime)
{
	// Prepare frame
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(1080.0f, 720.0f);
	io.DisplayFramebufferScale = ImVec2(1, 1);

	io.DeltaTime = deltaTime;

	ImGui::NewFrame();

	// Render widgets
	for (auto& widget : widgets_map)
	{
		widget.second->render();
	}

	// Debug code
	ImGui::ShowTestWindow();

	// Render everything to screen
	ImGui::Render();

	ImDrawData* drawData = ImGui::GetDrawData();

	drawData->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
	glActiveTexture(GL_TEXTURE0);
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
	GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
	GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
	GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
	GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
	GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

	float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
		{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
		{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	uiShader.bind();
	uiShader.uniform("Texture", 0);
	uiShader.uniformMatrix("ProjMtx", Matrix4(ortho_projection));
	glBindSampler(0, 0); // Rely on combined texture/sampler state.

	GLuint vao_handle = 0;
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

	// IMGUI draw loop
	for (int n = 0; n < drawData->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = drawData->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}
	glDeleteVertexArrays(1, &vao_handle);

	uiShader.unBind();

	// Restore modified GL state
	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindSampler(0, last_sampler);
	glActiveTexture(last_active_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void UIManager::registerWidget(UIWidget* widget)
{
	if (widgets_map.find(widget->getName()) == widgets_map.end())
	{
		widgets_map.insert(std::make_pair(widget->getName(), widget));
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name already exists.");
	}
}

void UIManager::unregisterWidget(const UIWidget * widget)
{
	if (widgets_map.find(widget->getName()) != widgets_map.end())
	{
		widgets_map.erase(widget->getName());
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name doesn't exists, can't be removed.");
	}
}

UIWidget * UIManager::getWidget(std::string name)
{
	auto it = widgets_map.find(name);
	if (it != widgets_map.end())
	{
		return it->second;
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name doesn't exists.");
	}
}
