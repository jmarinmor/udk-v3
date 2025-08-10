#pragma once

#include <stdint.h>
#include <cassert>
#include <pre.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vexel format
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum class vexel_layout_t : uint16_t
{
    UNKNOWN = 0,
    SINGLE_ELEMENT = 1,
    VECTOR_1 = 1,
    VECTOR_2 = 2,
    VECTOR_3 = 3,
    VECTOR_4 = 4,
    MATRIX_4x4 = 5, // COLUMNASxFILAS
    MATRIX_4x3 = 6,
    MATRIX_4x2 = 7,
    MATRIX_3x3 = 8,
    MATRIX_2x2 = 9,

    // BCn (Block Compression - S3TC/DXT)
    BLOCK_BC1,
    BLOCK_BC2,
    BLOCK_BC3,
    BLOCK_BC4,
    BLOCK_BC5,
    BLOCK_BC6H,
    BLOCK_BC7,

    // ETC / EAC
    BLOCK_ETC2,
    BLOCK_EAC_R11,
    BLOCK_EAC_R11G11,

    // ASTC (Adaptive Scalable Texture Compression)
    BLOCK_ASTC_4x4,
    BLOCK_ASTC_5x4,
    BLOCK_ASTC_5x5,
    BLOCK_ASTC_6x5,
    BLOCK_ASTC_6x6,
    BLOCK_ASTC_8x5,
    BLOCK_ASTC_8x6,
    BLOCK_ASTC_8x8,
    BLOCK_ASTC_10x5,
    BLOCK_ASTC_10x6,
    BLOCK_ASTC_10x8,
    BLOCK_ASTC_10x10,
    BLOCK_ASTC_12x10,
    BLOCK_ASTC_12x12,

    // PVRTC (PowerVR Texture Compression)
    BLOCK_PVRTC1_2BPP,
    BLOCK_PVRTC1_4BPP,
    BLOCK_PVRTC2_2BPP,
    BLOCK_PVRTC2_4BPP    
};

enum class vexel_number_format_t : uint8_t
{
    UNKNOWN = 0,
    INTEGER = 1,
    FLOAT = 2,
    FIXED = 3,
    TYPELESS = 4
};

enum class vexel_space_t : uint16_t
{
    LINEAR,
    RGB,
    SRGB,
    EXTENDED_SRGB,
    ADOBE_RGB,
    HDR_EBGR,
    HDR10_ST2084,
    HDR10_HLG,
    YUV_420_Y_U_V,
    YUV_420_Y_UV,
    YUV_422_Y_U_V,
    YUV_422_Y_UV,
    YUV_422_YUYV,
    YUV_422_UYVY,
    YUV_444_Y_U_V,
    YUV_444_Y_UV,
    DISPLAY_P3,
    DCI_P3,
    BT709,
    BT2020,
    DOLBYVISION,
    PASS_THROUGH,
    DISPLAY_NATIVE_AMD
};

enum class vexel_interpolation_t : uint8_t
{
    LINEAR,
    NONLINEAR
};


enum class vexel_bit_count_t : uint16_t
{
    NONE = 0,
    UNIFORM_8 = 1,
    UNIFORM_16 = 2,
    UNIFORM_32 = 3,
    UNIFORM_64 = 4,
    UNIFORM_128 = 5,
    UNIFORM_256 = 6,
    UNIFORM_512 = 7,
    UNIFORM_1024 = 8,

    PACK_4_4,
    PACK_4_4_4_4,
    PACK_5_6_5,
    PACK_1_5_5_5,
    PACK_5_5_5_1,
    PACK_2_10_10_10,
    PACK_10_11_11,
    PACK_5_9_9_9,
    PACK_8_8_8_1,
    PACK_24,
    PACK_X8_24,
    PACK_24_X8,
    PACK_10X6,
    PACK_10X6_10X6,
    PACK_10X6_10X6_10X6,
    PACK_10X6_10X6_10X6_10X6,
    PACK_12X4,
    PACK_12X4_12X4,
    PACK_12X4_12X4_12X4,
    PACK_12X4_12X4_12X4_12X4,
};

enum class vexel_special_t : uint16_t
{
    NONE
};

enum class vexel_order_t : uint8_t {
    DEFAULT = 0,
    RGBA = 0,  XYZW = 0,
    BGRA = 1,  ZYXW = 1,
    ARGB = 2,  WXYZ = 2,
    ABGR = 3,  WZYX = 3
};

// =======================
// Info (typos y bitfields)
// =======================
struct vexel_info_t
{
    union {
        uint32_t code;
        struct {
            vexel_special_t       special        : 10; // <-- “spacial” -> “special”
            vexel_space_t         space          : 10;
            vexel_order_t         order          : 5;
            vexel_interpolation_t interpolation  : 4;
            unsigned              _pad           : 3;  // opcional para redondear a 32 si tu ABI lo requiere
        };
    };
    constexpr vexel_info_t() : code(0) {}
    constexpr vexel_info_t(uint32_t c) : code(c) {}
};

// =======================
// Structure (macro fix + ctor fix)
// =======================
struct vexel_structure_t
{
    union {
        uint32_t code;
        struct {
            vexel_layout_t        layout         : 10;
            unsigned              is_signed      : 1;  // 1 bit basta
            unsigned              is_normalized  : 1;  // 1 bit basta
            vexel_number_format_t number_format  : 4;
            vexel_bit_count_t     bit_count      : 10;
            unsigned              _pad           : 6;
        };
    };

    constexpr vexel_structure_t() : code(0) {}
    constexpr vexel_structure_t(uint32_t c) : code(c) {}

    constexpr vexel_structure_t(vexel_layout_t l, bool s, bool n, vexel_number_format_t t, vexel_bit_count_t bc)
        : layout(l), is_signed(s?1:0), is_normalized(n?1:0), number_format(t), bit_count(bc) {}

    // Helper robusto para mapear bits reales a tu enum UNIFORM_*
    static constexpr vexel_bit_count_t uniform_from_bits(int bits) {
        return (bits==8 ) ? vexel_bit_count_t::UNIFORM_8  :
               (bits==16) ? vexel_bit_count_t::UNIFORM_16 :
               (bits==32) ? vexel_bit_count_t::UNIFORM_32 :
               (bits==64) ? vexel_bit_count_t::UNIFORM_64 :
               (bits==128)? vexel_bit_count_t::UNIFORM_128:
               (bits==256)? vexel_bit_count_t::UNIFORM_256:
               (bits==512)? vexel_bit_count_t::UNIFORM_512:
               (bits==1024)?vexel_bit_count_t::UNIFORM_1024:
               vexel_bit_count_t::NONE;
    }

    constexpr vexel_structure_t(vexel_layout_t l, bool s, bool n, vexel_number_format_t t, int bits)
        : layout(l), is_signed(s?1:0), is_normalized(n?1:0), number_format(t), bit_count(uniform_from_bits(bits))
    {
        if (bits == 24) { bit_count = vexel_bit_count_t::PACK_24; }
        assert( (bits==24) || (bit_count != vexel_bit_count_t::NONE) );
    }

    // Presets corregidos (nombres únicos y flags correctos)
    static constexpr vexel_structure_t VECx_F32 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), true,  false, vexel_number_format_t::FLOAT,   vexel_bit_count_t::UNIFORM_32); }
    static constexpr vexel_structure_t VECx_U8  (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_8 ); }
    static constexpr vexel_structure_t VECx_U16 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_16); }
    static constexpr vexel_structure_t VECx_U32 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_32); }

    static constexpr vexel_structure_t VEC1_F32() { return VECx_F32(1); }
    static constexpr vexel_structure_t VEC2_F32() { return VECx_F32(2); }
    static constexpr vexel_structure_t VEC3_F32() { return VECx_F32(3); }
    static constexpr vexel_structure_t VEC4_F32() { return VECx_F32(4); }

    static constexpr vexel_structure_t VEC1_U8 () { return VECx_U8 (1); } // p.ej. R8
    static constexpr vexel_structure_t VEC2_U8 () { return VECx_U8 (2); } // p.ej. RG8
    static constexpr vexel_structure_t VEC3_U8 () { return VECx_U8 (3); } // p.ej. RGB8
    static constexpr vexel_structure_t VEC4_U8 () { return VECx_U8 (4); } // p.ej. RGBA8

    static constexpr vexel_structure_t VEC1_U16() { return VECx_U16(1); } // p.ej. índices 16
    static constexpr vexel_structure_t VEC1_U32() { return VECx_U32(1); } // p.ej. índices 32
};

// =======================
// Formato combinado 64-bit
// =======================
struct vexel_format_t
{
    union {
        uint64_t code;
        struct {
            // Semantic information
            union {
                uint32_t code_info;
                struct {
                    vexel_special_t       special        : 10;
                    vexel_space_t         space          : 10;
                    vexel_order_t         order          : 5;
                    vexel_interpolation_t interpolation  : 4;
                    unsigned              _pad_info      : 3;
                };
            };
            // Structure of the content
            union {
                uint32_t code_structure;
                struct {
                    vexel_layout_t        layout         : 10;
                    unsigned              is_signed      : 1;
                    unsigned              is_normalized  : 1;
                    vexel_number_format_t number_format  : 4;
                    vexel_bit_count_t     bit_count      : 10;
                    unsigned              _pad_struct    : 6;
                };
            };
        };
    };

    constexpr vexel_format_t() : code(0) {}
    constexpr vexel_format_t(vexel_info_t info, vexel_structure_t s)
      : code(0) { code_info = info.code; code_structure = s.code; }

    constexpr auto info() const -> vexel_info_t { return vexel_info_t(code_info); }
    constexpr auto structure() const -> vexel_structure_t { return vexel_structure_t(code_structure); }
};

// Sane checks (no portabilidad de bitfields garantizada, pero al menos tamaño):
static_assert(sizeof(vexel_info_t)      == 4, "vexel_info_t must be 32-bit");
static_assert(sizeof(vexel_structure_t) == 4, "vexel_structure_t must be 32-bit");
static_assert(sizeof(vexel_format_t)    == 8, "vexel_format_t must be 64-bit");





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer format
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    LINEAR = 1
};

enum class sampler_wrap_mode_t
{
    REPEAT = 0,
    MIRROR = 1,
    CLAMP = 2
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


enum class buffer_object_layout_t
{
    NONE = 0,
    LAYOUT_1D = 1,
    LAYOUT_2D = 2,
    LAYOUT_3D = 3,
    CUBE = 4
};

struct buffer_object_size_t
{
    uint64_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t vexel_byte_count;
};

struct buffer_object_padding_t
{
    uint64_t row_byte_count;
    uint64_t surface_byte_count;
    uint64_t volume_byte_count;
    uint64_t object_byte_count;
    uint32_t vexel_byte_count;
};

struct buffer_layout_t
{
    buffer_object_layout_t object_layout;
    buffer_object_size_t object_size;
    uint64_t byte_count;
    uint32_t array_count;
    uint32_t lod_count;
};
