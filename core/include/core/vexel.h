#pragma once

#include <stdint.h>
#include <cassert>

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

enum class vexel_order_t : uint8_t
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

enum class vexel_struct_t : uint8_t
{
    UNKNOWN,
    ONE,
    MANY
};

enum class vexel_resource_t : uint8_t
{
    UNKNOWN,
    ARRAY_1D,
    ARRAY_2D,
    ARRAY_3D,
    CUBE
};

enum media_format_t : uint16_t
{
    UNKNOWN,
    VEXEL_RESOURCE,
    VEXEL_RESOURCE_ARRAY_1D,
    JPEG,
    PNG,
    DXT1,
    DXT5,
    PVR2,
    PVR4,
    WAV,
    OGG,
    MP3
};


struct vexel_format_t {
    uint64_t code;

    // Shifts & masks según el orden pedido
    static constexpr uint64_t SPACE_SHIFT = 51, SPACE_MASK = 0x3FFull << SPACE_SHIFT;
    static constexpr uint64_t ORDER_SHIFT = 46, ORDER_MASK = 0x1Full << ORDER_SHIFT;
    static constexpr uint64_t INTP_SHIFT  = 42, INTP_MASK  = 0xFull << INTP_SHIFT;
    static constexpr uint64_t MFMT_SHIFT  = 34, MFMT_MASK  = 0xFFull << MFMT_SHIFT;
    static constexpr uint64_t RSRC_SHIFT  = 30, RSRC_MASK  = 0xFull << RSRC_SHIFT;
    static constexpr uint64_t LODS_SHIFT  = 29, LODS_MASK  = 0x1ull << LODS_SHIFT;
    static constexpr uint64_t STRC_SHIFT  = 26, STRC_MASK  = 0x7ull << STRC_SHIFT;
    static constexpr uint64_t LOUT_SHIFT  = 16, LOUT_MASK  = 0x3FFull << LOUT_SHIFT;
    static constexpr uint64_t SIGN_SHIFT  = 15, SIGN_MASK  = 0x1ull << SIGN_SHIFT;
    static constexpr uint64_t NORM_SHIFT  = 14, NORM_MASK  = 0x1ull << NORM_SHIFT;
    static constexpr uint64_t NFMT_SHIFT  = 10, NFMT_MASK  = 0xFull << NFMT_SHIFT;
    static constexpr uint64_t BCNT_SHIFT  = 0,  BCNT_MASK  = 0x3FFull << BCNT_SHIFT;

    static constexpr uint64_t SEM_MASK = SPACE_MASK | ORDER_MASK | INTP_MASK;
    static constexpr uint64_t RES_MASK = MFMT_MASK | RSRC_MASK | LODS_MASK | STRC_MASK;
    static constexpr uint64_t STR_MASK = LOUT_MASK | SIGN_MASK | NORM_MASK | NFMT_MASK | BCNT_MASK;

    static constexpr bool fits_group(uint64_t code, uint64_t group_mask) { return (code & ~group_mask) == 0;}
    static constexpr uint64_t get_bits(uint64_t v, uint64_t m, uint64_t s) { return (v & m) >> s; }
    static constexpr uint64_t set_bits(uint64_t v, uint64_t m, uint64_t s, uint64_t x) { return (v & ~m) | ((x << s) & m);}

    // Ctors
    constexpr inline vexel_format_t() : code(0) {}
    constexpr inline vexel_format_t(uint64_t c) : code(c) {}

    // Ctor completo
    constexpr inline vexel_format_t(
        vexel_space_t space,
        vexel_order_t order,
        vexel_interpolation_t interpolation,
        media_format_t media_format,
        vexel_resource_t resource,
        bool has_lods,
        vexel_struct_t structure,
        vexel_layout_t layout,
        bool is_signed,
        bool is_normalized,
        vexel_number_format_t number_format,
        vexel_bit_count_t bit_count
    ) : code(0) {
        code = set_bits(code, SPACE_MASK, SPACE_SHIFT, static_cast<uint64_t>(space));
        code = set_bits(code, ORDER_MASK, ORDER_SHIFT, static_cast<uint64_t>(order));
        code = set_bits(code, INTP_MASK,  INTP_SHIFT,  static_cast<uint64_t>(interpolation));
        code = set_bits(code, MFMT_MASK,  MFMT_SHIFT,  static_cast<uint64_t>(media_format));
        code = set_bits(code, RSRC_MASK,  RSRC_SHIFT,  static_cast<uint64_t>(resource));
        code = has_lods ? (code | LODS_MASK) : (code & ~LODS_MASK);
        code = set_bits(code, STRC_MASK,  STRC_SHIFT,  static_cast<uint64_t>(structure));
        code = set_bits(code, LOUT_MASK,  LOUT_SHIFT,  static_cast<uint64_t>(layout));
        code = is_signed ? (code | SIGN_MASK) : (code & ~SIGN_MASK);
        code = is_normalized ? (code | NORM_MASK) : (code & ~NORM_MASK);
        code = set_bits(code, NFMT_MASK,  NFMT_SHIFT,  static_cast<uint64_t>(number_format));
        code = set_bits(code, BCNT_MASK,  BCNT_SHIFT,  static_cast<uint64_t>(bit_count));
    }
    constexpr inline vexel_format_t(
        vexel_space_t space,
        vexel_order_t order,
        vexel_interpolation_t interpolation
    ) : code(0) {
        code = set_bits(code, SPACE_MASK, SPACE_SHIFT, static_cast<uint64_t>(space));
        code = set_bits(code, ORDER_MASK, ORDER_SHIFT, static_cast<uint64_t>(order));
        code = set_bits(code, INTP_MASK,  INTP_SHIFT,  static_cast<uint64_t>(interpolation));
    }
    constexpr inline vexel_format_t(
        media_format_t media_format,
        vexel_resource_t resource,
        bool has_lods,
        vexel_struct_t structure
    ) : code(0) {
        code = set_bits(code, MFMT_MASK, MFMT_SHIFT, static_cast<uint64_t>(media_format));
        code = set_bits(code, RSRC_MASK, RSRC_SHIFT, static_cast<uint64_t>(resource));
        code = has_lods ? (code | LODS_MASK) : (code & ~LODS_MASK);
        code = set_bits(code, STRC_MASK, STRC_SHIFT, static_cast<uint64_t>(structure));
    }
    constexpr inline vexel_format_t(
        vexel_layout_t layout,
        bool is_signed,
        bool is_normalized,
        vexel_number_format_t number_format,
        vexel_bit_count_t bit_count
    ) : code(0) {
        code = set_bits(code, LOUT_MASK, LOUT_SHIFT, static_cast<uint64_t>(layout));
        code = is_signed ? (code | SIGN_MASK) : (code & ~SIGN_MASK);
        code = is_normalized ? (code | NORM_MASK) : (code & ~NORM_MASK);
        code = set_bits(code, NFMT_MASK, NFMT_SHIFT, static_cast<uint64_t>(number_format));
        code = set_bits(code, BCNT_MASK, BCNT_SHIFT, static_cast<uint64_t>(bit_count));
    }
    constexpr inline vexel_format_t(const vexel_format_t& semantic,
                            const vexel_format_t& resource,
                            const vexel_format_t& structural)
    : code( ((semantic.code  & SEM_MASK) |
            (resource.code  & RES_MASK) |
            (structural.code & STR_MASK)) )
    {
    #if !defined(NDEBUG)
        // Detecta mal uso: bits fuera de su grupo
        assert(fits_group(semantic.code,  SEM_MASK) && "semantic partial has bits outside SEM_MASK");
        assert(fits_group(resource.code,  RES_MASK) && "resource partial has bits outside RES_MASK");
        assert(fits_group(structural.code, STR_MASK) && "structural partial has bits outside STR_MASK");
    #endif
    }

    // Getters
    inline vexel_space_t           space()         const { return static_cast<vexel_space_t>(get_bits(code, SPACE_MASK, SPACE_SHIFT)); }
    inline vexel_order_t           order()         const { return static_cast<vexel_order_t>(get_bits(code, ORDER_MASK, ORDER_SHIFT)); }
    inline vexel_interpolation_t   interpolation() const { return static_cast<vexel_interpolation_t>(get_bits(code, INTP_MASK, INTP_SHIFT)); }
    inline media_format_t          media_format()  const { return static_cast<media_format_t>(get_bits(code, MFMT_MASK, MFMT_SHIFT)); }
    inline vexel_resource_t        resource()      const { return static_cast<vexel_resource_t>(get_bits(code, RSRC_MASK, RSRC_SHIFT)); }
    inline bool                    has_lods()      const { return (code & LODS_MASK) != 0; }
    inline vexel_struct_t          structure()     const { return static_cast<vexel_struct_t>(get_bits(code, STRC_MASK, STRC_SHIFT)); }
    inline vexel_layout_t          layout()        const { return static_cast<vexel_layout_t>(get_bits(code, LOUT_MASK, LOUT_SHIFT)); }
    inline bool                    is_signed()     const { return (code & SIGN_MASK) != 0; }
    inline bool                    is_normalized() const { return (code & NORM_MASK) != 0; }
    inline vexel_number_format_t   number_format() const { return static_cast<vexel_number_format_t>(get_bits(code, NFMT_MASK, NFMT_SHIFT)); }
    inline vexel_bit_count_t       bit_count()     const { return static_cast<vexel_bit_count_t>(get_bits(code, BCNT_MASK, BCNT_SHIFT)); }

    // Setters fluentes
    inline vexel_format_t& space(vexel_space_t v)                 { code = set_bits(code, SPACE_MASK, SPACE_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& order(vexel_order_t v)                 { code = set_bits(code, ORDER_MASK, ORDER_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& interpolation(vexel_interpolation_t v) { code = set_bits(code, INTP_MASK, INTP_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& media_format(media_format_t v)         { code = set_bits(code, MFMT_MASK, MFMT_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& resource(vexel_resource_t v)           { code = set_bits(code, RSRC_MASK, RSRC_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& has_lods(bool v)                       { code = v ? (code | LODS_MASK) : (code & ~LODS_MASK); return *this; }
    inline vexel_format_t& structure(vexel_struct_t v)            { code = set_bits(code, STRC_MASK, STRC_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& layout(vexel_layout_t v)               { code = set_bits(code, LOUT_MASK, LOUT_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& is_signed(bool v)                      { code = v ? (code | SIGN_MASK) : (code & ~SIGN_MASK); return *this; }
    inline vexel_format_t& is_normalized(bool v)                  { code = v ? (code | NORM_MASK) : (code & ~NORM_MASK); return *this; }
    inline vexel_format_t& number_format(vexel_number_format_t v) { code = set_bits(code, NFMT_MASK, NFMT_SHIFT, (uint64_t)v); return *this; }
    inline vexel_format_t& bit_count(vexel_bit_count_t v)         { code = set_bits(code, BCNT_MASK, BCNT_SHIFT, (uint64_t)v); return *this; }

    // Helpers
    static inline vexel_bit_count_t uniform_from_bits(int bits) {
        return (bits == 8) ? vexel_bit_count_t::UNIFORM_8 :
               (bits == 16)? vexel_bit_count_t::UNIFORM_16 :
               (bits == 32)? vexel_bit_count_t::UNIFORM_32 :
               (bits == 64)? vexel_bit_count_t::UNIFORM_64 :
               (bits == 128)?vexel_bit_count_t::UNIFORM_128 :
               (bits == 256)?vexel_bit_count_t::UNIFORM_256 :
               (bits == 512)?vexel_bit_count_t::UNIFORM_512 :
               (bits == 1024)?vexel_bit_count_t::UNIFORM_1024 :
               vexel_bit_count_t::NONE;
    }
    static inline bool validate(uint64_t v) {
        return ((v & LOUT_MASK) >> LOUT_SHIFT) != static_cast<uint64_t>(vexel_layout_t::UNKNOWN);
    }

    // Presets
    static constexpr inline vexel_format_t VECx_F32(int n) {
        return vexel_format_t(
            vexel_space_t::RGB, vexel_order_t::RGBA, vexel_interpolation_t::LINEAR,
            media_format_t::VEXEL_RESOURCE, vexel_resource_t::UNKNOWN, false,
            vexel_struct_t::ONE, static_cast<vexel_layout_t>(n),
            true, false, vexel_number_format_t::FLOAT, vexel_bit_count_t::UNIFORM_32
        );
    }
    static constexpr inline vexel_format_t VECx_U8(int n) {
        return vexel_format_t(
            vexel_space_t::RGB, vexel_order_t::RGBA, vexel_interpolation_t::LINEAR,
            media_format_t::VEXEL_RESOURCE, vexel_resource_t::UNKNOWN, false,
            vexel_struct_t::ONE, static_cast<vexel_layout_t>(n),
            false, false, vexel_number_format_t::INTEGER, vexel_bit_count_t::UNIFORM_8
        );
    }
};

static_assert(sizeof(vexel_format_t) == 8, "vexel_format_t must be 64-bit");

constexpr vexel_format_t operator|(const vexel_format_t& lhs, const vexel_format_t& rhs) {
    return vexel_format_t{ lhs.code | rhs.code };
}

// AND: conserva solo los bits en común y devuelve uno nuevo
constexpr vexel_format_t operator&(const vexel_format_t& lhs, const vexel_format_t& rhs) {
    return vexel_format_t{ lhs.code & rhs.code };
}

static inline constexpr vexel_format_t merge(
    const vexel_format_t& semantic,
    const vexel_format_t& resource,
    const vexel_format_t& structural) {
#if !defined(NDEBUG)
    assert(fits_group(semantic.code,  SEM_MASK) && "semantic partial has bits outside SEM_MASK");
    assert(fits_group(resource.code,  RES_MASK) && "resource partial has bits outside RES_MASK");
    assert(fits_group(structural.code, STR_MASK) && "structural partial has bits outside STR_MASK");
#endif
    return vexel_format_t{
        (semantic.code  & SEM_MASK) |
        (resource.code  & RES_MASK) |
        (structural.code & STR_MASK)
    };
}