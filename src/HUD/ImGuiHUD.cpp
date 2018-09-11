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
}

void ImGuiHUD::update() {
	static int entitySelected = -1;
	static int sceneSelected = -1;
	static unsigned int component_selected = SPRITE;
	static bool queryComponentInfo = false; // TODO: See if it is really useful in the future
	static BooleanCustom query_add_component = BooleanCustom();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_show_window_menubar)
	{
		//@TODO faire un menu qui ne prend pas de rectangle noir en dessous
		ImGuiWindowFlags my_menubar_flags = 0;
		my_menubar_flags += ImGuiWindowFlags_MenuBar;
		my_menubar_flags += ImGuiWindowFlags_NoTitleBar;
		my_menubar_flags += ImGuiWindowFlags_NoCollapse;
		my_menubar_flags += ImGuiWindowFlags_NoMove;

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
	my_window_scene_flags += ImGuiWindowFlags_NoMove;
	my_window_scene_flags += ImGuiWindowFlags_NoCollapse;

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
		my_window_entity_flags = ImGuiWindowFlags_NoMove;
		my_window_entity_flags += ImGuiWindowFlags_NoCollapse;

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
			for(unsigned int component_type : m_scene_manager.get_components(entity.id))
			{
				if(ImGui::Button( (component_names_map[component_type]).c_str() )) // button pressed ?
				{
					component_selected = component_type;
				}
			}
			//if (ImGui::Button("[TEST] Sprite Component"))
			//{
			//	// m_scene_manager.getActualScene()->getComponent(component_name, entity);
			//	componentSelected = "sprite";
			//	//entitySelected, already defined/selected
			//	// sceneSelected, already defined/selected

			//}

			// NOTE: Always add one more "button" -> add a new component to the selected entity
			if (ImGui::Button("ADD COMPONENT")) // TODO: Improve the design of the button (add a "+" in a "circle" shape centered in the button)
			{
				// TODO: Do a select box menu and take the result as a ComponentTypeStr and convert it to a ComponentType to invoke add_component(type,entity) method
				query_add_component = m_scene_manager.add_component(INPUT, m_scene_manager.getActualScene().getEntities()[entitySelected].id);
				if (!std::empty(query_add_component.Message))
				{
					queryComponentInfo = true;
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
			if(component_selected == SPRITE)
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

			if(component_selected == INPUT)
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

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (m_show_another_window) {
		ImGui::Begin("Another (Debug/Test) Window", &m_show_another_window);
		ImGui::SetWindowSize(ImVec2(800, 400));
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			m_show_another_window = false;
		ImGui::BeginColumns("tests", 2, false);
		ImGui::Button("Row 1 Col 1");
		ImGui::Button("Row 2 Col 1");
		ImGui::NextColumn();
		ImGui::Button("Row 1 & 2 Col 2");
		ImGui::Button("O");
		if (ImGui::IsItemClicked()) // Refers to the LAST button !
		{
			ImGui::Text("FFFFF");
		}
		ImGui::EndColumns();

		if (ImGui::InputFloat("red", &value, 0.05f, 0, "%.3f")) // Text changed !
		{
			ImGui::Text("Focus (Edition): Text changed !");
			ImGui::Text("Clicked with value : %.3f", value);
			m_scene_manager.getScene("Aloha")->getEntities()[0], m_scene_manager.getScene("Aloha")->getSprites().get(m_scene_manager.getScene("Aloha")->getEntities()[0].id)->Position.x = value;
		}

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

// TODO: TextBox to modify
