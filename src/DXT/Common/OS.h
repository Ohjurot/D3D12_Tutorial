#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12/d3dx12.h>

#ifdef DXT_DEBUG
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif

// OS Imported own stuff
#include <DXT/Common/ComPointer.h>
