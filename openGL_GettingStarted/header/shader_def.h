#pragma once
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n"
"layout (location = 1) in vec3 aColor;\n"

"out vec3 ourColor;\n"

"void main(){\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";



// define fragment shader (compute colors)
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n" // same name as in vertext shader => linked
"out vec4 FragColor;\n"

"void main(){\n"
"	FragColor = vec4(ourColor, 1.0);\n"
"}\0";