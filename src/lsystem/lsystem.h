#include "lsystem\dirent.h";

namespace octet {

	class LSYS_File_Reader {
  
	
  private:
    FILE *file;
    char fileBuffer[1024];

  public:

		struct LSystem {
		 string name;
		 string axiom;
		 float angle;
		 int iterations;
		 int ruleCount;
		 string rulePrefix[10];
		 string ruleSuffix[10];

		 LSystem():
		 name(),
		 axiom(),
		 angle(0.0f),
		 iterations(0),
		 ruleCount(0)
		 {}
		};
		dynarray<LSystem> LSystems;

    LSYS_File_Reader ():
    file(NULL)
    {
      memset(fileBuffer, '\0', sizeof(fileBuffer));
    }

    void LS_ReadFile(FILE *file) {
      fseek(file, 0, SEEK_SET);
      int counter = 0;
      while(!feof(file)) {
        fileBuffer[counter++] = getc(file);
      }
      fclose(file);

			LS_ReadInstructions((string) fileBuffer);
    }

		void LS_ReadInstructions (string fileData)
		{
			LSystem ls;
			dynarray<string> fileDatas;
			dynarray<string> prefix;

			fileData.split(fileDatas, "\n");

			for(int i = 0; i != fileDatas.size(); ++i) {
				fileDatas[i].split(prefix, ":");
				if(prefix.size() <= 1) continue;
				LS_DataBreakdown(prefix[0], prefix[1], &ls);
			}
			LSystems.push_back(ls);
		}

		void LS_DataBreakdown (string a, string b, LSystem *ls) {
			if(!strcmp(a, "Name")) ls->name = b;
			if(!strcmp(a, "Axiom")) ls->axiom = b;
			if(!strcmp(a, "Iterations")) ls->iterations = atoi(b);
			if(!strcmp(a, "Angle")) ls->angle = (float)atof(b);
			
			if(!strcmp(a, "Rules")) {
				dynarray<string> ruleBufferTop;
				b.split(ruleBufferTop, ",");
				//TODO
				if(ruleBufferTop.size() > 1) {
					dynarray<string> ruleBufferBottom;

					ruleBufferTop[0].split(ruleBufferBottom, "=");
					ls->ruleCount++;
					ls->rulePrefix[ls->ruleCount] = ruleBufferBottom[0];
					ls->ruleSuffix[ls->ruleCount] = ruleBufferBottom[1];
				}
			}

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

	class LSYS_line {
		
		mat4t modelToWorld;
		float vertices[6];
		float worldVertices[6];

	public:
		
		LSYS_line(mat4t _modelToWorld, float length = 1.0f):
		modelToWorld(_modelToWorld)
		{
			vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f; 
			vertices[3] = 0.0f; vertices[4] = length; vertices[5] = 0.0f; 

			vec4 vertex = vec4(vertices[0], vertices[1], vertices[2], 1.0f);
			vertex = vertex * modelToWorld;
			worldVertices[0] = vertex.x(); worldVertices[1] = vertex.y(); worldVertices[2] = vertex.z();

			vertex = vec4(vertices[3], vertices[4], vertices[5], 1.0f);
			vertex = vertex * modelToWorld;
			worldVertices[3] = vertex.x(); worldVertices[4] = vertex.y(); worldVertices[5] = vertex.z();
		}

		void render(color_shader _shader, mat4t cameraToWorld, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			_shader.render(modelToProjection, color);

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			glDrawArrays(GL_LINES, 0, 2);
		}

		float *GetWorldVertices()
		{
			return worldVertices;
		}

	};

	class LSYS_tree {
		mat4t modelToWorld;
		float *vertices;
		int numOfLines;

		void ReadLine(int lineIndex, LSYS_line *line)
		{
			for(int i = 0; i < 6; i++)
			{
				vertices[i + (lineIndex*6)] = line->GetWorldVertices()[i];
			}
		}

	public:
		
		LSYS_tree(mat4t _modelToWorld):
		modelToWorld(_modelToWorld),
		vertices(NULL),
		numOfLines(0)
		{}

		void Init(int _numOfLines, LSYS_line **lines)
		{
			numOfLines = _numOfLines;
			vertices = (float*)allocator::realloc(vertices, 0, numOfLines*6*sizeof(float));

			for(int i = 0; i < numOfLines; i++)
			{
				ReadLine(i, lines[i]);
			}
		}

		void render(color_shader _shader, mat4t cameraToWorld, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			_shader.render(modelToProjection, color);

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			glDrawArrays(GL_LINES, 0, 2 * numOfLines);
		}

	};

	class LSYS_production_rules {

		LSYS_line **lines;
		int linesSize;
		int lineIndex;
		LSYS_tree *tree;

		void ExtendLines()
		{
			lines = (LSYS_line**)allocator::realloc(lines, sizeof(LSYS_line*)*linesSize, sizeof(LSYS_line*)*linesSize*2);
			linesSize *= 2;
		}

		public:

		LSYS_production_rules():
		lines(NULL),
		linesSize(0),
		lineIndex(0)
		{
			lines = (LSYS_line**)allocator::realloc(lines, sizeof(LSYS_line*)*linesSize, sizeof(LSYS_line*)*256);
			linesSize = 256;
		}

		void AddLine(mat4t modelToWorld, float size = 1.0f)
		{
			LSYS_line *line = new LSYS_line(modelToWorld, size);

			if(lineIndex >= linesSize) ExtendLines();

			lines[lineIndex++] = line;
		}

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
						//DrawLine(modelToWorld, cameraToWorld, _shader);
						AddLine(modelToWorld);
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

		void ProcessTree()
		{
			tree = new LSYS_tree(*new mat4t(1.0f));
			tree->Init(lineIndex, lines);
			CleanUp();
		}

		void CleanUp()
		{
			for(int i = lineIndex-1; i >= 0; i--)
			{
				delete lines[i];
			}
		}

		void RenderOne(mat4t cameraToWorld, color_shader _shader, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			tree->render(_shader, cameraToWorld, color);
		}

		void RenderAll(mat4t cameraToWorld, color_shader _shader, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			for(int i = 0; i < lineIndex; i++)
			{
				lines[i]->render(_shader, cameraToWorld, color);
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