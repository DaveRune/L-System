////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012, 2013
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// texture example: How to draw triangles with textures
//
// Level: 0
//
// Demonstrates:
//   Basic framework app
//   Shaders
//   Basic Matrices
//   Textures
//


#include "lsystem/lsystem.h"

namespace octet {
  class lsystem_app : public octet::app {
    mat4t modelToWorld;              // this matrix converts from model space to world space
    mat4t cameraToWorld;             // this matrix converts from camera space to world space
    color_shader color_shader_; // this builds a shader to use with textures
		LSYS_production_rules *pd;
		LSYS_File_Finder *ff;
		string axiom;

  public:

    // this is called when we construct the class
    lsystem_app(int argc, char **argv) : app(argc, argv) {
    }

    // this is called once OpenGL is initialized
    void app_init() {

			// set up the shader
      color_shader_.init();

			// Create lsystems class and iterate
			ff = new LSYS_File_Finder();
			ff->LS_LocateLsystemFiles();
			pd = new LSYS_production_rules();
			axiom = "F";
			pd->Iterate(axiom, 5);


      modelToWorld.loadIdentity();
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 120, 180);

    }

		void Controls() {
#pragma region Camera Movement
			if(is_key_down('W')) {
				cameraToWorld.translate(0, +10, 0);
			} 
			if(is_key_down('S')) {
				cameraToWorld.translate(0, -10, 0);
			}
			if(is_key_down('A')) {
				cameraToWorld.translate(-10, 0, 0);
			}
			if(is_key_down('D')) {
				cameraToWorld.translate(+10, 0, 0);
			}
			if(is_key_down('Q')) {
				cameraToWorld.translate(0, 0, -10);
			}
			if(is_key_down('E')) {
				cameraToWorld.translate(0, 0, +10);
			}
			if(is_key_down('R')) {
				cameraToWorld.loadIdentity();
				cameraToWorld.translate(0, 120, 180);
			}
#pragma endregion

#pragma region L-System Loading
			if(is_key_down('1')) {
				printf("One");
			}
#pragma endregion
		}

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
			Controls();
      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glEnable(GL_DEPTH_TEST);
			int index = 0;
			pd->Process(axiom, index, modelToWorld, cameraToWorld, color_shader_, 25.7f);
    }
  };
}
