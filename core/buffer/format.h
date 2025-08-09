#pragma once

#include <stdint.h>
#include <pre.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Where is the buffer and how reads or writes are behaviour
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class buffer_location_t
{
    UNKNOWN = 0,
    HOST = 1,
    AUDIO = 2,
    GPU = 3,
    VIDEO = 4
};

enum class buffer_operation_frequency_t
{
    NEVER = 0,
    ONCE = 1,
    ONCE_EVERY_MANY_FRAMES = 2,
    EVERY_FRAME = 3,
    MANY_TIMES_PER_FRAME = 4,
    ALWAYS = 5
};

enum class buffer_usage_t
{
    UNKNOWN = 0,

    TRANSFER_SRC = 1 << 1,
    TRANSFER_DST = 1 << 2,
    TRANSFORM_FEEDBACK_BUFFER = 1 << 3,
    TRANSFORM_FEEDBACK_COUNTER_BUFFER = 1 << 4,

    SAMPLED = 1 << 5,

    UNIFORM_BUFFER = 1 << 8,
    STORAGE_BUFFER = 1 << 9,
    INDEX_BUFFER = 1 << 10,
    VERTEX_BUFFER = 1 << 11,
    INDIRECT_BUFFER = 1 << 12,
    TEXTURE = 1 << 13,

    COLOR_RENDER_TARGET = 1 << 16,
    DEPTH_STENCIL_RENDER_TARGET = 1 << 17,

    DECODE_SRC = 1 << 24,
    DECODE_DST = 1 << 25,
    DECODE_DPB = 1 << 26,
    ENCODE_DST = 1 << 27,
    ENCODE_SRC = 1 << 28,
    ENCODE_DPB = 1 << 29
};
//PRE_ENUM_AND_OR(buffer_usage_t)


struct buffer_access_t
{
    buffer_location_t location : 3;
    buffer_operation_frequency_t host_write : 3;
    buffer_operation_frequency_t host_read : 3;
    buffer_operation_frequency_t host_map_for_write : 3;
    buffer_operation_frequency_t host_map_for_read : 3;
    buffer_operation_frequency_t process_unit_write : 3;
    buffer_operation_frequency_t process_unit_read : 3;
    bool can_use_subregion : 1;
    buffer_usage_t usage;
};





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// How is the buffer readed when access to an element
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class sampler_read_t
{
    NEAREST = 0,
    LINEAR = 1,
    NONLINEAR = 3
};

enum class sampler_wrap_mode_t
{
    REPEAT = 0,
    MIRROT = 1
};

struct buffer_sampler_t
{
    sampler_read_t min_filter : 3;
    sampler_read_t mag_filter : 3;
    sampler_read_t mip_filter : 3;
    sampler_wrap_mode_t wrap_x : 3;
    sampler_wrap_mode_t wrap_y : 3;
    sampler_wrap_mode_t wrap_z : 3;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// How is the buffer readed when access to an element
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class buffer_layout_t
{
    SINGLE_ELEMENT = 0,
    LAYOUT_1D = 1,
    LAYOUT_2D = 2,
    LAYOUT_3D = 3
};

enum class buffer_object_layout_t
{
    SINGLE_ELEMENT = 0,
    LAYOUT_1D = 1,
    LAYOUT_2D = 2,
    LAYOUT_3D = 3,
    CUBE = 4
};

enum class buffer_element_layout_t
{
    UNKNOWN = 0,
    SINGLE_ELEMENT = 1,
    VECTOR_2 = 2,
    VECTOR_3 = 3,
    VECTOR_4 = 4,
    MATRIX_4x4 = 5
};

enum class number_format_t
{
    UNKNOWN = 0,
    INTEGER = 1,
    FLOAT = 2,
    FIXED = 3
};

enum class number_compressed_format_t
{
    UNDEFINED,
    R4G4_UNORM_PACK8,
    R4G4B4A4_UNORM_PACK16,
    B4G4R4A4_UNORM_PACK16,
    R5G6B5_UNORM_PACK16,
    B5G6R5_UNORM_PACK16,
    R5G5B5A1_UNORM_PACK16,
    B5G5R5A1_UNORM_PACK16,
    A1R5G5B5_UNORM_PACK16,
    A8B8G8R8_UNORM_PACK32,
    A8B8G8R8_SNORM_PACK32,
    A8B8G8R8_USCALED_PACK32,
    A8B8G8R8_SSCALED_PACK32,
    A8B8G8R8_UINT_PACK32,
    A8B8G8R8_SINT_PACK32,
    A8B8G8R8_SRGB_PACK32,
    A2R10G10B10_UNORM_PACK32,
    A2R10G10B10_SNORM_PACK32,
    A2R10G10B10_USCALED_PACK32,
    A2R10G10B10_SSCALED_PACK32,
    A2R10G10B10_UINT_PACK32,
    A2R10G10B10_SINT_PACK32,
    A2B10G10R10_UNORM_PACK32,
    A2B10G10R10_SNORM_PACK32,
    A2B10G10R10_USCALED_PACK32,
    A2B10G10R10_SSCALED_PACK32,
    A2B10G10R10_UINT_PACK32,
    A2B10G10R10_SINT_PACK32,
    B10G11R11_UFLOAT_PACK32,
    E5B9G9R9_UFLOAT_PACK32,
    X8_D24_UNORM_PACK32,
    G8B8G8R8_422_UNORM,
    B8G8R8G8_422_UNORM,
    G8_B8_R8_3PLANE_420_UNORM,
    G8_B8R8_2PLANE_420_UNORM,
    G8_B8_R8_3PLANE_422_UNORM,
    G8_B8R8_2PLANE_422_UNORM,
    G8_B8_R8_3PLANE_444_UNORM,
    R10X6_UNORM_PACK16,
    R10X6G10X6_UNORM_2PACK16,
    R10X6G10X6B10X6A10X6_UNORM_4PACK16,
    G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
    B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
    G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
    G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
    G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
    G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
    G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
    R12X4_UNORM_PACK16,
    R12X4G12X4_UNORM_2PACK16,
    R12X4G12X4B12X4A12X4_UNORM_4PACK16,
    G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
    B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
    G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
    G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
    G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
    G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
    G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
    G16B16G16R16_422_UNORM,
    B16G16R16G16_422_UNORM,
    G16_B16_R16_3PLANE_420_UNORM,
    G16_B16R16_2PLANE_420_UNORM,
    G16_B16_R16_3PLANE_422_UNORM,
    G16_B16R16_2PLANE_422_UNORM,
    G16_B16_R16_3PLANE_444_UNORM,
    G8_B8R8_2PLANE_444_UNORM,
    G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
    G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
    G16_B16R16_2PLANE_444_UNORM,
    A4R4G4B4_UNORM_PACK16,
    A4B4G4R4_UNORM_PACK16
};

enum class number_bit_count_t
{
    BC_8 = 0,
    BC_16 = 1,
    BC_32 = 2,
    BC_64 = 3,
    BC_128 = 4,
    BC_256 = 5,
    BC_512 = 6,
    BC_1024 = 7,
};

enum class color_space_t
{
    UNDEFINED = 0,
    RGB,
    SRGB,
    HSL,
    CMYK,
    EXTENDED_SRGB,
    DISPLAY_P3,
    DCI_P3,
    BT709,
    BT2020,
    HDR10_ST2084,
    DOLBYVISION,
    HDR10_HLG,
    ADOBERGB,
    PASS_THROUGH,
    DISPLAY_NATIVE_AMD
};

enum class buffer_compressed_format_t
{
    UNDEFINED,
    DXGI_FORMAT_BC1_UNORM,
    DXGI_FORMAT_BC1_UNORM_SRGB,
    DXGI_FORMAT_BC1_TYPELESS,
    DXGI_FORMAT_BC2_UNORM,
    DXGI_FORMAT_BC2_UNORM_SRGB,
    DXGI_FORMAT_BC2_TYPELESS,
    DXGI_FORMAT_BC3_UNORM,
    DXGI_FORMAT_BC3_UNORM_SRGB,
    DXGI_FORMAT_BC3_TYPELESS,
    DXGI_FORMAT_BC4_UNORM,
    DXGI_FORMAT_BC4_SNORM,
    DXGI_FORMAT_BC4_TYPELESS,
    DXGI_FORMAT_BC5_UNORM,
    DXGI_FORMAT_BC5_SNORM,
    DXGI_FORMAT_BC5_TYPELESS,
    DXGI_FORMAT_BC6H_UF16,
    DXGI_FORMAT_BC6H_SF16,
    DXGI_FORMAT_BC6H_TYPELESS,
    DXGI_FORMAT_BC7_UNORM,
    DXGI_FORMAT_BC7_UNORM_SRGB,
    DXGI_FORMAT_BC7_TYPELESS,
    ETC2_R8G8B8_UNORM_BLOCK,
    ETC2_R8G8B8_SRGB_BLOCK,
    ETC2_R8G8B8A1_UNORM_BLOCK,
    ETC2_R8G8B8A1_SRGB_BLOCK,
    ETC2_R8G8B8A8_UNORM_BLOCK,
    ETC2_R8G8B8A8_SRGB_BLOCK,
    EAC_R11_UNORM_BLOCK,
    EAC_R11_SNORM_BLOCK,
    EAC_R11G11_UNORM_BLOCK,
    EAC_R11G11_SNORM_BLOCK,
    ASTC_4x4_UNORM_BLOCK,
    ASTC_4x4_SRGB_BLOCK,
    ASTC_5x4_UNORM_BLOCK,
    ASTC_5x4_SRGB_BLOCK,
    ASTC_5x5_UNORM_BLOCK,
    ASTC_5x5_SRGB_BLOCK,
    ASTC_6x5_UNORM_BLOCK,
    ASTC_6x5_SRGB_BLOCK,
    ASTC_6x6_UNORM_BLOCK,
    ASTC_6x6_SRGB_BLOCK,
    ASTC_8x5_UNORM_BLOCK,
    ASTC_8x5_SRGB_BLOCK,
    ASTC_8x6_UNORM_BLOCK,
    ASTC_8x6_SRGB_BLOCK,
    ASTC_8x8_UNORM_BLOCK,
    ASTC_8x8_SRGB_BLOCK,
    ASTC_10x5_UNORM_BLOCK,
    ASTC_10x5_SRGB_BLOCK,
    ASTC_10x6_UNORM_BLOCK,
    ASTC_10x6_SRGB_BLOCK,
    ASTC_10x8_UNORM_BLOCK,
    ASTC_10x8_SRGB_BLOCK,
    ASTC_10x10_UNORM_BLOCK,
    ASTC_10x10_SRGB_BLOCK,
    ASTC_12x10_UNORM_BLOCK,
    ASTC_12x10_SRGB_BLOCK,
    ASTC_12x12_UNORM_BLOCK,
    ASTC_12x12_SRGB_BLOCK,
    PVRTC1_2BPP_UNORM_BLOCK_IMG,
    PVRTC1_4BPP_UNORM_BLOCK_IMG,
    PVRTC2_2BPP_UNORM_BLOCK_IMG,
    PVRTC2_4BPP_UNORM_BLOCK_IMG,
    PVRTC1_2BPP_SRGB_BLOCK_IMG,
    PVRTC1_4BPP_SRGB_BLOCK_IMG,
    PVRTC2_2BPP_SRGB_BLOCK_IMG,
    PVRTC2_4BPP_SRGB_BLOCK_IMG,
    ASTC_4x4_SFLOAT_BLOCK,
    ASTC_5x4_SFLOAT_BLOCK,
    ASTC_5x5_SFLOAT_BLOCK,
    ASTC_6x5_SFLOAT_BLOCK,
    ASTC_6x6_SFLOAT_BLOCK,
    ASTC_8x5_SFLOAT_BLOCK,
    ASTC_8x6_SFLOAT_BLOCK,
    ASTC_8x8_SFLOAT_BLOCK,
    ASTC_10x5_SFLOAT_BLOCK,
    ASTC_10x6_SFLOAT_BLOCK,
    ASTC_10x8_SFLOAT_BLOCK,
    ASTC_10x10_SFLOAT_BLOCK,
    ASTC_12x10_SFLOAT_BLOCK,
    ASTC_12x12_SFLOAT_BLOCK
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer morfology: what is inside the buffer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - bit number 31 -> compressed: if 1, the buffer has a compressed format. A compresed format has its own layout not specified
//                      here, for non compressed formats, following bits are usefull
// if bit 31 is 1, the code is interpreted as following:
//      [1 bit buffer is compressed] [8 bits color_space_t] [3 bits sampler_read_t] [20 bits compressed_format_t]
// if bit 31 is 0, the code is interpreted as following:
//      [1 bit buffer is compressed] [8 bits color_space_t] [3 bits sampler_read_t] 
//      [3 bits buffer_layout_t] [3 bits buffer_object_layout_t] [5 bits buffer_element_layout_t] 
//      [1 bit number is compressed]
//          [1 bit] [8 bits number_compressed_format_t]
//          [1 bit] [1 bit signed] [1 bit normalized] [2 bits number_format_t] [3 bits number_bit_count_t]
// Description bits:
// - compressed: if 1, the buffer has a compressed format. A compresed format has its own layout not specified
//               here, for non compressed formats, following bits are usefull
// - buffer_layout_t: specifies the layout about how objects are disposed, for example, an array of volume textures
// - buffer_object_layout_t: specifies the object layout, for example, a texture, a volume texture, a 1d vertex buffer, ...
// - buffer_element_layout_t: specifies hoe single elements are disposed
// - signed: if true, single elements are signed
// - normalized: if true, single elements are normalized, that is, must be interpreted in range from -1 to +1
// - number format: format of every single element
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
union buffer_format_t
{
    uint32_t code;
};







