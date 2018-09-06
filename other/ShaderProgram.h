//
// Created by ValentinDU on 30/01/2018.
//

#ifndef GLGUI_SHADERPROGRAMDATA_H
#define GLGUI_SHADERPROGRAMDATA_H

#include <iostream>

struct ShaderProgramData
{
    std::string vertex_source_filename;
    std::string fragment_source_filename;

    unsigned program_id = 0;
    unsigned vertex_id = 0;
    unsigned fragment_id = 0;
} ;

#endif //GLGUI_SHADERPROGRAMDATA_H
