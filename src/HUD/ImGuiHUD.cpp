//
// Created by ValentinDU on 20/02/2018.
//

#include <HUD/ImGuiHUD.h>
#include <GLFWEnvironment.h>

#include <scenes/SceneManager.h>
#include "ImGui/imgui.h"
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_internal.h>

/////////////////////////////////////////
float value = 0.00f;
float positionX = 0.00f, positionY = 0.00f;
float sizeX = 0.00f, sizeY = 0.00f;
float velocityX = 0.00f, velocityY = 0.00f;
float rotation = 0.00f;
/////////////////////////////////////////
bool* v = new bool[1]{ false };

#pragma region COMPONENT DATA (ENUM & STRING)
//std::string component_type_str[4] = {
//	"No Component",
//	"Sprite Component",
//	"BoxPhysics Component",
//	"Input Component"
//};
std::unordered_map<unsigned int, std::string> component_names_map;
std::unordered_map<std::string, unsigned int> component_types_map;
#pragma endregion

void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	const float height = ImGui::GetFrameHeight();
	const float width = height * 1.55f;
	const float radius = height * 0.50f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	const float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

ImGuiHUD::ImGuiHUD(SceneManager &scene_manager, GLFWEnvironment *glfw_environment,
	bool install_callbacks) : m_scene_manager(scene_manager) {
	m_glfw_environment = glfw_environment;
	m_install_callbacks = install_callbacks;

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
}

int ImGuiHUD::init() {
	if (m_glfw_environment == nullptr) {
		std::cout << "Error: GLFW is not initialized for ImGui" << std::endl;
		return false;
	}

	if (m_glfw_environment->get_window() == nullptr) {
		std::cout << "Error: GLFW window not initialized for ImGui" << std::endl;
		return false;
	}

	// Setup ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(m_glfw_environment->get_window(), true);

#if __APPLE__
	const char* glsl_version = "#version 150";
#else
	const char* glsl_version = "#version 130";
#endif

	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsLight();

	// DreamIn Engine main frames
	m_show_window_menubar = true;
	m_show_window_scene = true;
	m_show_window_entity = true;
	m_show_window_explorer = true;
	// Debug frames
	m_show_another_window = true;
	m_show_demo_window = true;

	//TODO: never forget to update this map when adding a new type
	component_names_map[SPRITE] = "Sprite Component";
	component_names_map[INPUT] = "Input Component";
	component_names_map[BOX2D] = "BoxPhysics Component";

	component_types_map[component_names_map[SPRITE]] = SPRITE;
	component_types_map[component_names_map[INPUT]] = INPUT;
	component_types_map[component_names_map[BOX2D]] = BOX2D;
}

void ImGuiHUD::update() {
	static int entitySelected = -1;
	static int sceneSelected = -1;

	static unsigned int component_selected = SPRITE;
	static bool show_listbox_components = false;

	static bool queryComponentInfo = false; // TODO: See if it is really useful in the future
	static BooleanCustom query_add_component = BooleanCustom();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_show_window_menubar)
	{
		//@TODO faire un menu qui ne prend pas de rectangle noir en dessous
		ImGuiWindowFlags my_menubar_flags = 0;
		my_menubar_flags |= ImGuiWindowFlags_MenuBar;
		my_menubar_flags |= ImGuiWindowFlags_NoTitleBar;
		my_menubar_flags |= ImGuiWindowFlags_NoCollapse;
		my_menubar_flags |= ImGuiWindowFlags_NoMove;
		my_menubar_flags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("MenuBar", nullptr, my_menubar_flags);
		ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width(), 0));

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				showMyExampleMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::Button("play")) {
			if (m_scene_manager.getRunningConfigEnum() == CONFIG)
				m_scene_manager.setRunningConfigEnum(RUNNING);
			else
				m_scene_manager.setRunningConfigEnum(CONFIG);
		}

		this->UpdateCurrentWindowRectData(&m_window_menubar);

		ImGui::End();
	}

	ImGuiWindowFlags my_window_scene_flags = 0;
	my_window_scene_flags |= ImGuiWindowFlags_NoMove;
	my_window_scene_flags |= ImGuiWindowFlags_NoCollapse;
	my_window_scene_flags |= ImGuiWindowFlags_NoResize;
	my_window_scene_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

	ImGui::Begin("Window scene", nullptr, my_window_scene_flags);
	ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width() * 0.2f, m_glfw_environment->get_height() - this->m_window_menubar.h));
	ImGui::SetWindowPos(ImVec2(0, 0 + this->m_window_menubar.h));
	ImGui::Spacing();
	for (unsigned int i = 0; i < m_scene_manager.getScenes().size(); i++) {
		if (ImGui::CollapsingHeader(m_scene_manager.getScenes()[i]->getName().c_str())) {
			for (unsigned int j = 0; j < m_scene_manager.getScenes()[i]->getEntities().size(); j++) {
				if (ImGui::Selectable(m_scene_manager.getScenes()[i]->getEntities()[j].name.c_str(), entitySelected == j)) {
					entitySelected = j;
					sceneSelected = i;
					// Reset the text triggered by the ADD COMPONENT (when we click on an entity in the "window scene")
					query_add_component = BooleanCustom();
				}
			}
		}
	}

	this->UpdateCurrentWindowRectData(&m_window_scene);

	ImGui::End();

	if (m_show_window_entity)
	{
		ImGuiWindowFlags my_window_entity_flags = 0;
		my_window_entity_flags |= ImGuiWindowFlags_NoMove;
		my_window_entity_flags |= ImGuiWindowFlags_NoCollapse;
		my_window_entity_flags |= ImGuiWindowFlags_NoResize;
		my_window_entity_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		ImGui::Begin("Window entity", nullptr, my_window_entity_flags);
		ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width() * 0.2f, m_glfw_environment->get_height() - this->m_window_menubar.h));
		ImGui::SetWindowPos(ImVec2(m_glfw_environment->get_width() - ImGui::GetWindowWidth(), 0 + this->m_window_menubar.h));

		ImGui::BeginGroup();
		if (entitySelected != -1 && sceneSelected != -1)
		{
			Entity entity = m_scene_manager.getActualScene().getEntities()[entitySelected];

			ImGui::Text("%s (id: %d, mask: %d)", entity.name.c_str(), entity.id, entity.mask);
			ImGui::Spacing();

			// TODO: For the selected entity: Display all its components
			// TODO: The displayed compoennts should be represented as Big Clickable Buttons 
			for (unsigned int component_type : m_scene_manager.get_components(entity.id))
			{
				if (ImGui::Button((component_names_map[component_type]).c_str())) // button pressed ?
				{
					component_selected = component_type;
				}
			}

			// NOTE: Always add one more "button" -> add a new component to the selected entity
			if (ImGui::Button("ADD COMPONENT")) // TODO: Improve the design of the button (add a "+" in a "circle" shape centered in the button)
			{
				show_listbox_components = true; // Trigger ListBox choices
			}

			if (show_listbox_components)
			{
				const char* listbox_items[3];
				static int listbox_item_current = 0;
				int item_id = 0;

				// build listbox item list
				for (auto& it : component_names_map)
				{
					listbox_items[item_id++] = (it.second).c_str(); // Name (component)
				}

				if (ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4))
				{
					const std::string component_selected_name = listbox_items[listbox_item_current];
					const unsigned int component_type = component_types_map[component_selected_name];

					query_add_component = m_scene_manager.add_component(component_type, m_scene_manager.getActualScene().getEntities()[entitySelected].id);
					if (!std::empty(query_add_component.Message))
					{
						queryComponentInfo = true;
					}
				}
			}

			if (queryComponentInfo && !std::empty(query_add_component.Message))
			{
				ImGui::TextColored((query_add_component.Result ? ImVec4(0.0f, 0.5f, 0.0f, 1.0f) : ImVec4(0.7f, 0.0f, 0.0f, 1.0f)), query_add_component.Message.c_str()); // Color TEXT !!!
			}

			// TODO: Determine the utility of this ToggleButton
			// ToggleButton("Test", v);
		}
		ImGui::EndGroup();
		// 0<=>stretch
		ImGui::BeginChild("component_details", ImVec2(0.0f, ImGui::GetWindowHeight() / 2), true);
		ImGui::Text("COMPONENT DETAILS");
		ImGui::Spacing();
		// TODO: Make a component info file (or Interface with Strategy pattern) to define what to do with each component !
		if (entitySelected != -1 && sceneSelected != -1) // TOOD: only for test purpose ! Replace by better API calls
		{
			if (component_selected == SPRITE)
			{
				if (ImGui::TreeNode("Position"))
				{
					ImGui::BeginGroup();
					// First line
					ImGui::Text("X");
					ImGui::SameLine();
					if (ImGui::InputFloat(" position.x", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Position.x), 0.10f, 0, "%.3f")) // Text changed !
					{
					}
					// Second line
					ImGui::Text("Y");
					ImGui::SameLine();
					if (ImGui::InputFloat(" position.y", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Position.y), 0.10f, 0, "%.3f")) // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}

				ImGui::Spacing();

				// TODO: Add other tree nodes
				if (ImGui::TreeNode("Size"))
				{
					ImGui::BeginGroup();
					// First line
					ImGui::Text("W");
					ImGui::SameLine();
					if (ImGui::InputFloat(" size.x", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Size.x), 0.10f, 0, "%.3f")) // Text changed !
					{
					}
					// Second line
					ImGui::Text("H");
					ImGui::SameLine();
					if (ImGui::InputFloat(" size.y", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Size.y), 0.10f, 0, "%.3f")) // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}

				ImGui::Spacing();

				if (ImGui::TreeNode("Rotation"))
				{
					ImGui::BeginGroup();
					ImGui::Text("Angle");
					ImGui::SameLine();
					if (ImGui::InputFloat("°", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Rotation), 0.10f, 0, "%.3f")) // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}

				ImGui::Spacing();

				if (ImGui::TreeNode("Color"))
				{
					ImGui::BeginGroup();
					// First line
					ImGui::Text("R");
					ImGui::SameLine();
					if (ImGui::InputFloat(" color.r", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Color.x), 0.10f, 0, "%.1f")) // Text changed !
					{
					}
					// Second line
					ImGui::Text("V");
					ImGui::SameLine();
					if (ImGui::InputFloat(" color.v", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Color.y), 0.10f, 0, "%.1f")) // Text changed !
					{
					}
					// Third line
					ImGui::Text("B");
					ImGui::SameLine();
					if (ImGui::InputFloat(" color.b", &(m_scene_manager.getActualScene().getEntities()[entitySelected], m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Color.z), 1.0f, 0, "%.1f")) // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}
			}

			if (component_selected == INPUT)
			{
				if (ImGui::TreeNode("Key Mapping"))
				{
					ImGui::BeginGroup();
					// First line
					ImGui::Text("Left");
					ImGui::SameLine();
					InputEnum *keyLeft = &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_left);
					int *keyLeftInt = (int*)keyLeft;
					if (ImGui::InputInt(" left", keyLeftInt)); // Text changed !
					{
					}
					// Second line
					ImGui::Text("Right");
					ImGui::SameLine();
					InputEnum *keyRight = &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_right);
					int *keyRightInt = (int*)keyRight;
					if (ImGui::InputInt(" right", keyRightInt)); // Text changed !
					{
					}
					// Third line
					ImGui::Text("Down");
					ImGui::SameLine();
					InputEnum *keyDown = &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_down);
					int *keyDownInt = (int*)keyDown;
					if (ImGui::InputInt(" down", keyDownInt)); // Text changed !
					{
					}
					// Fourth line
					ImGui::Text("Up");
					ImGui::SameLine();
					InputEnum *keyUp = &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_up);
					int *keyUpInt = (int*)keyUp;
					if (ImGui::InputInt(" up", keyUpInt)); // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}

				ImGui::Spacing();

				if (ImGui::TreeNode("Key Speed"))
				{
					ImGui::BeginGroup();
					// First line
					ImGui::Text("Left");
					ImGui::SameLine();
					if (ImGui::InputInt(" left", &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_speed_left))); // Text changed !
					{
					}
					// Second line
					ImGui::Text("Right");
					ImGui::SameLine();
					if (ImGui::InputInt(" right", &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_speed_right))); // Text changed !
					{
					}
					// Third line
					ImGui::Text("Down");
					ImGui::SameLine();
					if (ImGui::InputInt(" down", &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_speed_down))); // Text changed !
					{
					}
					// Fourth line
					ImGui::Text("Up");
					ImGui::SameLine();
					if (ImGui::InputInt(" up", &(m_scene_manager.getActualScene().getInputs().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->m_speed_up))); // Text changed !
					{
					}
					ImGui::EndGroup();
					ImGui::TreePop();
				}
			}
		}
		ImGui::EndChild();

		this->UpdateCurrentWindowRectData(&m_window_entity);

		ImGui::End();
	}

	if (m_show_window_explorer)
	{
		ImGuiWindowFlags my_window_explorer_flags = 0;
		my_window_explorer_flags |= ImGuiWindowFlags_NoMove;
		my_window_explorer_flags |= ImGuiWindowFlags_NoCollapse;
		my_window_explorer_flags |= ImGuiWindowFlags_NoResize;
		my_window_explorer_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		ImGui::Begin("Window Explorer", nullptr, my_window_explorer_flags);
		ImGui::SetWindowSize(ImVec2((m_glfw_environment->get_width() - m_window_scene.w - m_window_entity.w), m_glfw_environment->get_height() * 0.3f));
		ImGui::SetWindowPos(ImVec2(m_window_scene.w, (m_glfw_environment->get_height() * 0.7f))); // 0.8f stands for "glfw_height - (glfw_height * 0.2))" <=> "glfw_height * (1 - 0.2f)" <=> "glfw_height * 0.8f";

		ImGui::BeginGroup();
		// 0. Get textures bank data
		auto textures_bank = ResourceManager::GetTexturesBank();

		for (auto& it : textures_bank)
		{
			const std::string texture_name = it.first; // name;
			auto texture = it.second; // TODO: Load texture into ImGui

			// 1. TODO: Display all loaded resources (textures, for the moment)
				// Grid (selectable)
					// Texture Image
					// Text (under image, centered)Some Tex

			// 2. Clickable/Double-Clickable Texture&Text Button
			const bool button_pressed = ImGui::Button(it.first.c_str(), ImVec2(64.0f, 64.0f));
			ImGui::SameLine();

			// 2. Make textures selectable if an entity is selected
			if( entitySelected != -1 /* && sceneActive */ && has_component(m_scene_manager.getActualScene().getEntities()[entitySelected].mask, SPRITE) )
			{
				if (button_pressed)
				{
					// 2.2. Selected entity takes the selected Texture
					m_scene_manager.getActualScene().getSprites().get(m_scene_manager.getActualScene().getEntities()[entitySelected].id)->Texture = ResourceManager::GetTexture(texture_name);
				}
			}
		}

		ImGui::EndGroup();

		this->UpdateCurrentWindowRectData(&m_window_explorer);

		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (m_show_demo_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&m_show_demo_window);
	}
}

void ImGuiHUD::render() {
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHUD::close() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiHUD::showMyExampleMenuFile() {
	ImGui::MenuItem("File menu :D", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent")) {
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More..")) {
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse..")) {
				showMyExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options")) {
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		static bool b = true;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::Checkbox("Check", &b);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors")) {
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++) {
			const char *name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void ImGuiHUD::UpdateCurrentWindowRectData(ImGuiWindowRect* window_rect)
{
	window_rect->x = ImGui::GetWindowPos().x;
	window_rect->y = ImGui::GetWindowPos().y;
	window_rect->w = ImGui::GetWindowWidth();
	window_rect->h = ImGui::GetWindowHeight();
}

// TODO: right click on button to update its content (can do the same with text !
//static char name[32] = "Label1";
//char buf[64]; sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
//ImGui::Button(buf);
//if (ImGui::BeginPopupContextItem()) // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem().
//{
//	ImGui::Text("Edit name:");
//	ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
//	if (ImGui::Button("Close"))
//		ImGui::CloseCurrentPopup();
//	ImGui::EndPopup();
//}
//ImGui::SameLine(); ImGui::Text("(<-- right-click here)");

// NOTE: DRAW IMAGE EXAMPLE ?
// Recreate the VAO every time
// (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
//GLuint vao_handle = 0;
//glGenVertexArrays(1, &vao_handle);
//glBindVertexArray(vao_handle);
//glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
//glEnableVertexAttribArray(g_AttribLocationPosition);
//glEnableVertexAttribArray(g_AttribLocationUV);
//glEnableVertexAttribArray(g_AttribLocationColor);
//glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
//glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
//glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
//
//// Draw
//ImVec2 pos = draw_data->DisplayPos;
//for (int n = 0; n < draw_data->CmdListsCount; n++)
//{
//	const ImDrawList* cmd_list = draw_data->CmdLists[n];
//	const ImDrawIdx* idx_buffer_offset = 0;
//
//	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);
//
//	for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
//	{
//		const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
//		if (pcmd->UserCallback)
//		{
//			// User callback (registered via ImDrawList::AddCallback)
//			pcmd->UserCallback(cmd_list, pcmd);
//		}
//		else
//		{
//			ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
//			if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
//			{
//				// Apply scissor/clipping rectangle
//				glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
//
//				// Bind texture, Draw
//				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
//				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
//			}
//		}
//		idx_buffer_offset += pcmd->ElemCount;
//	}
//}