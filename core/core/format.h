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

    // YUV blocks
    BLOCK_YUV_420_Y_U_V,
    BLOCK_YUV_420_Y_UV,
    BLOCK_YUV_422_Y_U_V,
    BLOCK_YUV_422_Y_UV,
    BLOCK_YUV_422_YUYV,
    BLOCK_YUV_422_UYVY,
    BLOCK_YUV_444_Y_U_V,
    BLOCK_YUV_444_Y_UV,

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
    EUCLIDEAN,
    RGB,
    SRGB,
    EXTENDED_SRGB,
    ADOBE_RGB,
    YUV,
    HSL,
    CMYK,
    HDR_EBGR,
    HDR10_ST2084,
    HDR10_HLG,
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
    NONE,
    EXTERNAL_ID
};

enum class vexel_order_t : uint8_t {
    DEFAULT = 0,
    RGBA = 0,  XYZW = 0,
    BGRA = 1,  ZYXW = 1,
    ARGB = 2,  WXYZ = 2,
    ABGR = 3,  WZYX = 3
};

// =======================
// Semanthic info
// =======================
struct vexel_info_t {
    uint32_t code;

    // Mapa de bits (portable):
    // [31:22] special (10)
    // [21:12] space   (10)
    // [11:7]  order   (5)
    // [6:3]   interpolation (4)
    // [2:0]   _pad    (3)
    static constexpr uint32_t SPECIAL_SHIFT = 22, SPECIAL_MASK = 0x3FFu << SPECIAL_SHIFT;
    static constexpr uint32_t SPACE_SHIFT   = 12, SPACE_MASK   = 0x3FFu << SPACE_SHIFT;
    static constexpr uint32_t ORDER_SHIFT   =  7, ORDER_MASK   = 0x1Fu  << ORDER_SHIFT;
    static constexpr uint32_t INTP_SHIFT    =  3, INTP_MASK    = 0xFu   << INTP_SHIFT;
    // PAD: [2:0] — sin getters/setters

    static constexpr uint32_t get_bits(uint32_t v, uint32_t m, uint32_t s){ return (v & m) >> s; }
    static constexpr uint32_t set_bits(uint32_t v, uint32_t m, uint32_t s, uint32_t x){ return (v & ~m) | ((x << s) & m); }

    // Ctors
    constexpr vexel_info_t() : code(0) {}
    constexpr vexel_info_t(uint32_t c) : code(c) {}
    constexpr vexel_info_t(vexel_special_t special, vexel_space_t space, vexel_order_t order, vexel_interpolation_t interp)
        : code( set_bits( set_bits( set_bits( set_bits(0,
                SPECIAL_MASK, SPECIAL_SHIFT, static_cast<uint32_t>(special)),
                SPACE_MASK,   SPACE_SHIFT,   static_cast<uint32_t>(space)),
                ORDER_MASK,   ORDER_SHIFT,   static_cast<uint32_t>(order)),
                INTP_MASK,    INTP_SHIFT,    static_cast<uint32_t>(interp)) ) {}

    // Accessors
    constexpr auto special() const -> vexel_special_t { return static_cast<vexel_special_t>(get_bits(code, SPECIAL_MASK, SPECIAL_SHIFT)); }
    constexpr auto special(vexel_special_t v) -> vexel_info_t& { code = set_bits(code, SPECIAL_MASK, SPECIAL_SHIFT, static_cast<uint32_t>(v)); return *this; }

    constexpr auto space() const -> vexel_space_t { return static_cast<vexel_space_t>(get_bits(code, SPACE_MASK, SPACE_SHIFT)); }
    constexpr auto space(vexel_space_t v) -> vexel_info_t& { code = set_bits(code, SPACE_MASK, SPACE_SHIFT, static_cast<uint32_t>(v)); return *this; }

    constexpr auto order() const -> vexel_order_t { return static_cast<vexel_order_t>(get_bits(code, ORDER_MASK, ORDER_SHIFT)); }
    constexpr auto order(vexel_order_t v) -> vexel_info_t& { code = set_bits(code, ORDER_MASK, ORDER_SHIFT, static_cast<uint32_t>(v)); return *this; }

    constexpr auto interpolation() const -> vexel_interpolation_t { return static_cast<vexel_interpolation_t>(get_bits(code, INTP_MASK, INTP_SHIFT)); }
    constexpr auto interpolation(vexel_interpolation_t v) -> vexel_info_t& { code = set_bits(code, INTP_MASK, INTP_SHIFT, static_cast<uint32_t>(v)); return *this; }
};

// =======================
// Structure info
// =======================
struct vexel_structure_t {
    uint32_t code;

    // Mapa de bits:
    // [31:22] layout (10)
    // [21]    is_signed (1)
    // [20]    is_normalized (1)
    // [19:16] number_format (4)
    // [15:6]  bit_count (10)
    // [5:0]   _pad (6)
    static constexpr uint32_t LAYOUT_SHIFT = 22, LAYOUT_MASK = 0x3FFu << LAYOUT_SHIFT;
    static constexpr uint32_t SIGN_SHIFT   = 21, SIGN_MASK   = 1u     << SIGN_SHIFT;
    static constexpr uint32_t NORM_SHIFT   = 20, NORM_MASK   = 1u     << NORM_SHIFT;
    static constexpr uint32_t NFMT_SHIFT   = 16, NFMT_MASK   = 0xFu   << NFMT_SHIFT;
    static constexpr uint32_t BCNT_SHIFT   =  6, BCNT_MASK   = 0x3FFu << BCNT_SHIFT;
    // PAD: [5:0]

    static constexpr uint32_t get_bits(uint32_t v, uint32_t m, uint32_t s){ return (v & m) >> s; }
    static constexpr uint32_t set_bits(uint32_t v, uint32_t m, uint32_t s, uint32_t x){ return (v & ~m) | ((x << s) & m); }

    // Ctors
    constexpr vexel_structure_t() : code(0) {}
    constexpr vexel_structure_t(uint32_t c) : code(c) {}

    constexpr vexel_structure_t(vexel_layout_t layout, bool is_signed, bool is_normalized, vexel_number_format_t number_format, vexel_bit_count_t bit_count)
        : code(0)
    {
        code = set_bits(code, LAYOUT_MASK, LAYOUT_SHIFT, static_cast<uint32_t>(layout));
        code = is_signed      ? (code | SIGN_MASK) : (code & ~SIGN_MASK);
        code = is_normalized  ? (code | NORM_MASK) : (code & ~NORM_MASK);
        code = set_bits(code, NFMT_MASK,  NFMT_SHIFT,  static_cast<uint32_t>(number_format));
        code = set_bits(code, BCNT_MASK,  BCNT_SHIFT,  static_cast<uint32_t>(bit_count));
    }

    // Helper (lo mantienes)
    static constexpr vexel_bit_count_t uniform_from_bits(int bits) {
        return (bits == 8)    ? vexel_bit_count_t::UNIFORM_8   :
            (bits == 16)   ? vexel_bit_count_t::UNIFORM_16  :
            (bits == 32)   ? vexel_bit_count_t::UNIFORM_32  :
            (bits == 64)   ? vexel_bit_count_t::UNIFORM_64  :
            (bits == 128)  ? vexel_bit_count_t::UNIFORM_128 :
            (bits == 256)  ? vexel_bit_count_t::UNIFORM_256 :
            (bits == 512)  ? vexel_bit_count_t::UNIFORM_512 :
            (bits == 1024) ? vexel_bit_count_t::UNIFORM_1024:
                                vexel_bit_count_t::NONE;
    }

    constexpr vexel_structure_t(vexel_layout_t layout, bool is_signed, bool is_normalized, vexel_number_format_t number_format, int bits)
        : vexel_structure_t(
            layout,
            is_signed,
            is_normalized,
            number_format,
            (bits == 24) ? vexel_bit_count_t::PACK_24 : uniform_from_bits(bits)
        )
    {
        static_assert((bits == 24) || (uniform_from_bits(bits) != vexel_bit_count_t::NONE),
                    "Invalid bits value for vexel_structure_t");
    }
    // Accessors
    constexpr auto layout() const -> vexel_layout_t { return static_cast<vexel_layout_t>(get_bits(code, LAYOUT_MASK, LAYOUT_SHIFT)); }
    constexpr auto layout(vexel_layout_t v) -> vexel_structure_t& { code = set_bits(code, LAYOUT_MASK, LAYOUT_SHIFT, static_cast<uint32_t>(v)); return *this; }

    constexpr auto is_signed() const -> bool { return (code & SIGN_MASK) != 0; }
    constexpr auto is_signed(bool v) -> vexel_structure_t& { code = v ? (code | SIGN_MASK) : (code & ~SIGN_MASK); return *this; }

    constexpr auto is_normalized() const -> bool { return (code & NORM_MASK) != 0; }
    constexpr auto is_normalized(bool v) -> vexel_structure_t& { code = v ? (code | NORM_MASK) : (code & ~NORM_MASK); return *this; }

    constexpr auto number_format() const -> vexel_number_format_t { return static_cast<vexel_number_format_t>(get_bits(code, NFMT_MASK, NFMT_SHIFT)); }
    constexpr auto number_format(vexel_number_format_t v) -> vexel_structure_t& { code = set_bits(code, NFMT_MASK, NFMT_SHIFT, static_cast<uint32_t>(v)); return *this; }

    constexpr auto bit_count() const -> vexel_bit_count_t { return static_cast<vexel_bit_count_t>(get_bits(code, BCNT_MASK, BCNT_SHIFT)); }
    constexpr auto bit_count(vexel_bit_count_t v) -> vexel_structure_t& { code = set_bits(code, BCNT_MASK, BCNT_SHIFT, static_cast<uint32_t>(v)); return *this; }

    // Validación mínima de “estructura válida” (tu convención: layout != UNKNOWN)
    static constexpr bool validate(uint32_t v) {
        return ((v & LAYOUT_MASK) >> LAYOUT_SHIFT) != static_cast<uint32_t>(vexel_layout_t::UNKNOWN);
    }

    // Presets (ahora llaman al ctor que usa máscaras; no cambian)
    static constexpr vexel_structure_t VECx_F32 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), true,  false, vexel_number_format_t::FLOAT,   vexel_bit_count_t::UNIFORM_32); }
    static constexpr vexel_structure_t VECx_U8  (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_8 ); }
    static constexpr vexel_structure_t VECx_U16 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_16); }
    static constexpr vexel_structure_t VECx_U32 (int n) { return vexel_structure_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_32); }

    static constexpr vexel_structure_t VEC1_F32() { return VECx_F32(1); }
    static constexpr vexel_structure_t VEC2_F32() { return VECx_F32(2); }
    static constexpr vexel_structure_t VEC3_F32() { return VECx_F32(3); }
    static constexpr vexel_structure_t VEC4_F32() { return VECx_F32(4); }

    static constexpr vexel_structure_t VEC1_U8 () { return VECx_U8 (1); }
    static constexpr vexel_structure_t VEC2_U8 () { return VECx_U8 (2); }
    static constexpr vexel_structure_t VEC3_U8 () { return VECx_U8 (3); }
    static constexpr vexel_structure_t VEC4_U8 () { return VECx_U8 (4); }

    static constexpr vexel_structure_t VEC1_U16() { return VECx_U16(1); }
    static constexpr vexel_structure_t VEC1_U32() { return VECx_U32(1); }
};

// =======================
// Full vexel format
// =======================
struct vexel_format_t 
{
    uint32_t code_info;
    uint32_t code_structure;

    // Ctors
    constexpr vexel_format_t() : code_info(0), code_structure(0) {}
    constexpr vexel_format_t(vexel_info_t info, vexel_structure_t structure)
        : code_info(info.code), code_structure(structure.code) {}

    constexpr auto info() const -> vexel_info_t { return vexel_info_t(code_info); }
    constexpr auto info(vexel_info_t value) -> vexel_format_t& { code_info = value.code; return *this;}
    constexpr auto structure() const -> vexel_structure_t { return vexel_structure_t(code_structure); }
    constexpr auto structure(vexel_structure_t value) -> vexel_format_t& { code_structure = value.code; return *this;}
};

// Sane checks (tamaños)
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
