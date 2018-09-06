////
//// Created by ValentinDU on 20/02/2018.
////
//
//#include <HUD/ImGuiHUD.h>
//#include <GLFWEnvironment.h>
//
//#include <scenes/SceneManager.h>
//#include "ImGui/imgui.h"
//#include <ImGui/imgui_impl_glfw.h>
//#include <ImGui/imgui_impl_opengl3.h>
//
//ImGuiHUD::ImGuiHUD(SceneManager &scene_manager, GLFWEnvironment *glfw_environment,
//                                   bool install_callbacks) : m_scene_manager(scene_manager) {
//    m_glfw_environment = glfw_environment;
//    m_install_callbacks = install_callbacks;
//
//    // Load Fonts
//    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//    // - Read 'misc/fonts/README.txt' for more instructions and details.
//    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//    //io.Fonts->AddFontDefault();
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//    //IM_ASSERT(font != NULL);
//}
//
//int ImGuiHUD::init() {
//    if (m_glfw_environment == nullptr) {
//        return -1;
//    }
//
//    if (m_glfw_environment->get_window() == nullptr) {
//        return -1;
//    }
//
//    // INIT GLEW ??? Already initialized in GLFW ?!
//
//    // Setup ImGui binding
//    ImGui::CreateContext();
//
//    ImGuiIO &io = ImGui::GetIO();
//    (void) io;
//
//    ImGui_ImplGlfwGL3_Init(m_glfw_environment->get_window(), m_install_callbacks);
//    //io.NavFlags |= ImGuiNavFlags_EnableKeyboard;  // Enable Keyboard Controls
//    //io.NavFlags |= ImGuiNavFlags_EnableGamepad;   // Enable Gamepad Controls
//
//    // Setup style
//    ImGui::StyleColorsDark();
//    ImGui::StyleColorsClassic();
//
//    /// CONFIGURE AND SHOW A WINDOW
//    m_show_demo_window = true;
//    m_show_another_window = true;
//    m_show_menubar = true;
//    m_show_window_scene = true;
//    m_show_window_entity = true;
//    m_clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//}
//
//void ImGuiHUD::update() {
//    ImGui_ImplGlfwGL3_NewFrame();
//
//    // 1. Show a simple window.
//    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
//    {
//        static float f = 0.0f;
//        static int counter = 0;
//        ImGui::Text(
//                "Hello, world!");                           // Display some text (you can use a format string too)
//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//        ImGui::ColorEdit3("clear color", (float *) &m_clear_color); // Edit 3 floats representing a color
//
//        ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our windows open/close state
//        ImGui::Checkbox("Another Window", &m_show_another_window);
//
//        if (ImGui::Button(
//                "Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);
//
//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
//                    ImGui::GetIO().Framerate);
//    }
//
//    //@TODO faire un menu qui ne prend pas de rectangle noir en dessous
//    ImGuiWindowFlags my_menubar_flags = 0;
//    //my_menubar_flags = ImGuiWindowFlags_AlwaysAutoResize;
//    my_menubar_flags += ImGuiWindowFlags_MenuBar;
//    my_menubar_flags += ImGuiWindowFlags_NoTitleBar;
//    ImGui::Begin("MenuBar", &m_show_menubar, my_menubar_flags);
//    ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width(), 0));
//    if (ImGui::BeginMenuBar()) {
//        if (ImGui::BeginMenu("File")) {
//            showMyExampleMenuFile();
//            ImGui::EndMenu();
//        }
//        if (ImGui::BeginMenu("Edit")) {
//            ImGui::EndMenu();
//        }
//        ImGui::EndMenuBar();
//    }
//    ImGui::End();
//
//    static int entitySelected = -1;
//    static int sceneSelected = -1;
//    ImGuiWindowFlags my_window_scene_flags = 0;
//    //my_menubar_flags = ImGuiWindowFlags_AlwaysAutoResize;
//    my_window_scene_flags += ImGuiWindowFlags_MenuBar;
//    my_window_scene_flags += ImGuiWindowFlags_NoTitleBar;
//    ImGui::Begin("Window scene", &m_show_window_scene, my_window_scene_flags);
//    ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width() * 0.15f,m_glfw_environment->get_height()*0.70f));
//    if (ImGui::BeginMenuBar()) {
//        if (ImGui::BeginMenu("File")) {
//            showMyExampleMenuFile();
//            ImGui::EndMenu();
//        }
//        ImGui::EndMenuBar();
//
//        ImGui::Spacing();
//        for (int i = 0; i < m_scene_manager.getScenes().size(); i++) {
//            if (ImGui::CollapsingHeader(m_scene_manager.getScenes()[i]->getName().c_str())) {
//                for(int j=0; j < m_scene_manager.getScenes()[i]->getEntities().size(); j++) {
//                    if(ImGui::Selectable( m_scene_manager.getScenes()[i]->getEntities()[j].name.c_str(), entitySelected == j)) {
//                        entitySelected = j;
//                        sceneSelected = i;
//                    }
//                }
//            }
//        }
//    }
//    ImGui::End();
//
//
//    ImGuiWindowFlags my_window_entity_flags = 0;
//    //my_menubar_flags = ImGuiWindowFlags_AlwaysAutoResize;
//    //my_window_scene_flags += ImGuiWindowFlags_MenuBar;
//    my_window_scene_flags += ImGuiWindowFlags_NoTitleBar;
//    ImGui::Begin("Window entity", &m_show_window_entity, my_window_entity_flags);
//    ImGui::SetWindowSize(ImVec2(m_glfw_environment->get_width() * 0.15f,m_glfw_environment->get_height()*0.70f));
//    if(entitySelected != -1 && sceneSelected != -1) {
//        Entity e = m_scene_manager.getScenes()[sceneSelected]->getEntities()[entitySelected];
//        ImGui::Text(e.name.c_str());
//        ImGui::Spacing();
//        //ImGui::CollapsingHeader(.id)
//    }
//    ImGui::End();
//
//    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
//    if (m_show_another_window) {
//        ImGui::Begin("Another Window", &m_show_another_window);
//        ImGui::Text("Hello from another window!");
//        if (ImGui::Button("Close Me"))
//            m_show_another_window = false;
//        ImGui::End();
//    }
//
//    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
//    if (m_show_demo_window) {
//        ImGui::SetNextWindowPos(ImVec2(650, 20),
//                                ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
//        ImGui::ShowDemoWindow(&m_show_demo_window);
//    }
//}
//
//void ImGuiHUD::render() {
//    ImGui::Render();
//    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//}
//
//void ImGuiHUD::close() {
//    ImGui_ImplGlfwGL3_Shutdown();
//    ImGui::DestroyContext();
//}
//
//void ImGuiHUD::showMyExampleMenuFile() {
//    ImGui::MenuItem("File menu :D", NULL, false, false);
//    if (ImGui::MenuItem("New")) {}
//    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
//    if (ImGui::BeginMenu("Open Recent")) {
//        ImGui::MenuItem("fish_hat.c");
//        ImGui::MenuItem("fish_hat.inl");
//        ImGui::MenuItem("fish_hat.h");
//        if (ImGui::BeginMenu("More..")) {
//            ImGui::MenuItem("Hello");
//            ImGui::MenuItem("Sailor");
//            if (ImGui::BeginMenu("Recurse..")) {
//                showMyExampleMenuFile();
//                ImGui::EndMenu();
//            }
//            ImGui::EndMenu();
//        }
//        ImGui::EndMenu();
//    }
//    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
//    if (ImGui::MenuItem("Save As..")) {}
//    ImGui::Separator();
//    if (ImGui::BeginMenu("Options")) {
//        static bool enabled = true;
//        ImGui::MenuItem("Enabled", "", &enabled);
//        ImGui::BeginChild("child", ImVec2(0, 60), true);
//        for (int i = 0; i < 10; i++)
//            ImGui::Text("Scrolling Text %d", i);
//        ImGui::EndChild();
//        static float f = 0.5f;
//        static int n = 0;
//        static bool b = true;
//        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
//        ImGui::InputFloat("Input", &f, 0.1f);
//        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
//        ImGui::Checkbox("Check", &b);
//        ImGui::EndMenu();
//    }
//    if (ImGui::BeginMenu("Colors")) {
//        float sz = ImGui::GetTextLineHeight();
//        for (int i = 0; i < ImGuiCol_COUNT; i++) {
//            const char *name = ImGui::GetStyleColorName((ImGuiCol) i);
//            ImVec2 p = ImGui::GetCursorScreenPos();
//            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol) i));
//            ImGui::Dummy(ImVec2(sz, sz));
//            ImGui::SameLine();
//            ImGui::MenuItem(name);
//        }
//        ImGui::EndMenu();
//    }
//    if (ImGui::BeginMenu("Disabled", false)) // Disabled
//    {
//        IM_ASSERT(0);
//    }
//    if (ImGui::MenuItem("Checked", NULL, true)) {}
//    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
//}