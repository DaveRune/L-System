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
		
		LSYS_File_Finder *ff;
		LSYS_File_Reader *fr;
		LSYS_Production_Rules *pd;

		int LSystemQuantity;
		int currentDocument;

		string fileDirectory;

		//string axiom;

  public:

    // this is called when we construct the class
    lsystem_app(int argc, char **argv):

		app(argc, argv),
		LSystemQuantity(0),
		currentDocument(0)
		{
			for(int i = 0; i < argc; i++) 
				fileDirectory += argv[i];
		}

    // this is called once OpenGL is initialized
    void app_init() {

			// set up the shader
      color_shader_.init();

			// Create lsystems class and iterate
			fr = new LSYS_File_Reader();
			ff = new LSYS_File_Finder(*fr);
			ff->LS_AssignFileLoc(fileDirectory);
			ff->LS_LocateLsystemFiles();

			pd = new LSYS_Production_Rules();
			pd->AssignLSystem(&fr->LSystems[0]);
			pd->Iterate();

			LSystemQuantity = fr->LSystems.size();

			int index = 0;
      modelToWorld.loadIdentity();
			float treeHeight = 0, treeWidth = 0;
			pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[0].angle);
			pd->ProcessTree(treeHeight, treeWidth);
			treeHeight *= 0.5f;  treeWidth *= 0.5f;
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(treeWidth, treeHeight, treeHeight*1.2f);
    }

		void Controls() {
			static bool keyPressed = false;
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
			//if(is_key_down('R')) {
			//	cameraToWorld.loadIdentity();
			//	cameraToWorld.translate(0, 120, 180);
			//}
			if(is_key_down(key_space)) {
				system("cls");
				fr->DisplayFileResults();
				pd->DisplayLSystemDraw();
			}
#pragma endregion

#pragma region L-System Loading
			int index = 0;
			if(is_key_down('1')) {
				if(0 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[0]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[0].angle);
					currentDocument = 0;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('2')) {
				if(1 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[1]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[1].angle);
					currentDocument = 1;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('3')) {
				if(2 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[2]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[2].angle);
					currentDocument = 2;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('4')) {
				if(3 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[3]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[3].angle);
					currentDocument = 3;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('5')) {
				if(4 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[4]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[4].angle);
					currentDocument = 4;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('6')) {
				if(5 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[5]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[5].angle);
					currentDocument = 5;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('7')) {
				if(6 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[6]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[6].angle);
					currentDocument = 6;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('8')) {
				if(7 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[7]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[7].angle);
					currentDocument = 7;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down('9')) {
				if(8 >= LSystemQuantity) return;
				if(!keyPressed)
				{
					pd->AssignLSystem(&fr->LSystems[8]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[8].angle);
					currentDocument = 8;
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			}
			else if(is_key_down(key_up)) {
				if(!keyPressed) {
					pd->ReIterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_);
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			} else if(is_key_down(key_down)) {
				if(!keyPressed) {
					pd->DeIterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_);
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			} else if(is_key_down(key_left)) {
				if(!keyPressed) {
					if(currentDocument-1 < 0) return;
					else currentDocument--;
					pd->AssignLSystem(&fr->LSystems[currentDocument]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[currentDocument].angle);
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			} else if(is_key_down(key_right)) {
				if(!keyPressed) {
					if(currentDocument+1 >= LSystemQuantity) return;
					else currentDocument++;
					pd->AssignLSystem(&fr->LSystems[currentDocument]);
					pd->Iterate();
					pd->Process(pd->axiom, index, modelToWorld, cameraToWorld, color_shader_, fr->LSystems[currentDocument].angle);
					float treeHeight = 0, treeWidth = 0;
					pd->ProcessTree(treeHeight, treeWidth);
					treeHeight *= 0.5f;  treeWidth *= 0.5f;
					cameraToWorld.loadIdentity();
					cameraToWorld.translate(0, treeHeight, treeHeight*1.2f);
					keyPressed = true;
				}
			} else
				keyPressed = false;
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
			
			pd->RenderOne(cameraToWorld, color_shader_);
    }
  };
}
