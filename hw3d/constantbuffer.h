#pragma once
#include "bindable.h"

template <typename C>
class ConstantBuffer : public Bindable {
public:
    // First constructor
    ConstantBuffer(Graphics& gfx, const char type)
        : type(type) {
        STARTUP();
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(C);
        cbd.StructureByteStride = 0u;
        GFX_THROW(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &constantBuffer));
    }

    // Second constructor
    ConstantBuffer(Graphics& gfx, const char type, const C& consts)
        : type(type) {
        STARTUP();
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(consts);
        cbd.StructureByteStride = 0u;
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = &consts;
        GFX_THROW(GetDevice(gfx)->CreateBuffer(&cbd, &sd, &constantBuffer));
    }

    // Update method
    void Update(Graphics& gfx, const C& consts) {
        STARTUP();
        D3D11_MAPPED_SUBRESOURCE msr;
        GFX_THROW(GetContext(gfx)->Map(
            constantBuffer.Get(), 0u,
            D3D11_MAP_WRITE_DISCARD, 0u,
            &msr
        ));
        memcpy(msr.pData, &consts, sizeof(consts));
        GetContext(gfx)->Unmap(constantBuffer.Get(), 0u);
    }

    // Bind method
    void Bind(Graphics& gfx) noexcept override {
        switch (type) {
        case 'p':
            GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
            break;
        case 'v':
            GetContext(gfx)->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
            break;
        }
    }

private:
    const char type;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

