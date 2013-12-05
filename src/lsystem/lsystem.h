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
				char c = getc(file);
				if(c == ' ') continue;
        fileBuffer[counter++] = (c == EOF) ? '\0' : c;
      }
      fclose(file);

			LS_ReadInstructions((string) fileBuffer);
    }

		void LS_ReadInstructions (string fileData)
		{
			LSystem *ls = new LSystem();
			dynarray<string> fileDatas;
			dynarray<string> prefix;

			fileData.split(fileDatas, "\n");

			for(int i = 0; i != fileDatas.size(); ++i) {
				fileDatas[i].split(prefix, ":");
				if(prefix.size() <= 1) continue;
				LS_DataBreakdown(prefix[0], prefix[1], ls);
			}
			LSystems.push_back(*ls);
		}

		void LS_DataBreakdown (string a, string b, LSystem *ls) {
			if(a == "Name") ls->name = b;
			else if(a == "Axiom") ls->axiom = b;
			else if(a == "Iterations") ls->iterations = atoi(b);
			else if(a == "Angle") ls->angle = (float)atof(b);
			else if(a == "Rules") {
				dynarray<string> ruleBufferTop;
				b.split(ruleBufferTop, ",");

				for(int i = 0; i != ruleBufferTop.size(); ++i) {
					dynarray<string> ruleBufferBottom;
					ruleBufferTop[i].split(ruleBufferBottom, "=");

					ls->rulePrefix[ls->ruleCount] = ruleBufferBottom[0];
					ls->ruleSuffix[ls->ruleCount] = ruleBufferBottom[1];
					ls->ruleCount++;
				}
			}
		}

		void LS_LoadLsystem (int i) {
			if(i > LSystems.size()) return;

			LS_LoadData(LSystems[i]);
		}

		void LS_LoadData(LSystem ls) {
			
		}

		void DisplayFileResults() {
			
			printf("%d L-System files exist.\n\n", LSystems.size());

			for(int i = 0; i != LSystems.size(); ++i) {
				
				if(i == 9) printf("\n");

				if(i < 9)
					printf("L-System  \"%s\"\tassigned to button %d\n", LSystems[i].name, i+1);
				else
					printf("To access L-System  \"%s\"\tuse left and right keys.\n", LSystems[i].name, i+1);
			}
			printf("\n");
		}
	};

	class LSYS_File_Finder {
		private:
		dynarray<FILE> LSYS_Files;
		string fileDirectory;

		public:
		LSYS_File_Reader *fr;

		LSYS_File_Finder(LSYS_File_Reader &_fr){
			fr = &_fr;
		}

		void LS_AssignFileLoc (const string &s) {
			fileDirectory = s;
			fileDirectory.truncate(fileDirectory.size()-10);
			fileDirectory += "configurations/";
			printf("FILE LOCATION = %s\n\n\n\n", fileDirectory);
		}

		void LS_LocateLsystemFiles () {
			printf("\nSearching for L-System Files in \"../../assets/lsystem/configurations\" ... \n");

			//LSYS_File_Reader *fr = new LSYS_File_Reader();
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
							fr->LS_ReadFile(file);
						}
					}
				}
				closedir (dir);
			} else {
				/* could not open directory */
				printf("No Directory Found - Trying Local Folder\n");
				LS_LocateLsystemFilesBackup();
			}
			fr->DisplayFileResults();
		}

		void LS_LocateLsystemFilesBackup () {
			printf("\nSearching for L-System Files in .exe folder ... \n");

			//LSYS_File_Reader *fr = new LSYS_File_Reader();
			DIR *dir;
			struct dirent *ent;

			if ((dir = opendir (fileDirectory)) != NULL) {
				while ((ent = readdir (dir)) != NULL) {
					FILE *file = NULL;
					string fileLocation = fileDirectory;
					fileLocation += ent->d_name;
					
					file = fopen(fileLocation, "r");
					if(file != NULL) {
						printf("Located File: %s ... ", ent->d_name);
						
						if(File_isLsystem(file)) {
							fr->LS_ReadFile(file);
						}
					}
				}
				closedir (dir);
			} else {
				/* could not open directory */
				printf("CANT FIND ANY CONFIG FILES\n");
			}
			fr->DisplayFileResults();
		}

		//std::string ExePath() {
		//	char bufferX[MAX_PATH];
		//	GetModuleFileName( NULL, bufferX, MAX_PATH );
		//	std::string::size_type pos = std::string( bufferX ).find_last_of( "\\/" );
		//	return std::string( bufferX ).substr( 0, pos);
		//}



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
			printf("DECLINED! - File Not L-System!\n");
			return false;
		}
	};

	class LSYS_Line {
		
		mat4t modelToWorld;
		float vertices[6];
		float worldVertices[6];

	public:
		
		LSYS_Line(mat4t _modelToWorld,  float length = 1.0f):
		modelToWorld(_modelToWorld)
		{
			vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f; 
			vertices[3] = 0.0f; vertices[4] = length; vertices[5] = 0.0f; 

			vec4 vertex = vec4(vertices[0], vertices[1], vertices[2], 1.0f);
			vertex = vertex * modelToWorld;
			worldVertices[0] = vertex.x(); worldVertices[1] = vertex.y(); worldVertices[2] = vertex.z();
			//

			vertex = vec4(vertices[3], vertices[4], vertices[5], 1.0f);
			vertex = vertex * modelToWorld;
			worldVertices[3] = vertex.x(); worldVertices[4] = vertex.y(); worldVertices[5] = vertex.z();
			//
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

	class LSYS_Tree {
		mat4t modelToWorld;
		float *vertices;
		int numOfLines;

		void ReadLine(int lineIndex, LSYS_Line *line)
		{
			for(int i = 0; i < 6; i++)
			{
				vertices[i + (lineIndex*6)] = line->GetWorldVertices()[i];
				if(i == 1 || i == 4) {
					float y = vertices[i + (lineIndex*6)];
					highestY = max<float>(highestY, y);
					lowestY = min<float>(lowestY, y);
				}

				if(i == 0 || i == 3) {
					float x = vertices[i + (lineIndex*6)];
					highestX = max<float>(highestX, x);
					lowestX = min<float>(lowestX, x);
				}
			}
		}

	public:

		float highestY, lowestY;
		float highestX, lowestX;
		
		LSYS_Tree(mat4t _modelToWorld):
		modelToWorld(_modelToWorld),
		vertices(NULL),
		numOfLines(0),
		highestY(-999999.0f),
		lowestY(999999.0f)
		{}

		void Init(int _numOfLines, LSYS_Line **lines)
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

	class LSYS_Production_Rules {

		LSYS_Line **lines;
		LSYS_Tree *tree;
		int linesSize;
		int lineIndex;
		int currentIteration;
		LSYS_File_Reader::LSystem *ls;
		float highestY, lowestY;
		float highestX, lowestX;
		
		void ExtendLines()
		{
			lines = (LSYS_Line**)allocator::realloc(lines, sizeof(LSYS_Line*)*linesSize, sizeof(LSYS_Line*)*linesSize*2);
			linesSize *= 2;
		}

		public:

		string axiom;

		LSYS_Production_Rules():
		lines(NULL),
		linesSize(0),
		lineIndex(0),
		currentIteration(0)
		{
			lines = (LSYS_Line**)allocator::realloc(lines, sizeof(LSYS_Line*)*linesSize, sizeof(LSYS_Line*)*256);
			linesSize = 256;
		}

		void AssignLSystem (LSYS_File_Reader::LSystem *_ls) {
			ls = _ls;
		}

		void AddLine(mat4t modelToWorld, float size = 1.0f)
		{
			LSYS_Line *line = new LSYS_Line(modelToWorld, size);

			if(lineIndex >= linesSize) ExtendLines();

			lines[lineIndex++] = line;
		}

		#pragma region DrawLine
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
		
		void Process(string &rules, int &index, mat4t modelToWorld, mat4t &cameraToWorld, color_shader _shader)
		{
			Process(rules, index, modelToWorld, cameraToWorld, _shader, ls->angle);
		}

		void Process(string &rules, int &index, mat4t modelToWorld, mat4t &cameraToWorld, color_shader _shader, const float &angle)
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
						AddLine(modelToWorld);
						modelToWorld.translate(0.0f, 1.0f, 0.0f);
						break;
					}
					case '[':
					{
						Process(rules, index, modelToWorld, cameraToWorld, _shader, angle);
						break;
					}
					case 'X':
					{
						AddLine(modelToWorld);
						modelToWorld.translate(0.0f, 1.0f, 0.0f);
						break;
					}
				}
			}
		}

		void ProcessTree(float &treeHeight, float &treeWidth)
		{
			tree = new LSYS_Tree(*new mat4t(1.0f));
			tree->Init(lineIndex, lines);
			CleanUp();
			lineIndex = 0;

			treeWidth = abs(tree->highestX - tree->highestX);
			treeHeight = abs(tree->highestY - tree->lowestY);
			printf("\t-\t Draw Complete\n");
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

		void Iterate (int count = -1) {
			
			axiom = ls->axiom;
			int axiomSize = axiom.size();
			string newVariable;

			int iterations = 0;
			if(count < 0)
				iterations = ls->iterations;
			else
				iterations = count;

			int j;
			for(int i = 0; i != iterations; ++i) {
				for(int v = 0; v != axiomSize; ++v) {

					char c[2];
					c[0] = axiom[v];
					c[1] = '\0';
					string temp = (string)c;

					for(j = 0; j != ls->ruleCount; ++j) {
						if(temp == ls->rulePrefix[j]) {
							newVariable += ls->ruleSuffix[j];
							break;
						}
					}
					if(j == ls->ruleCount)	newVariable += temp;

				}
				axiom = newVariable;
				axiomSize = axiom.size();
				if(i != iterations-1)
					newVariable = "";
			}



			currentIteration = ls->iterations;
			//printf(axiom);
			//printf("Size = %d\n",axiom.size());
			printf("Current Ireration of tree \"%s\" is %d", ls->name, currentIteration);
		}

		void ReIterate () {
			
			int axiomSize = axiom.size();
			string newVariable;

			//if(currentIteration > 6) return;

			int j;
			for(int i = 0; i != 1; ++i) {
				for(int v = 0; v != axiomSize; ++v) {

					char c[2];
					c[0] = axiom[v];
					c[1] = '\0';
					string temp = (string)c;

					for(j = 0; j != ls->ruleCount; ++j) {
						if(temp == ls->rulePrefix[j]) {
							newVariable += ls->ruleSuffix[j];
							break;
						}
					}
					if(j == ls->ruleCount)	newVariable += temp;

				}
				axiom = newVariable;
				axiomSize = axiom.size();
				if(i != 0)
					newVariable = "";
			}



			currentIteration++;
			//printf("Size = %d\n",axiom.size());
			printf("Current Ireration of tree \"%s\" is %d", ls->name, currentIteration);
		}

		void DeIterate () {
			
			if(currentIteration == 1) return;
			int iterations = currentIteration-1;

			axiom = ls->axiom;
			int axiomSize = axiom.size();
			string newVariable;

			int j;
			for(int i = 0; i != iterations; ++i) {
				for(int v = 0; v != axiomSize; ++v) {

					char c[2];
					c[0] = axiom[v];
					c[1] = '\0';
					string temp = (string)c;

					for(j = 0; j != ls->ruleCount; ++j) {
						if(temp == ls->rulePrefix[j]) {
							newVariable += ls->ruleSuffix[j];
							break;
						}
					}
					if(j == ls->ruleCount)	newVariable += temp;

				}
				axiom = newVariable;
				axiomSize = axiom.size();
				if(i != iterations-1)
					newVariable = "";
			}

			currentIteration--;
			//system("cls");
			//printf(axiom);
			//printf("Size = %d\n",axiom.size());
			printf("Current Ireration of tree \"%s\" is %d", ls->name, currentIteration);
		}

		void DisplayLSystemDraw () {
			printf("Current Ireration of tree \"%s\" is %d\n", ls->name, currentIteration);
		}
	};
}