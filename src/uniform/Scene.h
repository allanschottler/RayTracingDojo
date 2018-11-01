#pragma once
#include "../gl/uniform.h"
#include "../model/Scene.h"
#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

namespace uniform
{
    template<typename Program>
    inline void load(const Program& program, const Scene& model)
    {
    /******************************************************************************************/
    /*      Uniformes da câmera                                                               */
    /******************************************************************************************/
        
        // Pega matrizes da câmera
//        glm::vec4 viewport;
//        glGetFloatv(GL_VIEWPORT, glm::value_ptr(viewport)); 

        glm::mat4 view  = model.camera().modelview;
        glm::mat4 projection = model.camera().projection;
        glm::vec4 viewport   = model.camera().viewport;
        
        int width  = viewport.z;
        int height = viewport.w;
      
//        glm::mat4 modelview, projection;
//        glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelview));
//        glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));

        // Passa matrizes para o programa
        gl::uniform(program, model.transform, "model");
        gl::uniform(program, view, "view");
        gl::uniform(program, projection, "projection");
        gl::uniform(program, glm::vec2{viewport.z, viewport.w}, "viewport");  
        gl::uniform(program, model.lightPos, "lightPos");
        
    /******************************************************************************************/
    /*      Uniformes de frustrum                                                             */
    /******************************************************************************************/

        // Pega pontos do near e far
        glm::vec3 nearCenter     {width/2.f, height/2.f, 0.f};
        glm::vec3 nearHorizontal {width, height/2.f, 0.f};
        glm::vec3 nearVertical   {width/2.f, height, 0.f};            
        glm::vec3 farCenter      {width/2.f, height/2.f, 1.f};
        glm::vec3 farHorizontal  {width, height/2.f, 1.f};
        glm::vec3 farVertical    {width/2.f, height, 1.f};
        
        // Transforma pontos para mundo
        nearCenter     = glm::unProject(nearCenter, view, projection, viewport);
        nearHorizontal = glm::unProject(nearHorizontal, view, projection, viewport);
        nearVertical   = glm::unProject(nearVertical, view, projection, viewport);
        farCenter      = glm::unProject(farCenter, view, projection, viewport);
        farHorizontal  = glm::unProject(farHorizontal, view, projection, viewport);
        farVertical    = glm::unProject(farVertical, view, projection, viewport);
        
        // Passa pontos para o programa
        gl::uniform(program, nearCenter, "nearCenter");
        gl::uniform(program, nearHorizontal - nearCenter, "nearRight");
        gl::uniform(program, nearVertical - nearCenter, "nearUp");
        gl::uniform(program, farCenter, "farCenter");
        gl::uniform(program, farHorizontal - farCenter, "farRight");
        gl::uniform(program, farVertical - farCenter, "farUp");
        
        // Passa objetos para o programa
        int index = 0;
        for(const auto& sphere : model.objects())
            uniform::load(program, sphere, index++);
    }
}

