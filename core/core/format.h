#pragma once

#include <stdint.h>
#include <pre.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vexel format
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum class vexel_layout_t
{
    UNKNOWN = 0,
    SINGLE_ELEMENT = 1,
    VECTOR_2 = 2,
    VECTOR_3 = 3,
    VECTOR_4 = 4,
    MATRIX_4x4 = 5, // COLUMNASxFILAS
    MATRIX_4x3 = 6,
    MATRIX_4x2 = 7,
    MATRIX_3x3 = 8,
    MATRIX_2x2 = 9,
};

enum class vexel_number_format_t
{
    UNKNOWN = 0,
    INTEGER = 1,
    FLOAT = 2,
    FIXED = 3,
    TYPELESS = 4
};

enum class vexel_space_t
{
    LINEAR,
    SRGB,
    EXTENDED_SRGB,
    ADOBE_RGB,
    HDR_EBGR,
    HDR10_ST2084,
    HDR10_HLG,
    YUV_420_G_B_R_3PLANE,
    YUV_420_G_BR_2PLANE,
    YUV_422_G_B_R_3PLANE,
    YUV_422_G_BR_2PLANE,
    YUV_422_GBGR,
    YUV_422_BGRG,
    YUV_444_G_B_R_3PLANE,
    YUV_444_G_BR_2PLANE,
    DISPLAY_P3,
    DCI_P3,
    BT709,
    BT2020,
    DOLBYVISION,
    PASS_THROUGH,
    DISPLAY_NATIVE_AMD
};

enum class vexel_compressed_format_t
{
    // Sin compresión (packed pixel formats)
    UNDEFINED,
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


enum class vexel_element_bit_count_t
{
    BIT_COUNT_8 = 0,
    BIT_COUNT_16 = 1,
    BIT_COUNT_32 = 2,
    BIT_COUNT_64 = 3,
    BIT_COUNT_128 = 4,
    BIT_COUNT_256 = 5,
    BIT_COUNT_512 = 6,
    BIT_COUNT_1024 = 7,
};

enum class vexel_order_t
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
// Buffer format: what is inside the buffer, packed into a 32-bit code.
// ----------------------------------------------------------------------------------------------------------------------------
// - **Bit 31**: `is_compressed`
//   - **1** → Formato **comprimido**:
//       - **Bits 30–0** (31 bits) : `vexel_compressed_format_t`
//   - **0** → Formato **no comprimido**:
//       - **Bits 30–22** (9 bits) : `vexel_layout_t`
//           → distribución de componentes por elemento (SINGLE, VECTOR_2, VECTOR_3, VECTOR_4, matrices, empaquetados, etc.)
//       - **Bits 21–13** (9 bits) : `vexel_space_t`
//           → espacio de color o numérico (LINEAR, SRGB, ADOBE_RGB, YUV variantes, DEPTH, etc.)
//       - **Bits 12–9**  (4 bits) : `vexel_order_t`
//           → orden de canales / mapeo (R, RG, RGB, RGBA, BGRA, YUV, etc.)
//       - **Bit 8**      (1 bit)  : `is_linear`
//           → 1 si el espacio es lineal, 0 si es no lineal (p.ej., sRGB, gamma corregida).
//       - **Bit 7**      (1 bit)  : `is_normalized`
//           → 1 si la conversión a float normaliza (UNORM/SNORM).
//       - **Bit 6**      (1 bit)  : `is_signed`
//           → 1 si el tipo base es con signo (SNORM/SINT/SFLOAT).
//       - **Bits 5–3**   (3 bits) : `vexel_number_format_t`
//           → clase numérica (INTEGER, FLOAT, FIXED, etc.).
//       - **Bits 2–0**   (3 bits) : `vexel_element_bit_count_t`
//           → tamaño base por componente (codificado por enum; p.ej., 8/16/32 bits).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct vexel_format_t
{
    uint32_t code;

private:
    // Bit 31: is_compressed
    static constexpr uint32_t IS_COMPRESSED_SHIFT = 31;
    static constexpr uint32_t IS_COMPRESSED_MASK  = 1u << IS_COMPRESSED_SHIFT;

    // Compressed (bits 0–30): vexel_compressed_format_t (31 bits)
    static constexpr uint32_t COMPRESSED_FORMAT_SHIFT = 0;
    static constexpr uint32_t COMPRESSED_FORMAT_MASK  = 0x7FFFFFFFu << COMPRESSED_FORMAT_SHIFT;

    // Uncompressed layout (bits 0–30):
    // [30:22] layout(9) | [21:13] space(9) | [12:9] order(4) | [8] is_linear(1) | [7] is_norm(1) | [6] is_sign(1) | [5:3] numFmt(3) | [2:0] bitCount(3)
    static constexpr uint32_t VEXEL_LAYOUT_SHIFT = 22;
    static constexpr uint32_t VEXEL_LAYOUT_MASK  = 0x1FFu << VEXEL_LAYOUT_SHIFT;  // 9 bits

    static constexpr uint32_t VEXEL_SPACE_SHIFT  = 13;
    static constexpr uint32_t VEXEL_SPACE_MASK   = 0x1FFu << VEXEL_SPACE_SHIFT;   // 9 bits (ANTES 10)

    static constexpr uint32_t VEXEL_ORDER_SHIFT  = 9;
    static constexpr uint32_t VEXEL_ORDER_MASK   = 0xFu << VEXEL_ORDER_SHIFT;     // 4 bits

    static constexpr uint32_t IS_LINEAR_SHIFT     = 8;   // NUEVO
    static constexpr uint32_t IS_LINEAR_MASK      = 1u << IS_LINEAR_SHIFT;

    static constexpr uint32_t IS_NORMALIZED_SHIFT = 7;
    static constexpr uint32_t IS_NORMALIZED_MASK  = 1u << IS_NORMALIZED_SHIFT;

    static constexpr uint32_t IS_SIGNED_SHIFT     = 6;
    static constexpr uint32_t IS_SIGNED_MASK      = 1u << IS_SIGNED_SHIFT;

    static constexpr uint32_t NUMBER_FORMAT_SHIFT = 3;
    static constexpr uint32_t NUMBER_FORMAT_MASK  = 0x7u << NUMBER_FORMAT_SHIFT;  // 3 bits

    static constexpr uint32_t ELEMENT_BIT_COUNT_SHIFT = 0;
    static constexpr uint32_t ELEMENT_BIT_COUNT_MASK  = 0x7u << ELEMENT_BIT_COUNT_SHIFT; // 3 bits

    // Helpers
    static constexpr auto get_flag(uint32_t c, uint32_t mask) -> bool { return (c & mask) != 0; }
    static constexpr auto set_flag(uint32_t c, uint32_t mask, bool v) -> uint32_t { return (c & ~mask) | (uint32_t(v) ? mask : 0u); }
    static constexpr auto get_bits(uint32_t c, uint32_t mask, uint32_t shift) -> uint32_t { return (c & mask) >> shift; }
    static constexpr auto set_bits(uint32_t c, uint32_t mask, uint32_t shift, uint32_t v) -> uint32_t { return (c & ~mask) | ((v << shift) & mask); }
    template<class E> static constexpr auto get_enum(uint32_t c, uint32_t mask, uint32_t shift) -> E { return static_cast<E>(get_bits(c, mask, shift)); }
    template<class E> static constexpr auto enc(E v) -> uint32_t { return static_cast<uint32_t>(v); }

public:
    // Constructores
    constexpr vexel_format_t() : code(0) {}
    constexpr vexel_format_t(uint32_t initial_code) : code(initial_code) {}
    constexpr vexel_format_t(vexel_compressed_format_t cf) { code = (1u << IS_COMPRESSED_SHIFT) | (enc(cf) << COMPRESSED_FORMAT_SHIFT); }
    constexpr vexel_format_t(vexel_layout_t layout, vexel_space_t space, vexel_order_t order,
                             bool is_signed_, bool is_linear_, bool is_normalized_,
                             vexel_number_format_t num_format, vexel_element_bit_count_t bit_count)
    {
        code =
            (enc(layout)     << VEXEL_LAYOUT_SHIFT)     |
            (enc(space)      << VEXEL_SPACE_SHIFT)      |
            (enc(order)      << VEXEL_ORDER_SHIFT)      |
            (uint32_t(is_linear_)     << IS_LINEAR_SHIFT)     |
            (uint32_t(is_normalized_) << IS_NORMALIZED_SHIFT) |
            (uint32_t(is_signed_)     << IS_SIGNED_SHIFT)     |
            (enc(num_format) << NUMBER_FORMAT_SHIFT)     |
            (enc(bit_count)  << ELEMENT_BIT_COUNT_SHIFT);
    }

    // Accessors
    constexpr auto is_compressed() const -> bool { return get_flag(code, IS_COMPRESSED_MASK); }
    constexpr auto is_compressed(bool v) -> vexel_format_t& { code = set_flag(code, IS_COMPRESSED_MASK, v); return *this; }

    constexpr auto compressed_format() const -> vexel_compressed_format_t { return get_enum<vexel_compressed_format_t>(code, COMPRESSED_FORMAT_MASK, COMPRESSED_FORMAT_SHIFT); }
    constexpr auto compressed_format(vexel_compressed_format_t v) -> vexel_format_t& { code = set_bits(code, COMPRESSED_FORMAT_MASK, COMPRESSED_FORMAT_SHIFT, enc(v)); return *this; }

    constexpr auto layout() const -> vexel_layout_t { return get_enum<vexel_layout_t>(code, VEXEL_LAYOUT_MASK, VEXEL_LAYOUT_SHIFT); }
    constexpr auto layout(vexel_layout_t v) -> vexel_format_t& { code = set_bits(code, VEXEL_LAYOUT_MASK, VEXEL_LAYOUT_SHIFT, enc(v)); return *this; }

    constexpr auto space() const -> vexel_space_t { return get_enum<vexel_space_t>(code, VEXEL_SPACE_MASK, VEXEL_SPACE_SHIFT); }
    constexpr auto space(vexel_space_t v) -> vexel_format_t& { code = set_bits(code, VEXEL_SPACE_MASK, VEXEL_SPACE_SHIFT, enc(v)); return *this; }

    constexpr auto order() const -> vexel_order_t { return get_enum<vexel_order_t>(code, VEXEL_ORDER_MASK, VEXEL_ORDER_SHIFT); }
    constexpr auto order(vexel_order_t v) -> vexel_format_t& { code = set_bits(code, VEXEL_ORDER_MASK, VEXEL_ORDER_SHIFT, enc(v)); return *this; }

    constexpr auto is_linear() const -> bool { return get_flag(code, IS_LINEAR_MASK); }
    constexpr auto is_linear(bool v) -> vexel_format_t& { code = set_flag(code, IS_LINEAR_MASK, v); return *this; }

    constexpr auto is_normalized() const -> bool { return get_flag(code, IS_NORMALIZED_MASK); }
    constexpr auto is_normalized(bool v) -> vexel_format_t& { code = set_flag(code, IS_NORMALIZED_MASK, v); return *this; }

    constexpr auto is_signed() const -> bool { return get_flag(code, IS_SIGNED_MASK); }
    constexpr auto is_signed(bool v) -> vexel_format_t& { code = set_flag(code, IS_SIGNED_MASK, v); return *this; }

    constexpr auto number_format() const -> vexel_number_format_t { return get_enum<vexel_number_format_t>(code, NUMBER_FORMAT_MASK, NUMBER_FORMAT_SHIFT); }
    constexpr auto number_format(vexel_number_format_t v) -> vexel_format_t& { code = set_bits(code, NUMBER_FORMAT_MASK, NUMBER_FORMAT_SHIFT, enc(v)); return *this; }

    constexpr auto element_bit_count() const -> vexel_element_bit_count_t { return get_enum<vexel_element_bit_count_t>(code, ELEMENT_BIT_COUNT_MASK, ELEMENT_BIT_COUNT_SHIFT); }
    constexpr auto element_bit_count(vexel_element_bit_count_t v) -> vexel_format_t& { code = set_bits(code, ELEMENT_BIT_COUNT_MASK, ELEMENT_BIT_COUNT_SHIFT, enc(v)); return *this; }

    // Utilidades
    constexpr auto element_bits() const -> int { return 1 << (int(element_bit_count()) + 3); }
    constexpr auto element_bytes() const -> int { return element_bits() / 8; }
};







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
