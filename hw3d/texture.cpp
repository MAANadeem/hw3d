#include "texture.h"

Texture::Texture(Graphics& gfx, const wchar_t* path) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    Microsoft::WRL::ComPtr<ID3D11Resource> texture;
    GFX_THROW(CreateWICTextureFromFile(
        GetDevice(gfx),
        GetContext(gfx),
        path,
        &texture,
        &textureView));
}

void Texture::Bind(Graphics& gfx) noexcept {
    GetContext(gfx)->PSSetShaderResources(0, 1, textureView.GetAddressOf());
}