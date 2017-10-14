#include "RenderSystem.h"

//Vertex Shader
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"layout ( location = 1 ) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"uniform vec3 translate;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
//"gl_Position = vec4(position.x, position.y, position.z, 1.0f) * transform * vec4(translate.x, translate.y, 1.0, 1.0);\n"
//"gl_Position = transform * vec4(position.x, position.y, position.z, 1.0f);\n"
//"gl_Position = transform * vec4(position.x, position.y, position.z, 1.0) + vec4(translate.x, translate.y, 0.0, 0.0);\n"
//"gl_Position = (transform * vec4(translate.x, translate.y, translate.z, 1.0f)) * vec4( position.x, position.y, position.z, 1.0 );\n"
//"gl_Position = transform * vec4( position.x + translate.x, position.y + translate.y, position.z, 1.0 );\n"
"gl_Position = vec4( position.x + translate.x, position.y + translate.y, position.z, 1.0 ) * transform;\n"
//"gl_Position = vec4( position.x + translate.x, position.y + translate.y, position.z, 1.0 );\n" //Render without scaling
"TexCoord = vec2( texCoord.x, 1.0f - texCoord.y);\n"
"}";

//Fragment Shader
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture1;\n"
"void main()\n"
"{\n"
"color = texture(ourTexture1, TexCoord);\n"
"}";

vector<string> gameObjectsToTest;


RenderSystem::RenderSystem(MessageBus* mbus) : System (mbus) {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	aspectRatio = (GLfloat)(WIDTH) / (GLfloat)(HEIGHT);
	//aspectRatio = 1.0f;
	window = SDL_CreateWindow("Okeanos - Made with Zephyr", RenderSystem::XSTART, RenderSystem::YSTART, RenderSystem::WIDTH, RenderSystem::HEIGHT, SDL_WINDOW_OPENGL);
	
	SDL_GL_SwapWindow(window);

	/*
	error = glGetError();
	if (error != 0) {
		const GLubyte* error2 = gluErrorString(error);
		int hit = 0;
	}
	*/
}


RenderSystem::~RenderSystem() {
}

void RenderSystem::init() {

	//TEMPORARY TEST OBJECTS
	//gameObjectsToTest.push_back("obj1,boatTest.png,1,1,0,0");
	gameObjectsToTest.push_back("obj2,boatTest.png,-1,-1,0,0");
	//gameObjectsToTest.push_back("obj3,boatTest.png,0,0,0,0");
	//gameObjectsToTest.push_back("obj3,boatTest.png,-1,0,0,0");
	//gameObjectsToTest.push_back("obj3,boatTest.png,0,1,0,0");


	//Setup window and context
	context = SDL_GL_CreateContext(window);
	//Setup SDL and GL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to init GLEW" << std::endl;
		return;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60 * aspectRatio, aspectRatio, 0.1, 100.0);

	//Make transparent background
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glewExperimental = GL_TRUE;
	glewInit();
	//Set up vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Set up fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		OutputDebugString("DIDNT WORK");
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Set up program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Set up VBOs, VAOs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Set up texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord), TexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::draw(string ID, string sprite, float x, float y, float z, float rotation) {

	//Bind translate to vertex shader
	GLint ourTranslate = glGetUniformLocation(shaderProgram, "translate");
	glUniform3f(ourTranslate, 0.5f * x, 0.5f * y, z);
	

	//Bind transform to vertex shader
	//Create a transform matrix and bind it to shader
	
	//Row major order
	// [0  1  2  3]
	// [4  5  6  7]
	// [8  9  10 11]
	// [12 13 14 15]

	//Column major order
	// [0  4  8  12]
	// [1  5  9  13]
	// [2  6  10 14]
	// [3  7  11 15]
	GLfloat transformMatrix[16];
	for (int i = 0; i < 16; i++) {
		transformMatrix[i] = 0.0f;
	}
	transformMatrix[0] = aspectRatio;
	transformMatrix[5] = aspectRatio;
	transformMatrix[10] = aspectRatio;

	/*
	//col major order
	transformMatrix[3] = 0.5f * x;
	transformMatrix[7] = 0.5f * y;
	transformMatrix[11] = z;
	*/
	/*
	//row major order
	transformMatrix[12] = 0.5f * x;
	transformMatrix[13] = 0.5f * y;
	transformMatrix[14] = z;
	*/

	transformMatrix[15] = 1.0f;
	/*OutputDebugString("\n");
	OutputDebugString(to_string(transformMatrix[0]).c_str());
	OutputDebugString(to_string(transformMatrix[1]).c_str());
	OutputDebugString(to_string(transformMatrix[2]).c_str());
	OutputDebugString(to_string(transformMatrix[3]).c_str());
	OutputDebugString(to_string(transformMatrix[4]).c_str());
	OutputDebugString(to_string(transformMatrix[5]).c_str());
	OutputDebugString(to_string(transformMatrix[6]).c_str());
	OutputDebugString(to_string(transformMatrix[7]).c_str());
	OutputDebugString(to_string(transformMatrix[8]).c_str());
	OutputDebugString(to_string(transformMatrix[9]).c_str());
	OutputDebugString(to_string(transformMatrix[10]).c_str());
	OutputDebugString(to_string(transformMatrix[11]).c_str());
	OutputDebugString(to_string(transformMatrix[12]).c_str());
	OutputDebugString(to_string(transformMatrix[13]).c_str());
	OutputDebugString(to_string(transformMatrix[14]).c_str());
	OutputDebugString(to_string(transformMatrix[15]).c_str());
	OutputDebugString("\n");*/
	
	GLint ourTransform = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(ourTransform, 1, GL_TRUE, transformMatrix);


	//Bind texture to fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getTexture(sprite));
	GLint ourTextureLocation = glGetUniformLocation(shaderProgram, "ourTexture1");
	glUniform1i(ourTextureLocation, 0);//Put GL_TEXTURE0 into ourTexture

	//Use shader
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	glBindVertexArray(0);

}

void RenderSystem::renderAllItems() {
	for (string* s : gameObjectsToRender) {
		//std::vector<std::string> data = split(s, ',');
		//OutputDebugString(s->c_str());
		//OutputDebugString("\n");
		renderObject(*s);
	}
	/*for (std::string s : gameObjectsToTest) {
		//std::vector<std::string> data = split(s, ',');
		//std::cout << s << "\n"; 
		renderObject(s);
	}*/
}

void RenderSystem::renderObject(string object) {
	//object format: ID,png,x,y,z,orientation
	string ID, sprite;
	float x, y, z, orientation;

	//Split object
	std::vector<string> objectData = split(object, ',');

	//Parse object data
	ID = objectData[0];
	sprite = objectData[1];
	x = atof(objectData[2].c_str());
	y = atof(objectData[3].c_str());
	z = atof(objectData[4].c_str());
	orientation = atof(objectData[5].c_str());

	//Draw object
	draw(ID, sprite, x, y, z, orientation);
}

GLuint RenderSystem::getTexture(string path) {
	OutputDebugString("trying to get texture: ");
	OutputDebugString(path.c_str());
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_Surface *temp = IMG_Load(path.c_str());

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	const char* sdlError = SDL_GetError();
	GLenum glError = glGetError();
	if (strlen(sdlError) > 0) {
		std::cout << sdlError << std::endl;
	}
	if (glError != GL_NO_ERROR) {
		std::cout << glError << std::endl;
	}

	SDL_FreeSurface(temp);
	glBindTexture(GL_TEXTURE_2D, 0);
	OutputDebugString("PASSED\n");
	return texture;
}

void RenderSystem::startSystemLoop() {
	init();
	running = true;
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;
	SDL_Event windowEvent;
	while (running) {
		thisTime = clock();
		if ((thisTime - lastTime) > timeFrame) {
			lastTime = thisTime;
			mtx.lock();
			//Clear the screen
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//Render all objects
			renderAllItems();
			//Update openGL window
			SDL_GL_SwapWindow(window);
			mtx.unlock();
		}
	}
}

void RenderSystem::stopSystemLoop() {

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	running = false;

}
void RenderSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case UPDATE_OBJECT_POSITION:
		mtx.lock();
		updateObjPosition(msg);
		mtx.unlock();
		break;
	case GO_ADDED:
		// parse data from msg to add obj to list
		mtx.lock();
		addObjectToRenderList(msg);
		mtx.unlock();
		break;
	case GO_REMOVED:
		mtx.lock();
		removeObjectFromRenderList(msg);
		mtx.unlock();
		break;
	default:
		break;
	}
}

void RenderSystem::removeObjectFromRenderList(Msg* m) {
	for (auto s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		// found the obj
		if (obj.front() == m->data) {
			// remove the object

			return;
		}
	}
	gameObjectsToRender.push_back(&m->data);
}

void RenderSystem::addObjectToRenderList(Msg* m) {
	gameObjectsToRender.push_back(&m->data);
}

void RenderSystem::updateObjPosition(Msg* m) {
	std::vector<std::string> data = split(m->data, ',');
	
	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		
		// found the obj
		if (obj.front() == data.front()) {
			// replace this string's information with new information
			OutputDebugString("updateObjPosition: ");
			OutputDebugString(s->c_str());
			OutputDebugString("  to  ");
			OutputDebugString(m->data.c_str());
			OutputDebugString("\n");
			*s = m->data;
			return;
		}

	}
}