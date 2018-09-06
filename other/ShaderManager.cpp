//
// Created by ValentinDU on 31/01/2018.
//

#include "ShaderManager.h"
#include "glad_interface.h"

ShaderManager::ShaderManager() : m_shader_programs_data(new ShaderProgramData[MAX_SHADER_PROGRAMS]) {

}

ShaderManager::ShaderManager(std::vector<std::string> shaders) : m_shader_programs_data(
        new ShaderProgramData[MAX_SHADER_PROGRAMS]) {
    for (auto &shader : shaders) {
        register_program(shader);
    }
}

bool ShaderManager::register_program(std::string const &name) {
    bool success = true;
    ShaderProgramData *shader_program_data = new ShaderProgramData();

    shader_program_data->vertex_source_filename += PATH_TO_SHADERS + name + ".vert";
    shader_program_data->fragment_source_filename += PATH_TO_SHADERS + name + ".frag";

    // Create corresponding shaders
    if (!compile_shader(shader_program_data->vertex_id, GL_VERTEX_SHADER,
                        shader_program_data->vertex_source_filename)) {
        std::cout << "Error: Could not compile vertex shader" << std::endl;
        success = false;
    }

    if (!compile_shader(shader_program_data->fragment_id, GL_FRAGMENT_SHADER,
                        shader_program_data->fragment_source_filename)) {
        std::cout << "Could not compile fragment shader" << std::endl;
        success = false;
    }

    if (!create_program(shader_program_data)) {
        std::cout << "Error: Could not create program (generated ID = 0)" << std::endl;
        success = false;
    }

    if (!success) {
        std::cout << "Error: Cannot create shader program" << std::endl;
    } else // creation success
    {
        unsigned program_id = shader_program_data->program_id;
        m_shader_programs_map.insert(ShaderProgramPair(name, program_id));

        if (DI_VALID(program_id)) {
            m_shader_programs_data[program_id] = *shader_program_data;
        }
    }

    debug(shader_program_data);

    DI_DELETE(shader_program_data);

    return success;
}

unsigned ShaderManager::get_program_id(std::string const &name) {
    if (!m_shader_programs_map.count(name)) {
        std::cout << "Error: Could not find Shader Program in the Program List" << std::endl;
    }

    return m_shader_programs_map[name];
}

void ShaderManager::use_program(std::string const &name){
    glUseProgram(get_program_id(name));
}

void ShaderManager::debug(ShaderProgramData *shader_program_data) {
    // @DEBUG: display what file is read !
    std::cout << "Vert file = " << shader_program_data->vertex_source_filename << std::endl;
    std::cout << "Frag file = " << shader_program_data->fragment_source_filename << std::endl;
    std::cout << "Vert ID = " << shader_program_data->vertex_id << std::endl;
    std::cout << "Frag ID = " << shader_program_data->fragment_id << std::endl;
    std::cout << "Prog ID = " << shader_program_data->program_id << std::endl;
}

bool ShaderManager::compile_shader(GLuint &shader, GLenum type, std::string const &fichierSource) {
    // Create shader
    shader = glCreateShader(type);

    // Invalid (NULL) shader
    if (shader == 0) {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }

    // Read source code from file
    std::ifstream fichier(fichierSource.c_str());

    // Debug: non-opening file
    if (!fichier) {
        std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
        glDeleteShader(shader);
        return false;
    }

    // Var for file/source_code reading
    std::string line;
    std::string source_code;

    // Read lines from file
    while (getline(fichier, line)) {
        source_code += line += '\n';
    }

    // Close file
    fichier.close();

    // Convert source code properly
    const GLchar *chaineCodeSource = source_code.c_str();

    // Send source code to the shader
    glShaderSource(shader, 1, &chaineCodeSource, 0);

    // Compile shader
    glCompileShader(shader);

    // Check compilation status
    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);

    // Detect compilation errors
    if (erreurCompilation != GL_TRUE) {
        // Get error size
        GLint error_size(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_size);

        // Allocate memory to store error data
        char *error = new char[error_size + 1];

        // Get error data
        glGetShaderInfoLog(shader, error_size, &error_size, error);
        error[error_size] = '\0';

        // Debug: display error
        std::cout << error << std::endl;

        // Free memory
        delete[] error;
        glDeleteShader(shader);

        return false;
    } else // no error
    {
        return true;
    }
}

bool ShaderManager::create_program(ShaderProgramData *shader_program_data) {
    // Create Program
    unsigned program_id = glCreateProgram();

    // Register program id in structure for information purpose
    shader_program_data->program_id = program_id;

    // Bind Shaders to the newly created Program
    glAttachShader(program_id, shader_program_data->vertex_id);
    glAttachShader(program_id, shader_program_data->fragment_id);

    // Link Program
    glLinkProgram(program_id);

    // Check link status between Program & their Shaders
    GLint erreurLink(0);
    glGetProgramiv(program_id, GL_LINK_STATUS, &erreurLink);

    // => Error occured
    if (erreurLink != GL_TRUE) {
        // Get error size
        GLint tailleErreur(0);
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &tailleErreur);

        // Memory allocation for the error
        char *erreur = new char[tailleErreur + 1];

        // Get error log info
        glGetShaderInfoLog(program_id, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';

        // Debug: display error
        std::cout << erreur << std::endl;

        // Free memory
        delete[] erreur;
        glDeleteProgram(program_id);

        return false;
    } else // creation success
    {
        return true;
    }
}

void ShaderManager::set_bool(std::string const &program_name, std::string const &uniform_name, bool value) {
    glUniform1i(glGetUniformLocation(get_program_id(program_name), uniform_name.c_str()), (int) value);
}

void ShaderManager::set_int(std::string const &program_name, std::string const &uniform_name, int value) {
    glUniform1i(glGetUniformLocation(get_program_id(program_name), uniform_name.c_str()), value);
}

void ShaderManager::set_float(std::string const &program_name, std::string const &uniform_name, float value) {
    glUniform1f(glGetUniformLocation(get_program_id(program_name), uniform_name.c_str()), value);
}

void ShaderManager::activate_texture(unsigned texture, unsigned location_id){
    glActiveTexture(GL_TEXTURE0 + location_id);
    glBindTexture(GL_TEXTURE_2D, texture); // optionnal
}