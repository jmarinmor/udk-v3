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
struct buffer_format_t
{
    uint32_t code;

private:
    // Common fields
    static constexpr uint32_t IS_COMPRESSED_SHIFT = 31;
    static constexpr uint32_t COLOR_SPACE_SHIFT = 23;
    static constexpr uint32_t SAMPLER_READ_SHIFT = 20;

    static constexpr uint32_t IS_COMPRESSED_MASK = 1 << IS_COMPRESSED_SHIFT;
    static constexpr uint32_t COLOR_SPACE_MASK = 0xFF << COLOR_SPACE_SHIFT;
    static constexpr uint32_t SAMPLER_READ_MASK = 0x7 << SAMPLER_READ_SHIFT;

    // Compressed format fields (when bit 31 is 1)
    static constexpr uint32_t COMPRESSED_FORMAT_SHIFT = 0;
    static constexpr uint32_t COMPRESSED_FORMAT_MASK = 0xFFFFF; // 20 bits

    // Uncompressed format fields (when bit 31 is 0)
    static constexpr uint32_t BUFFER_LAYOUT_SHIFT = 17;
    static constexpr uint32_t BUFFER_OBJECT_LAYOUT_SHIFT = 14;
    static constexpr uint32_t BUFFER_ELEMENT_LAYOUT_SHIFT = 9;
    static constexpr uint32_t IS_NUMBER_COMPRESSED_SHIFT = 8;

    static constexpr uint32_t BUFFER_LAYOUT_MASK = 0x7 << BUFFER_LAYOUT_SHIFT;
    static constexpr uint32_t BUFFER_OBJECT_LAYOUT_MASK = 0x7 << BUFFER_OBJECT_LAYOUT_SHIFT;
    static constexpr uint32_t BUFFER_ELEMENT_LAYOUT_MASK = 0x1F << BUFFER_ELEMENT_LAYOUT_SHIFT;
    static constexpr uint32_t IS_NUMBER_COMPRESSED_MASK = 1 << IS_NUMBER_COMPRESSED_SHIFT;

    // Uncompressed number fields (when bit 8 is 1)
    static constexpr uint32_t NUMBER_COMPRESSED_FORMAT_SHIFT = 0;
    static constexpr uint32_t NUMBER_COMPRESSED_FORMAT_MASK = 0xFF; // 8 bits

    // Uncompressed non-numeric-compressed fields (when bit 8 is 0)
    static constexpr uint32_t NUMBER_BIT_COUNT_SHIFT = 0;
    static constexpr uint32_t NUMBER_FORMAT_SHIFT = 3;
    static constexpr uint32_t IS_NORMALIZED_SHIFT = 5;
    static constexpr uint32_t IS_SIGNED_SHIFT = 6;

    static constexpr uint32_t NUMBER_BIT_COUNT_MASK = 0x7 << NUMBER_BIT_COUNT_SHIFT; // 3 bits
    static constexpr uint32_t NUMBER_FORMAT_MASK = 0x3 << NUMBER_FORMAT_SHIFT; // 2 bits
    static constexpr uint32_t IS_NORMALIZED_MASK = 1 << IS_NORMALIZED_SHIFT;
    static constexpr uint32_t IS_SIGNED_MASK = 1 << IS_SIGNED_SHIFT;

public:
    // Constructors
    inline buffer_format_t() : code(0) {}
    inline buffer_format_t(uint32_t c) : code(c) {}

    // Constructor for compressed formats
    inline buffer_format_t(buffer_compressed_format_t format, color_space_t cs = color_space_t::UNDEFINED, sampler_read_t sr = sampler_read_t::NEAREST)
    {
        code = (1 << IS_COMPRESSED_SHIFT) |
               (static_cast<uint32_t>(cs) << COLOR_SPACE_SHIFT) |
               (static_cast<uint32_t>(sr) << SAMPLER_READ_SHIFT) |
               (static_cast<uint32_t>(format) << COMPRESSED_FORMAT_SHIFT);
    }

    // Constructor for uncompressed, number-compressed formats
    inline buffer_format_t(number_compressed_format_t num_format, buffer_layout_t bl, buffer_object_layout_t bol, buffer_element_layout_t bel, color_space_t cs = color_space_t::UNDEFINED, sampler_read_t sr = sampler_read_t::NEAREST)
    {
        code = (static_cast<uint32_t>(cs) << COLOR_SPACE_SHIFT) |
               (static_cast<uint32_t>(sr) << SAMPLER_READ_SHIFT) |
               (static_cast<uint32_t>(bl) << BUFFER_LAYOUT_SHIFT) |
               (static_cast<uint32_t>(bol) << BUFFER_OBJECT_LAYOUT_SHIFT) |
               (static_cast<uint32_t>(bel) << BUFFER_ELEMENT_LAYOUT_SHIFT) |
               (1 << IS_NUMBER_COMPRESSED_SHIFT) |
               (static_cast<uint32_t>(num_format) << NUMBER_COMPRESSED_FORMAT_SHIFT);
    }

    // Constructor for uncompressed, non-number-compressed formats
    inline buffer_format_t(number_bit_count_t nbc, number_format_t nf, bool normalized, bool sign, buffer_layout_t bl, buffer_object_layout_t bol, buffer_element_layout_t bel, color_space_t cs = color_space_t::UNDEFINED, sampler_read_t sr = sampler_read_t::NEAREST)
    {
        code = (static_cast<uint32_t>(cs) << COLOR_SPACE_SHIFT) |
               (static_cast<uint32_t>(sr) << SAMPLER_READ_SHIFT) |
               (static_cast<uint32_t>(bl) << BUFFER_LAYOUT_SHIFT) |
               (static_cast<uint32_t>(bol) << BUFFER_OBJECT_LAYOUT_SHIFT) |
               (static_cast<uint32_t>(bel) << BUFFER_ELEMENT_LAYOUT_SHIFT) |
               (static_cast<uint32_t>(sign) << IS_SIGNED_SHIFT) |
               (static_cast<uint32_t>(normalized) << IS_NORMALIZED_SHIFT) |
               (static_cast<uint32_t>(nf) << NUMBER_FORMAT_SHIFT) |
               (static_cast<uint32_t>(nbc) << NUMBER_BIT_COUNT_SHIFT);
    }

    // Getters and Setters
    inline bool is_compressed() const { return (code & IS_COMPRESSED_MASK) != 0; }
    inline void set_is_compressed(bool compressed) { code = (code & ~IS_COMPRESSED_MASK) | (static_cast<uint32_t>(compressed) << IS_COMPRESSED_SHIFT); }

    inline color_space_t get_color_space() const { return static_cast<color_space_t>((code & COLOR_SPACE_MASK) >> COLOR_SPACE_SHIFT); }
    inline void set_color_space(color_space_t cs) { code = (code & ~COLOR_SPACE_MASK) | (static_cast<uint32_t>(cs) << COLOR_SPACE_SHIFT); }

    inline sampler_read_t get_sampler_read() const { return static_cast<sampler_read_t>((code & SAMPLER_READ_MASK) >> SAMPLER_READ_SHIFT); }
    inline void set_sampler_read(sampler_read_t sr) { code = (code & ~SAMPLER_READ_MASK) | (static_cast<uint32_t>(sr) << SAMPLER_READ_SHIFT); }

    // Compressed format accessors (valid only if is_compressed() is true)
    inline buffer_compressed_format_t get_compressed_format() const { return static_cast<buffer_compressed_format_t>((code & COMPRESSED_FORMAT_MASK) >> COMPRESSED_FORMAT_SHIFT); }
    inline void set_compressed_format(buffer_compressed_format_t format) { code = (code & ~COMPRESSED_FORMAT_MASK) | (static_cast<uint32_t>(format) << COMPRESSED_FORMAT_SHIFT); }

    // Uncompressed format accessors (valid only if is_compressed() is false)
    inline buffer_layout_t get_buffer_layout() const { return static_cast<buffer_layout_t>((code & BUFFER_LAYOUT_MASK) >> BUFFER_LAYOUT_SHIFT); }
    inline void set_buffer_layout(buffer_layout_t bl) { code = (code & ~BUFFER_LAYOUT_MASK) | (static_cast<uint32_t>(bl) << BUFFER_LAYOUT_SHIFT); }

    inline buffer_object_layout_t get_buffer_object_layout() const { return static_cast<buffer_object_layout_t>((code & BUFFER_OBJECT_LAYOUT_MASK) >> BUFFER_OBJECT_LAYOUT_SHIFT); }
    inline void set_buffer_object_layout(buffer_object_layout_t bol) { code = (code & ~BUFFER_OBJECT_LAYOUT_MASK) | (static_cast<uint32_t>(bol) << BUFFER_OBJECT_LAYOUT_SHIFT); }

    inline buffer_element_layout_t get_buffer_element_layout() const { return static_cast<buffer_element_layout_t>((code & BUFFER_ELEMENT_LAYOUT_MASK) >> BUFFER_ELEMENT_LAYOUT_SHIFT); }
    inline void set_buffer_element_layout(buffer_element_layout_t bel) { code = (code & ~BUFFER_ELEMENT_LAYOUT_MASK) | (static_cast<uint32_t>(bel) << BUFFER_ELEMENT_LAYOUT_SHIFT); }

    inline bool is_number_compressed() const { return (code & IS_NUMBER_COMPRESSED_MASK) != 0; }
    inline void set_is_number_compressed(bool num_compressed) { code = (code & ~IS_NUMBER_COMPRESSED_MASK) | (static_cast<uint32_t>(num_compressed) << IS_NUMBER_COMPRESSED_SHIFT); }

    // Number-compressed format accessors (valid only if is_compressed() is false and is_number_compressed() is true)
    inline number_compressed_format_t get_number_compressed_format() const { return static_cast<number_compressed_format_t>((code & NUMBER_COMPRESSED_FORMAT_MASK) >> NUMBER_COMPRESSED_FORMAT_SHIFT); }
    inline void set_number_compressed_format(number_compressed_format_t num_format) { code = (code & ~NUMBER_COMPRESSED_FORMAT_MASK) | (static_cast<uint32_t>(num_format) << NUMBER_COMPRESSED_FORMAT_SHIFT); }

    // Non-number-compressed format accessors (valid only if is_compressed() is false and is_number_compressed() is false)
    inline bool is_signed() const { return (code & IS_SIGNED_MASK) != 0; }
    inline void set_is_signed(bool sign) { code = (code & ~IS_SIGNED_MASK) | (static_cast<uint32_t>(sign) << IS_SIGNED_SHIFT); }

    inline bool is_normalized() const { return (code & IS_NORMALIZED_MASK) != 0; }
    inline void set_is_normalized(bool normalized) { code = (code & ~IS_NORMALIZED_MASK) | (static_cast<uint32_t>(normalized) << IS_NORMALIZED_SHIFT); }

    inline number_format_t get_number_format() const { return static_cast<number_format_t>((code & NUMBER_FORMAT_MASK) >> NUMBER_FORMAT_SHIFT); }
    inline void set_number_format(number_format_t nf) { code = (code & ~NUMBER_FORMAT_MASK) | (static_cast<uint32_t>(nf) << NUMBER_FORMAT_SHIFT); }

    inline number_bit_count_t get_number_bit_count() const { return static_cast<number_bit_count_t>((code & NUMBER_BIT_COUNT_MASK) >> NUMBER_BIT_COUNT_SHIFT); }
    inline void set_number_bit_count(number_bit_count_t nbc) { code = (code & ~NUMBER_BIT_COUNT_MASK) | (static_cast<uint32_t>(nbc) << NUMBER_BIT_COUNT_SHIFT); }
};







