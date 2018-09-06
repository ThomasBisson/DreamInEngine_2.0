//
// Created by ValentinDU on 20/02/2018.
//

#ifndef DREAMINENGINE_IMGUIGLFWCONTEXT_H
#define DREAMINENGINE_IMGUIGLFWCONTEXT_H

class GLFWEnvironment;

class SceneManager;

struct ImVec4;

class ImGuiHUD {
public:
    ImGuiHUD(SceneManager &scene_manager, GLFWEnvironment *glfw_environment, bool install_callbacks);

    /* Init
     * Initialize ImGui Context for a given GLFW environment
     * @return -1 if GLFW context is not initialized
     * @return -1 if gl3w cannot be initialized
     * */
    int init();

    /* Configure ImGui window(s) EVERY FRAME */
    void update();

    /* Render/Show the ImGui frame(s) */
    void render();

    void close();

    void showMyExampleMenuFile();

private:
    SceneManager &m_scene_manager;
    GLFWEnvironment *m_glfw_environment;
    bool m_install_callbacks;
    bool m_show_demo_window;
    bool m_show_another_window;
    ImVec4 *m_clear_color;
    bool m_show_menubar;
    bool m_show_window_scene;
    bool m_show_window_entity;
};


#endif //DREAMINENGINE_IMGUIGLFWCONTEXT_H
