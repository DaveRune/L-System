#include "lsystem\dirent.h";

namespace octet {

	class LSYS_File_Reader {
    
  private:
    FILE *file;
    char fileBuffer[1024];

  public:
    LSYS_File_Reader ():
    file(NULL)
    {
      memset(fileBuffer, '\0', sizeof(fileBuffer));
    }

//    void LS_OpenFile () {
//      file = fopen("../../assets/LSystems/lsystem1.txt", "r");
//      if(file == NULL) {
//        printf("ERROR: LSystem File not found\n");
//      } else {
//        LS_ReadFile(file);
//      }
//    }

    void LS_ReadFile(FILE *file) {
      fseek(file, 0, SEEK_SET);
      int counter = 0;
      while(!feof(file)) {
        fileBuffer[counter++] = getc(file);
      }
      printf(fileBuffer);
      putc('\n', stdout);
      fclose(file);
    }
	};

	class LSYS_File_Finder {
		private:
		dynarray<FILE> LSYS_Files;

		public:

		LSYS_File_Finder(){}

		void LS_LocateLsystemFiles () {
			printf("\nSearching for L-System Files in \"../../assets/lsystem/configurations\" ... \n");

			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir ("../../assets/lsystem/configurations")) != NULL) {
				while ((ent = readdir (dir)) != NULL) {
					FILE *file = NULL;
					string fileLocation = "../../assets/lsystem/configurations/";
					fileLocation += ent->d_name;
					
					file = fopen(fileLocation, "r");
					if(file != NULL) {
						printf("Located File: %s ... ", ent->d_name);
						
						if(File_isLsystem(file)) {
							LSYS_File_Reader *fr = new LSYS_File_Reader();
							fr->LS_ReadFile(file);
						}
					}
				}
				closedir (dir);
			} else {
				/* could not open directory */
				perror ("");
			}

		}

		bool File_isLsystem (FILE *file) {
			char fileBuffer[512];
			memset(fileBuffer, '\0', sizeof(fileBuffer));
			int counter = 0;

			fseek(file, 0, SEEK_SET);
			while(!feof(file)) {
				fileBuffer[counter++] = getc(file);
				if(!strcmp(fileBuffer, "Lsystem")) {
					printf("CONFIRMED as L-System.\n");
					return true;
				}
			}
			printf("DECLINED! - File Not L-System!\n\n");
			return false;
		}
	};

	


	class LSYS_production_rules {

		public:

		// Pass is the data to store variables and rules
		void StoreRules() {
			
		}

		#pragma region Line Drawing
		void DrawLine
		(
			const mat4t &modelToWorld, 
			const mat4t &cameraToWorld,
			color_shader _shader,
			const vec2 &_first = vec2(0.0f, 0.0f), 
			const vec2 &_second = vec2(0.0f, 1.0f), 
			const vec4 &colour = vec4(0.8f, 0.8f, 0.0f, 1.0f)
		)
		{
			float vertices[6];
			//vec4 first = vec4(_first, 0.0f, 1.0f);
			//vec4 second = vec4(_second, 0.0f, 1.0f);
			//first = modelToWorld*first;
			//second = modelToWorld*second;

			vertices[0] = _first.x();
			vertices[1] = _first.y();
			vertices[2] = 0.0f;
			vertices[3] = _second.x();
			vertices[4] = _second.y();
			vertices[5] = 0.0f;

			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			_shader.render(modelToProjection, colour);

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			glDrawArrays(GL_LINES, 0, 2);
		}
		#pragma endregion

		void Process(string &rules, int &index, mat4t modelToWorld, mat4t &cameraToWorld, color_shader _shader, const float &angle = 45.0f)
		{
			char c = rules[index];
			while(index < rules.size() && c != ']')
			{

				c = rules[index++];

				switch(c)
				{
					case '+':
					{
						modelToWorld.rotateZ(angle);
						break;
					}
					case '-':
					{
						modelToWorld.rotateZ(-angle);
						break;
					}
					case 'F':
					{
						DrawLine(modelToWorld, cameraToWorld, _shader);
						modelToWorld.translate(0.0f, 1.0f, 0.0f);
						break;
					}
					case '[':
					{
						Process(rules, index, modelToWorld, cameraToWorld, _shader, angle);
						break;
					}
				}
			}
		}

		void Iterate (string &axiom, int iterations) {

			string newVariable;

			//printf("%s\n", axiom.c_str());

			for(int i = 0; i != iterations; ++i) {
				for(int v = 0; v != axiom.size(); ++v) {
					switch (axiom[v])
					{
						case 'F':
							newVariable += "F[+F]F[-F]F"; // "F[+F]F[-F]F";
							break;

						default:
							char c[2];
							c[0] = axiom[v];
							c[1] = '\0';
							newVariable += (string)c;
							break;
					}
				}
				axiom = newVariable;
				if(i != iterations-1)
					newVariable = "";
			}
		}
	};

}