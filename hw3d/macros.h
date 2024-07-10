#pragma once

#define GFX_THROW(hresult) if (FAILED(hr = (hresult))) throw Graphics::Exception(__LINE__, __FILE__, hr)
#define STARTUP() HRESULT hr;