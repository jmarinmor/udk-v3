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
// Buffer content
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

enum class buffer_layout_t
{
    SINGLE_ELEMENT = 0,
    LAYOUT_1D = 1,
    LAYOUT_2D = 2,
    LAYOUT_3D = 3,
    CUBE = 4
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer element format
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

enum class compressed_format_t
{
    UNDEFINED,
    PACK_4_4,
    PACK_4_4_4_4,
    PACK_5_6_5,
    PACK_1_5_5_5,
    PACK_5_5_5_1,
    PACK_2_10_10_10,
    PACK_10_11_11,
    D24_PACK24,
    X8_D24_UNORM_PACK32,
    A8B8G8R8_SRGB_PACK32,
    E5B9G9R9_UFLOAT_PACK32,
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
    DXGI_FORMAT_BC1_UNORM_BLOCK,
    DXGI_FORMAT_BC1_UNORM_SRGB_BLOCK,
    DXGI_FORMAT_BC1_TYPELESS_BLOCK,
    DXGI_FORMAT_BC2_UNORM_BLOCK,
    DXGI_FORMAT_BC2_UNORM_SRGB_BLOCK,
    DXGI_FORMAT_BC2_TYPELESS_BLOCK,
    DXGI_FORMAT_BC3_UNORM_BLOCK,
    DXGI_FORMAT_BC3_UNORM_SRGB_BLOCK,
    DXGI_FORMAT_BC3_TYPELESS_BLOCK,
    DXGI_FORMAT_BC4_UNORM_BLOCK,
    DXGI_FORMAT_BC4_SNORM_BLOCK,
    DXGI_FORMAT_BC4_TYPELESS_BLOCK,
    DXGI_FORMAT_BC5_UNORM_BLOCK,
    DXGI_FORMAT_BC5_SNORM_BLOCK,
    DXGI_FORMAT_BC5_TYPELESS_BLOCK,
    DXGI_FORMAT_BC6H_UF16_BLOCK,
    DXGI_FORMAT_BC6H_SF16_BLOCK,
    DXGI_FORMAT_BC6H_TYPELESS_BLOCK,
    DXGI_FORMAT_BC7_UNORM_BLOCK,
    DXGI_FORMAT_BC7_UNORM_SRGB_BLOCK,
    DXGI_FORMAT_BC7_TYPELESS_BLOCK,
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
    PVRTC1_2BPP_UNORM_BLOCK,
    PVRTC1_4BPP_UNORM_BLOCK,
    PVRTC2_2BPP_UNORM_BLOCK,
    PVRTC2_4BPP_UNORM_BLOCK,
    PVRTC1_2BPP_SRGB_BLOCK,
    PVRTC1_4BPP_SRGB_BLOCK,
    PVRTC2_2BPP_SRGB_BLOCK,
    PVRTC2_4BPP_SRGB_BLOCK,
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

enum class order_t
{
    DEFAULT = 0,
    RGBA = 0,
    XYZW = 0,
    BGRA = 1,
    ZYXW = 1,
    ARGB = 2,
    WXYZ = 2,
    ABGR = 3,
    WZYX = 3
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer format: what is inside the buffer, packed into a 16-bit code.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The 16 bits are interpreted as follows:
//
// Bit 15: is_compressed
// |
// +- if 1 (Compressed format):
// |   |
// |   +-- Bits 0-14: compressed_format_t (15 bits for compressed format enum)
// |
// +- if 0 (Uncompressed format):
//     |
//     +-- Bits 10-14: buffer_element_layout (5 bits)
//     +-- Bits 8-9: order_t (2 bits)
//     +-- Bit 7: is_normalized (1 bit)
//     +-- Bit 6: is_signed (1 bit)
//     +-- Bits 3-5: number_format (3 bits)
//     +-- Bits 0-2: number_bit_count (3 bits)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct buffer_format_t
{
    uint16_t code;

private:
    // Bit layout
    static constexpr uint16_t IS_COMPRESSED_SHIFT = 15;
    static constexpr uint16_t IS_COMPRESSED_MASK  = 1u << IS_COMPRESSED_SHIFT;

    static constexpr uint16_t COMPRESSED_FORMAT_SHIFT = 0;
    static constexpr uint16_t COMPRESSED_FORMAT_MASK  = 0x7FFFu << COMPRESSED_FORMAT_SHIFT;

    static constexpr uint16_t BUFFER_ELEMENT_LAYOUT_SHIFT = 10;
    static constexpr uint16_t BUFFER_ELEMENT_LAYOUT_MASK  = 0x1Fu << BUFFER_ELEMENT_LAYOUT_SHIFT;

    static constexpr uint16_t ORDER_SHIFT = 8;
    static constexpr uint16_t ORDER_MASK  = 0x3u << ORDER_SHIFT;

    static constexpr uint16_t IS_NORMALIZED_SHIFT = 7;
    static constexpr uint16_t IS_NORMALIZED_MASK  = 1u << IS_NORMALIZED_SHIFT;

    static constexpr uint16_t IS_SIGNED_SHIFT = 6;
    static constexpr uint16_t IS_SIGNED_MASK  = 1u << IS_SIGNED_SHIFT;

    static constexpr uint16_t NUMBER_FORMAT_SHIFT = 3;
    static constexpr uint16_t NUMBER_FORMAT_MASK  = 0x7u << NUMBER_FORMAT_SHIFT;

    static constexpr uint16_t NUMBER_BIT_COUNT_SHIFT = 0;
    static constexpr uint16_t NUMBER_BIT_COUNT_MASK  = 0x7u << NUMBER_BIT_COUNT_SHIFT;

    // Helpers para bits y enums
    static constexpr auto get_flag(uint16_t c, uint16_t mask) -> bool { return (c & mask) != 0; }
    static constexpr auto set_flag(uint16_t c, uint16_t mask, bool v) -> uint16_t { return (c & ~mask) | (uint16_t(v) ? mask : 0u); }
    static constexpr auto get_bits(uint16_t c, uint16_t mask, uint16_t shift) -> uint16_t { return (c & mask) >> shift; }
    static constexpr auto set_bits(uint16_t c, uint16_t mask, uint16_t shift, uint16_t v) -> uint16_t { return uint16_t((c & ~mask) | ((v << shift) & mask)); }
    template<class E> static constexpr auto get_enum(uint16_t c, uint16_t mask, uint16_t shift) -> E { return static_cast<E>(get_bits(c, mask, shift)); }
    template<class E> static constexpr auto enc(E v) -> uint16_t { return static_cast<uint16_t>(v); }

public:
    // Constructores
    buffer_format_t() : code(0) {}
    buffer_format_t(uint16_t initial_code) : code(initial_code) {}
    buffer_format_t(compressed_format_t compressed_format) { code = (1u << IS_COMPRESSED_SHIFT) | (enc(compressed_format) << COMPRESSED_FORMAT_SHIFT); }
    buffer_format_t(buffer_element_layout_t layout, order_t order, bool is_signed_, bool is_normalized_, number_format_t num_format, number_bit_count_t num_bits)
    {
        code = (enc(layout) << BUFFER_ELEMENT_LAYOUT_SHIFT) |
               (enc(order) << ORDER_SHIFT) |
               (uint16_t(is_normalized_) << IS_NORMALIZED_SHIFT) |
               (uint16_t(is_signed_) << IS_SIGNED_SHIFT) |
               (enc(num_format) << NUMBER_FORMAT_SHIFT) |
               (enc(num_bits) << NUMBER_BIT_COUNT_SHIFT);
    }

    // Presets
    static constexpr auto R8G8B8A8_UNORM() -> buffer_format_t { return buffer_format_t(buffer_element_layout_t::VECTOR_4, order_t::RGBA, false, true, number_format_t::INTEGER, number_bit_count_t::BC_8); }
    static constexpr auto R8G8B8A8_SNORM() -> buffer_format_t { return buffer_format_t(buffer_element_layout_t::VECTOR_4, order_t::RGBA, true,  true, number_format_t::INTEGER, number_bit_count_t::BC_8); }
    static constexpr auto R8_UNORM()       -> buffer_format_t { return buffer_format_t(buffer_element_layout_t::SINGLE_ELEMENT, order_t::R, false, true, number_format_t::INTEGER, number_bit_count_t::BC_8); }
    static constexpr auto R32_SFLOAT()     -> buffer_format_t { return buffer_format_t(buffer_element_layout_t::SINGLE_ELEMENT, order_t::R, true,  false, number_format_t::FLOAT,   number_bit_count_t::BC_32); }
    static constexpr auto R32G32_SFLOAT()  -> buffer_format_t { return buffer_format_t(buffer_element_layout_t::VECTOR_2, order_t::RG, true,  false, number_format_t::FLOAT,   number_bit_count_t::BC_32); }
    static constexpr auto R32G32B32_SFLOAT()-> buffer_format_t { return buffer_format_t(buffer_element_layout_t::VECTOR_3, order_t::RGB, true, false, number_format_t::FLOAT,   number_bit_count_t::BC_32); }
    static constexpr auto R32G32B32A32_SFLOAT()-> buffer_format_t { return buffer_format_t(buffer_element_layout_t::VECTOR_4, order_t::RGBA, true, false, number_format_t::FLOAT, number_bit_count_t::BC_32); }

    // Accessors
    auto is_compressed() const -> bool { return get_flag(code, IS_COMPRESSED_MASK); }
    auto is_compressed(bool v) -> buffer_format_t& { code = set_flag(code, IS_COMPRESSED_MASK, v); return *this; }
    auto is_signed() const -> bool { return get_flag(code, IS_SIGNED_MASK); }
    auto is_signed(bool v) -> buffer_format_t& { code = set_flag(code, IS_SIGNED_MASK, v); return *this; }
    auto is_normalized() const -> bool { return get_flag(code, IS_NORMALIZED_MASK); }
    auto is_normalized(bool v) -> buffer_format_t& { code = set_flag(code, IS_NORMALIZED_MASK, v); return *this; }

    auto compressed_format() const -> compressed_format_t { return get_enum<compressed_format_t>(code, COMPRESSED_FORMAT_MASK, COMPRESSED_FORMAT_SHIFT); }
    auto compressed_format(compressed_format_t v) -> buffer_format_t& { code = set_bits(code, COMPRESSED_FORMAT_MASK, COMPRESSED_FORMAT_SHIFT, enc(v)); return *this; }
    auto element_layout() const -> buffer_element_layout_t { return get_enum<buffer_element_layout_t>(code, BUFFER_ELEMENT_LAYOUT_MASK, BUFFER_ELEMENT_LAYOUT_SHIFT); }
    auto element_layout(buffer_element_layout_t v) -> buffer_format_t& { code = set_bits(code, BUFFER_ELEMENT_LAYOUT_MASK, BUFFER_ELEMENT_LAYOUT_SHIFT, enc(v)); return *this; }
    auto order() const -> order_t { return get_enum<order_t>(code, ORDER_MASK, ORDER_SHIFT); }
    auto order(order_t v) -> buffer_format_t& { code = set_bits(code, ORDER_MASK, ORDER_SHIFT, enc(v)); return *this; }
    auto number_format() const -> number_format_t { return get_enum<number_format_t>(code, NUMBER_FORMAT_MASK, NUMBER_FORMAT_SHIFT); }
    auto number_format(number_format_t v) -> buffer_format_t& { code = set_bits(code, NUMBER_FORMAT_MASK, NUMBER_FORMAT_SHIFT, enc(v)); return *this; }
    auto number_bit_count() const -> number_bit_count_t { return get_enum<number_bit_count_t>(code, NUMBER_BIT_COUNT_MASK, NUMBER_BIT_COUNT_SHIFT); }
    auto number_bit_count(number_bit_count_t v) -> buffer_format_t& { code = set_bits(code, NUMBER_BIT_COUNT_MASK, NUMBER_BIT_COUNT_SHIFT, enc(v)); return *this; }

    // Tamaños
    auto element_bit_count() const -> int { return 1 << (int(number_bit_count()) + 3); }
    auto element_byte_count() const -> int { return element_bit_count() / 8; }
    auto byte_count() const -> int { return element_byte_count() * int(element_layout()); }
    auto byte_count(int width) const -> int { return byte_count() * width; }
    auto byte_count(int width, int height) const -> int { return byte_count(width) * height; }
    auto byte_count(int width, int height, int depth) const -> int { return byte_count(width, height) * depth; }
};

