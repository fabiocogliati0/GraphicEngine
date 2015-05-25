#pragma once

#include "InputLayoutElementDescriptor.h"

#include <Windows.h>

struct VertexShaderDescriptor
{
	LPCWSTR FileName;
	InputLayoutElementDescriptor* InputLayoutElementsDescriptors;
	unsigned int InputLayoutElementsNumber;
};