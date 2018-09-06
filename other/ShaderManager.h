//
// Created by ValentinDU on 31/01/2018.
//

// @TODO: Replace map "count() iteration" by "Key <-> Value" search, and catch an exception (+ throw a message) when name (:string) is not found

// @TODO: Build a shader file generator for vertex and fragment ( generate_vertex_file() and generate_fragment_file() )

// @TODO: Add possibility to SET a shader (vertex, fragment, or a program)
// @TODO: Add a shader DELETE functionnality

#ifndef DREAMINENGINE_SHADERMANAGER_H
#define DREAMINENGINE_SHADERMANAGER_H

#include <unordered_map>
#include <fstream>
#include <vector>
#include "ShaderProgram.h"

#define NULL_PTR 0
#define MAX_SHADER_PROGRAMS 16
#define PATH_TO_SHADERS "../shaders/"
#define DI_DELETE(x) ({ delete (x); (x) = 0; })
#define DI_VALID(x) ({ int validity; validity = ((x) > 0 && (x) < MAX_SHADER_PROGRAMS); validity;})

typedef std::unordered_map<std::string, unsigned> ShaderProgramMap;
typedef std::pair<std::string, unsigned> ShaderProgramPair;

class ShaderManager {
public:
    explicit ShaderManager();

    /* Register 1 to N shaders */
    explicit ShaderManager(std::vector<std::string> shaders);

    /* Register Program
     *   Create a shader
     *   Bind a source to this shader
     *   Compile the shader
     *   Register the newly created structure
     *  */
    bool register_program(std::string const &name);

    /* Get Shader Program
     *   Retrieve a Program ID for a given name
     *   => Get program id
     */
    unsigned get_program_id(std::string const &name);

    /* Use Program
     * Calls glUseProgram() with a specific program ID
     */
    void use_program(std::string const &name);

    /* Set Uniform 1 Bool */
    void set_bool(std::string const &program_name, std::string const &uniform_name, bool value);

    /* Set Uniform 1 Int */
    void set_int(std::string const &program_name, std::string const &uniform_name, int value);

    /* Set Uniform 1 Float */
    void set_float(std::string const &program_name, std::string const &uniform_name, float value);

    /* Activate and Bind a Texture in OpenGL
     * Takes a texture ID and its location (ID) in the shader manager */
    void activate_texture(unsigned texture, unsigned location_id = 0); // location_id in shader samples;

private:
    /* Compile Shader
     *   Create a shader
     *   Load source code from file
     *   Put source code into the shader
     *   Check compilation errors
     */
    bool compile_shader(unsigned &shader, unsigned type, std::string const &fichierSource);

    /* Create Program
     *   Create a program (id)
     *   Attach shaders to the program
     *   Link the Program
     *   Check program creation error
     */
    bool create_program(ShaderProgramData *shader_program_data);

    /* Debug
     * @DEBUG: Display Shader Data
     */
    void debug(ShaderProgramData *shader_program_data);

    ShaderProgramMap m_shader_programs_map;

    ShaderProgramData *m_shader_programs_data = NULL_PTR;
};


#endif //DREAMINENGINE_SHADERMANAGER_H
