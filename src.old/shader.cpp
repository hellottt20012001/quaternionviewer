#include <shader.h>

int ShaderProgram::loadShader(const char* shader_filepath, GLuint &shader)
{
    // Read shader code
    std::string shaderCode = "";
	std::ifstream shaderStream(shader_filepath, std::ios::in);
	if(shaderStream.is_open())
    {
		std::string Line = "";
		while(getline(shaderStream, Line)) shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else { printf("Failed to open %s.\n", shader_filepath); getchar(); return -1;}

    // Compile shader
	//printf("Compiling shader: %s\n", shader_filepath);
	char const* source = shaderCode.c_str();
	glShaderSource(shader, 1, &source , NULL);
	glCompileShader(shader);

	// Check shader
	GLint result = GL_FALSE; int infoLogLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
    {
		std::vector<char> shaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
		return -2;
	}

	return 0;
}

int ShaderProgram::loadProgram(const char * vertex_filepath, const char * fragment_filepath)
{
	// Create the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// load code into shader
	loadShader(vertex_filepath, vertexShader);
	loadShader(fragment_filepath, fragmentShader);

	// Link the program
	//printf("Linking program\n");
	Program = glCreateProgram();
	glAttachShader(Program, vertexShader);
	glAttachShader(Program, fragmentShader);
	glLinkProgram(Program);

	// Check the program
	GLint result = GL_FALSE; int infoLogLength;
	glGetProgramiv(Program, GL_LINK_STATUS, &result);
	glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
    {
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(Program, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return 0;
}
