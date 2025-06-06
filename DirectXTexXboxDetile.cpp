//--------------------------------------------------------------------------------------
// File: DirectXTexXboxDetile.cpp
//
// DirectXTex Auxilary functions for converting from Xbox tiled to linear
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#include "DirectXTexP.h"
#include "DirectXTexXbox.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::Internal;
using namespace Xbox;

namespace
{
    //----------------------------------------------------------------------------------
    inline HRESULT DetileByElement1D(
        const XboxImage& xbox,
        uint32_t level,
        _In_ XGTextureAddressComputer* computer,
        const XG_RESOURCE_LAYOUT& layout,
        _In_reads_(nimages) const Image* const * result,
        size_t nimages,
        size_t bpp,
        size_t w,
        bool packed)
    {
        const uint8_t* sptr = xbox.GetPointer();
        const uint8_t* endPtr = sptr + layout.SizeBytes;

        for (uint32_t item = 0; item < nimages; ++item)
        {
            const Image* img = result[item];
            if (!img || !img->pixels)
                return E_POINTER;

            assert(img->width == result[0]->width);
            assert(img->height == result[0]->height);
            assert(img->rowPitch == result[0]->rowPitch);
            assert(img->format == result[0]->format);

            uint8_t* dptr = img->pixels;

            for (size_t x = 0; x < w; ++x)
            {
            #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
                const UINT64 element = (packed) ? (x >> 1) : x;
                const size_t offset = computer->GetTexelElementOffsetBytes(0, level, element, 0, item, 0, nullptr);
            #else
                const size_t offset = computer->GetTexelElementOffsetBytes(0, level, x, 0, item, 0);
            #endif
                if (offset == size_t(-1))
                    return E_FAIL;

                const uint8_t* src = sptr + offset;

                if ((src + bpp) > endPtr)
                    return E_FAIL;

                memcpy(dptr, src, bpp);
                dptr += bpp;

                if (packed)
                    ++x;
            }
        }

        return S_OK;
    }

    //-------------------------------------------------------------------------------------
    // 1D Tiling
    //-------------------------------------------------------------------------------------
    HRESULT Detile1D(
        const XboxImage& xbox,
        uint32_t level,
        _In_ XGTextureAddressComputer* computer,
        const XG_RESOURCE_LAYOUT& layout,
        _In_reads_(nimages) const Image** result,
        size_t nimages)
    {
        if (!nimages)
            return E_INVALIDARG;

        if (!xbox.GetPointer() || !computer || !result || !result[0])
            return E_POINTER;

        assert(layout.Planes == 1);

        const DXGI_FORMAT format = result[0]->format;

        assert(format == xbox.GetMetadata().format);

        assert(!IsCompressed(format));

        bool byelement = IsTypeless(format);
    #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
        if (nimages > 1)
            byelement = true;
    #endif

        if (IsPacked(format))
        {
            const size_t bpp = (BitsPerPixel(format) + 7) / 8;

            // XG (XboxOne) incorrectly returns 2 instead of 4 here for layout.Plane[0].BytesPerElement

            const size_t w = result[0]->width;
            assert(((w + 1) / 2) == layout.Plane[0].MipLayout[level].WidthElements);

            return DetileByElement1D(xbox, level, computer, layout, result, nimages, bpp, w, true);
        }
        else if (byelement)
        {
            //--- Typeless is done with per-element copy ----------------------------------
            const size_t bpp = (BitsPerPixel(format) + 7) / 8;
            assert(bpp == layout.Plane[0].BytesPerElement);

            const size_t w = result[0]->width;
            assert(w == layout.Plane[0].MipLayout[level].WidthElements);

            return DetileByElement1D(xbox, level, computer, layout, result, nimages, bpp, w, false);
        }
        else
        {
            //--- Standard format handling ------------------------------------------------
            auto& mip = layout.Plane[0].MipLayout[level];

            const UINT32 tiledPixels = mip.PitchPixels * mip.PaddedDepthOrArraySize;

            auto scanline = make_AlignedArrayXMVECTOR(tiledPixels + result[0]->width);

            XMVECTOR* target = scanline.get();
            XMVECTOR* tiled = target + result[0]->width;

        #ifdef _DEBUG
            memset(target, 0xCD, sizeof(XMVECTOR) * result[0]->width);
            memset(tiled, 0xDD, sizeof(XMVECTOR) * tiledPixels);
        #endif

                    // Load tiled texture
            if ((xbox.GetSize() - mip.OffsetBytes) < mip.SizeBytes)
                return E_FAIL;

            if (!LoadScanline(tiled, tiledPixels, xbox.GetPointer() + mip.OffsetBytes, mip.SizeBytes, xbox.GetMetadata().format))
                return E_FAIL;

            // Perform detiling
            for (uint32_t item = 0; item < nimages; ++item)
            {
                const Image* img = result[item];
                if (!img || !img->pixels)
                    return E_POINTER;

                assert(img->width == result[0]->width);
                assert(img->height == result[0]->height);
                assert(img->rowPitch == result[0]->rowPitch);
                assert(img->format == result[0]->format);

                for (size_t x = 0; x < img->width; ++x)
                {
                #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
                    size_t offset = computer->GetTexelElementOffsetBytes(0, level, x, 0, item, 0, nullptr);
                #else
                    size_t offset = computer->GetTexelElementOffsetBytes(0, level, x, 0, item, 0);
                #endif
                    if (offset == size_t(-1))
                        return E_FAIL;

                    assert(offset >= mip.OffsetBytes);
                    assert(offset < mip.OffsetBytes + mip.SizeBytes);

                    offset = (offset - mip.OffsetBytes) / layout.Plane[0].BytesPerElement;
                    assert(offset < tiledPixels);

                    target[x] = tiled[offset];
                }

                if (!StoreScanline(img->pixels, img->rowPitch, img->format, target, img->width))
                    return E_FAIL;
            }
        }

        return S_OK;
    }


    //-------------------------------------------------------------------------------------
    // 2D Tiling
    //-------------------------------------------------------------------------------------
    HRESULT Detile2D(
        const XboxImage& xbox,
        uint32_t level,
        _In_ XGTextureAddressComputer* computer,
        _In_reads_(nimages) const Image** result,
        size_t nimages)
    {
        if (!nimages)
            return E_INVALIDARG;

        if (!xbox.GetPointer() || !computer || !result || !result[0])
            return E_POINTER;

        assert(xbox.GetMetadata().format == result[0]->format);

        uint8_t* baseAddr = xbox.GetPointer();
        const auto& metadata = xbox.GetMetadata();

        for (uint32_t item = 0; item < nimages; ++item)
        {
            const Image* img = result[item];
            if (!img || !img->pixels)
                return E_POINTER;

            assert(img->width == result[0]->width);
            assert(img->height == result[0]->height);
            assert(img->rowPitch == result[0]->rowPitch);
            assert(img->format == result[0]->format);

            HRESULT hr = computer->CopyFromSubresource(
                img->pixels,
                0u,
                metadata.CalculateSubresource(level, item),
            #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
                0u,
            #endif
                baseAddr,
                static_cast<UINT32>(img->rowPitch),
                0u);
            if (FAILED(hr))
            {
                return hr;
            }
        }

        return S_OK;
    }


    //-------------------------------------------------------------------------------------
    // 3D Tiling
    //-------------------------------------------------------------------------------------
    HRESULT Detile3D(
        const XboxImage& xbox,
        uint32_t level,
        _In_ XGTextureAddressComputer* computer,
        const Image& result)
    {
        if (!computer || !xbox.GetPointer() || !result.pixels)
            return E_POINTER;

        assert(xbox.GetMetadata().format == result.format);

        uint8_t* baseAddr = xbox.GetPointer();
        const auto& metadata = xbox.GetMetadata();

        return computer->CopyFromSubresource(
            result.pixels,
            0u,
            metadata.CalculateSubresource(level, 0),
        #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
            0u,
        #endif
            baseAddr,
            static_cast<UINT32>(result.rowPitch),
            static_cast<UINT32>(result.slicePitch));
    }
}

//=====================================================================================
// Entry-points
//=====================================================================================

//-------------------------------------------------------------------------------------
// Detile image
//-------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT Xbox::Detile(
    const XboxImage& xbox,
    DirectX::ScratchImage& image)
{
    if (!xbox.GetSize() || !xbox.GetPointer() || xbox.GetTileMode() == c_XboxTileModeInvalid)
        return E_INVALIDARG;

    image.Release();

    auto& metadata = xbox.GetMetadata();

    if (metadata.format == DXGI_FORMAT_R1_UNORM
        || IsVideo(metadata.format))
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    switch (metadata.format)
    {
    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

    default:
        break;
    }

    XG_RESOURCE_LAYOUT layout = {};

    switch (metadata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
        {
            XG_TEXTURE1D_DESC desc = {};
            desc.Width = static_cast<UINT>(metadata.width);
            desc.MipLevels = static_cast<UINT>(metadata.mipLevels);
            desc.ArraySize = static_cast<UINT>(metadata.arraySize);
            desc.Format = static_cast<XG_FORMAT>(metadata.format);
            desc.Usage = XG_USAGE_DEFAULT;
            desc.BindFlags = XG_BIND_SHADER_RESOURCE;
            desc.MiscFlags = (metadata.IsCubemap()) ? XG_RESOURCE_MISC_TEXTURECUBE : 0;
        #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
            desc.SwizzleMode = xbox.GetTileMode();
        #else
            desc.TileMode = xbox.GetTileMode();
        #endif

            ComPtr<XGTextureAddressComputer> computer;
            HRESULT hr = XGCreateTexture1DComputer(&desc, computer.GetAddressOf());
            if (FAILED(hr))
                return hr;

            hr = computer->GetResourceLayout(&layout);
            if (FAILED(hr))
                return hr;

            if (layout.Planes != 1)
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

            if (layout.SizeBytes != xbox.GetSize()
                || layout.BaseAlignmentBytes != xbox.GetAlignment())
                return E_UNEXPECTED;

            hr = image.Initialize(metadata);
            if (FAILED(hr))
                return hr;

            for (uint32_t level = 0; level < metadata.mipLevels; ++level)
            {
                if (metadata.arraySize > 1)
                {
                    std::vector<const Image*> images;
                    images.reserve(metadata.arraySize);
                    for (uint32_t item = 0; item < metadata.arraySize; ++item)
                    {
                        const Image* img = image.GetImage(level, item, 0);
                        if (!img)
                        {
                            image.Release();
                            return E_FAIL;
                        }

                        images.push_back(img);
                    }

                    hr = Detile1D(xbox, level, computer.Get(), layout, &images[0], images.size());
                }
                else
                {
                    const Image* img = image.GetImage(level, 0, 0);
                    if (!img)
                    {
                        image.Release();
                        return E_FAIL;
                    }

                    hr = Detile1D(xbox, level, computer.Get(), layout, &img, 1);
                }

                if (FAILED(hr))
                {
                    image.Release();
                    return hr;
                }
            }
        }
        break;

    case TEX_DIMENSION_TEXTURE2D:
        {
            XG_TEXTURE2D_DESC desc = {};
            desc.Width = static_cast<UINT>(metadata.width);
            desc.Height = static_cast<UINT>(metadata.height);
            desc.MipLevels = static_cast<UINT>(metadata.mipLevels);
            desc.ArraySize = static_cast<UINT>(metadata.arraySize);
            desc.Format = static_cast<XG_FORMAT>(metadata.format);
            desc.SampleDesc.Count = 1;
            desc.Usage = XG_USAGE_DEFAULT;
            desc.BindFlags = XG_BIND_SHADER_RESOURCE;
            desc.MiscFlags = (metadata.miscFlags & TEX_MISC_TEXTURECUBE) ? XG_RESOURCE_MISC_TEXTURECUBE : 0;
        #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
            desc.SwizzleMode = xbox.GetTileMode();
        #else
            desc.TileMode = xbox.GetTileMode();
        #endif

            ComPtr<XGTextureAddressComputer> computer;
            HRESULT hr = XGCreateTexture2DComputer(&desc, computer.GetAddressOf());
            if (FAILED(hr))
                return hr;

            hr = computer->GetResourceLayout(&layout);
            if (FAILED(hr))
                return hr;

            if (layout.Planes != 1)
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

            if (layout.SizeBytes != xbox.GetSize()
                || layout.BaseAlignmentBytes != xbox.GetAlignment())
                return E_UNEXPECTED;

            hr = image.Initialize(metadata);
            if (FAILED(hr))
                return hr;

            for (uint32_t level = 0; level < metadata.mipLevels; ++level)
            {
                if (metadata.arraySize > 1)
                {
                    std::vector<const Image*> images;
                    images.reserve(metadata.arraySize);
                    for (uint32_t item = 0; item < metadata.arraySize; ++item)
                    {
                        const Image* img = image.GetImage(level, item, 0);
                        if (!img)
                        {
                            image.Release();
                            return E_FAIL;
                        }

                        images.push_back(img);
                    }

                    hr = Detile2D(xbox, level, computer.Get(), &images[0], images.size());
                }
                else
                {
                    const Image* img = image.GetImage(level, 0, 0);
                    if (!img)
                    {
                        image.Release();
                        return E_FAIL;
                    }

                    hr = Detile2D(xbox, level, computer.Get(), &img, 1);
                }

                if (FAILED(hr))
                {
                    image.Release();
                    return hr;
                }
            }
        }
        break;

    case TEX_DIMENSION_TEXTURE3D:
        {
            XG_TEXTURE3D_DESC desc = {};
            desc.Width = static_cast<UINT>(metadata.width);
            desc.Height = static_cast<UINT>(metadata.height);
            desc.Depth = static_cast<UINT>(metadata.depth);
            desc.MipLevels = static_cast<UINT>(metadata.mipLevels);
            desc.Format = static_cast<XG_FORMAT>(metadata.format);
            desc.Usage = XG_USAGE_DEFAULT;
            desc.BindFlags = XG_BIND_SHADER_RESOURCE;
        #if defined(_GAMING_XBOX_SCARLETT) || defined(_USE_SCARLETT)
            desc.SwizzleMode = xbox.GetTileMode();
        #else
            desc.TileMode = xbox.GetTileMode();
        #endif

            ComPtr<XGTextureAddressComputer> computer;
            HRESULT hr = XGCreateTexture3DComputer(&desc, computer.GetAddressOf());
            if (FAILED(hr))
                return hr;

            hr = computer->GetResourceLayout(&layout);
            if (FAILED(hr))
                return hr;

            if (layout.Planes != 1)
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

            if (layout.SizeBytes != xbox.GetSize()
                || layout.BaseAlignmentBytes != xbox.GetAlignment())
                return E_UNEXPECTED;

            hr = image.Initialize(metadata);
            if (FAILED(hr))
                return hr;

            uint32_t d = static_cast<uint32_t>(metadata.depth);

            size_t index = 0;
            for (uint32_t level = 0; level < metadata.mipLevels; ++level)
            {
                if ((index + d) > image.GetImageCount())
                {
                    image.Release();
                    return E_FAIL;
                }

                // Relies on the fact that slices are contiguous
                hr = Detile3D(xbox, level, computer.Get(), image.GetImages()[index]);
                if (FAILED(hr))
                {
                    image.Release();
                    return hr;
                }

                index += d;

                if (d > 1)
                    d >>= 1;
            }
        }
        break;

    default:
        return E_FAIL;
    }

    return S_OK;
}
