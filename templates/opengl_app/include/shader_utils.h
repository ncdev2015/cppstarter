#pragma once
#include <string>

unsigned int compileShader(const std::string& source, unsigned int type);
unsigned int createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
