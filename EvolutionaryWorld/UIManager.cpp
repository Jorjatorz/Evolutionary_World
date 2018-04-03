#include "UIManager.h"

#include <SDL.h>
#include <SDL_syswm.h>
#include <imgui.h>

#include "UIWidget.h"
#include "FLog.h"


UIManager::UIManager(SDL_Window * window)
{
	// Setup Imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

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
}

UIManager::~UIManager()
{
}

void UIManager::processInputEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
	case SDL_MOUSEWHEEL:
	{
		if (event->wheel.x > 0) io.MouseWheelH += 1;
		if (event->wheel.x < 0) io.MouseWheelH -= 1;
		if (event->wheel.y > 0) io.MouseWheel += 1;
		if (event->wheel.y < 0) io.MouseWheel -= 1;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		/*if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
		if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
		if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;*/
	}
	case SDL_TEXTINPUT:
	{
		io.AddInputCharactersUTF8(event->text.text);
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		/*int key = event->key.keysym.scancode;
		IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
		io.KeysDown[key] = (event->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);*/
	}
	}
}

void UIManager::processAndRenderWidgets()
{
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
