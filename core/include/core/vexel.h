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

enum class vexel_bit_format_t : uint16_t
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
    SINGLE,
    STRUCT_PART
};

enum class vexel_resource_t : uint8_t
{
    UNKNOWN,
    ARRAY_1D = 1,
    ARRAY_2D = 2,
    ARRAY_3D = 3,
    CUBE = 6
};

// SUGERENCIA: si quieres reforzar el contrato de 8 bits, puedes usar : uint8_t
enum media_format_t : uint16_t
{
    UNKNOWN = 0,

    EXTERNAL_ID = 1,

    // VEXEL/recursos
    VEXEL_RESOURCE,
    VEXEL_RESOURCE_ARRAY_1D,

    // Imagen raster comunes
    JPEG,
    TGA,
    TIFF,
    PNG,
    BMP,
    GIF,
    WEBP,
    HEIF,
    HEIC,
    AVIF,

    // HDR / alto rango dinámico
    HDR_RADIANCE,   // .hdr / .rgbe
    OPENEXR,        // .exr

    // Contenedores de texturas
    DDS,            // DirectDraw Surface
    KTX,            // Khronos Texture
    KTX2,

    // Iconografía
    ICO,
    CUR,

    // Vectoriales / documento
    SVG,
    PDF,

    // Compresiones específicas (ya tenías algunas)
    DXT1,
    DXT5,
    PVR2,
    PVR4,

    // 3D/escena
    GLTF,
    GLB,
    OBJ,
    FBX,
    STL,
    PLY,

    // Audio
    WAV,
    OGG,    // contenedor; suele albergar Vorbis/Opus
    MP3,
    FLAC,
    AAC,
    OPUS,
    M4A,
    AIFF,
    CAF,
    MIDI,

    // Vídeo
    MP4,
    MOV,
    MKV,
    AVI,
    WEBM
};


/*
===============================================================================
vexel_format_t — Layout de bits (uint64_t)
-------------------------------------------------------------------------------
Orden de bits: MSB → LSB
-------------------------------------------------------------------------------
[63:61]  _pad            (3)   Reservado (sin uso actual).
[60:51]  space           (10)  vexel_space_t — Espacio de color (RGB, YUV, ...).
[50:46]  order           (5)   vexel_order_t — Orden de componentes (RGBA, BGRA, ...).
[45:42]  interpolation   (4)   vexel_interpolation_t — LINEAR / NONLINEAR.
[41:34]  media_format    (8)   media_format_t — Formato/contenedor (PNG, JPEG, WAV, ...).
[33:30]  resource        (4)   vexel_resource_t — ARRAY_1D/2D/3D, CUBE.
[29]     has_lods        (1)   1 si el recurso incluye niveles de detalle (LODs).
[28:26]  structure       (3)   vexel_struct_t — UNKNOWN/ONE/MANY.
[25:16]  layout          (10)  vexel_layout_t — VECTOR_n, MATRIX, BLOCK_* (BCn/ASTC/etc.).
[15]     is_signed       (1)   1 si los datos son con signo.
[14]     is_normalized   (1)   1 si los datos están normalizados.
[13:10]  number_format   (4)   vexel_number_format_t — INTEGER/FLOAT/FIXED/TYPELESS.
[9:0]    bit_count       (10)  vexel_bit_count_t — bits por elemento o empaquetado.
===============================================================================

Constantes de desplazamiento (SHIFT) y máscara (MASK)
-------------------------------------------------------------------------------
Nombre    | Bits    | SHIFT | MASK (hex)
----------+---------+-------+---------------------------
PAD       | 63:61   |   61  | 0xE000000000000000
SPACE     | 60:51   |   51  | 0x1FF8000000000000
ORDER     | 50:46   |   46  | 0x007C000000000000
INTP      | 45:42   |   42  | 0x00003C0000000000
MEDIA_FMT | 41:34   |   34  | 0x000003FC00000000
RESOURCE  | 33:30   |   30  | 0x00000003C0000000
HAS_LODS  | 29      |   29  | 0x0000000200000000
STRUCTURE | 28:26   |   26  | 0x00000001C0000000
LAYOUT    | 25:16   |   16  | 0x0000000000FFF000
IS_SIGNED | 15      |   15  | 0x0000000000008000
IS_NORM   | 14      |   14  | 0x0000000000004000
NUM_FMT   | 13:10   |   10  | 0x0000000000003C00
BIT_COUNT | 9:0     |    0  | 0x00000000000003FF

Notas:
- El empaquetado ocupa 64 bits; los 3 bits superiores (_pad) quedan reservados.
- Los campos enumerados (space/order/...) se guardan como sus valores integrales.
- 'media_format' dispone de 8 bits (hasta 256 valores distintos).

Helpers típicos (patrón de uso)
-------------------------------------------------------------------------------
- Obtener un campo:
    value = (code & FIELD_MASK) >> FIELD_SHIFT;

- Escribir un campo (sin afectar otros):
    code = (code & ~FIELD_MASK) | ((uint64_t(value) << FIELD_SHIFT) & FIELD_MASK);

Composición y parciales (recomendación)
-------------------------------------------------------------------------------
- Es común construir formatos “parciales” (semántica / recurso / estructura) y
  fusionarlos con OR enmascarado. Por ejemplo:
    code = (semantic.code  & (SPACE_MASK | ORDER_MASK | INTP_MASK)) |
           (resource.code  & (MFMT_MASK  | RSRC_MASK | LODS_MASK | STRC_MASK)) |
           (struct.code    & (LOUT_MASK  | SIGN_MASK | NORM_MASK | NFMT_MASK | BCNT_MASK));

Validación mínima sugerida
-------------------------------------------------------------------------------
- Considerar válido si layout != vexel_layout_t::UNKNOWN:
    bool ok = ((code & LOUT_MASK) >> LOUT_SHIFT) != uint64_t(vexel_layout_t::UNKNOWN);
===============================================================================
*/
struct vexel_format_t;
static constexpr vexel_format_t merge(const vexel_format_t& semantic,
                                             const vexel_format_t& resource,
                                             const vexel_format_t& structural);
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
        vexel_bit_format_t bit_count
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
        vexel_bit_format_t bit_count
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
    inline vexel_bit_format_t       bit_count()     const { return static_cast<vexel_bit_format_t>(get_bits(code, BCNT_MASK, BCNT_SHIFT)); }

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
    inline vexel_format_t& bit_count(vexel_bit_format_t v)         { code = set_bits(code, BCNT_MASK, BCNT_SHIFT, (uint64_t)v); return *this; }

    // Helpers
    static inline vexel_bit_format_t uniform_from_bits(int bits) {
        return (bits == 8) ? vexel_bit_format_t::UNIFORM_8 :
               (bits == 16)? vexel_bit_format_t::UNIFORM_16 :
               (bits == 32)? vexel_bit_format_t::UNIFORM_32 :
               (bits == 64)? vexel_bit_format_t::UNIFORM_64 :
               (bits == 128)?vexel_bit_format_t::UNIFORM_128 :
               (bits == 256)?vexel_bit_format_t::UNIFORM_256 :
               (bits == 512)?vexel_bit_format_t::UNIFORM_512 :
               (bits == 1024)?vexel_bit_format_t::UNIFORM_1024 :
               vexel_bit_format_t::NONE;
    }
    static inline bool validate(uint64_t v) {
        return ((v & LOUT_MASK) >> LOUT_SHIFT) != static_cast<uint64_t>(vexel_layout_t::UNKNOWN);
    }

    // Presets
	static constexpr vexel_format_t VECx_Fx(int n, vexel_bit_format_t bit_format) { return vexel_format_t(static_cast<vexel_layout_t>(n), true, false, vexel_number_format_t::FLOAT, bit_format); }
	static constexpr vexel_format_t VECx_Ux(int n, vexel_bit_format_t bit_format) { return vexel_format_t(static_cast<vexel_layout_t>(n), false, false, vexel_number_format_t::INTEGER, bit_format); }
	static constexpr vexel_format_t VECx_Ix(int n, vexel_bit_format_t bit_format) { return vexel_format_t(static_cast<vexel_layout_t>(n), true, false, vexel_number_format_t::INTEGER, bit_format); }
	static constexpr vexel_format_t VECx_UNx(int n, vexel_bit_format_t bit_format) { return vexel_format_t(static_cast<vexel_layout_t>(n), false, true, vexel_number_format_t::INTEGER, bit_format); }
	

    #define VEXEL_FORMAT_MAKE_PRESET(_sufix, _bit_count) \
    static constexpr vexel_format_t VECx_##_sufix##_bit_count(int n) { return VECx_##_sufix##x(n, vexel_bit_format_t::UNIFORM_##_bit_count); } \
	static constexpr vexel_format_t VEC1_##_sufix##_bit_count() { return VECx_##_sufix##_bit_count(1); } \
	static constexpr vexel_format_t VEC2_##_sufix##_bit_count() { return VECx_##_sufix##_bit_count(2); } \
	static constexpr vexel_format_t VEC3_##_sufix##_bit_count() { return VECx_##_sufix##_bit_count(3); } \
	static constexpr vexel_format_t VEC4_##_sufix##_bit_count() { return VECx_##_sufix##_bit_count(4); }

    VEXEL_FORMAT_MAKE_PRESET(F, 32)
    VEXEL_FORMAT_MAKE_PRESET(F, 64)
    VEXEL_FORMAT_MAKE_PRESET(U, 8)
    VEXEL_FORMAT_MAKE_PRESET(U, 16)
    VEXEL_FORMAT_MAKE_PRESET(U, 32)
    VEXEL_FORMAT_MAKE_PRESET(U, 64)
    VEXEL_FORMAT_MAKE_PRESET(UN, 8)
    VEXEL_FORMAT_MAKE_PRESET(UN, 16)
    VEXEL_FORMAT_MAKE_PRESET(UN, 32)
    VEXEL_FORMAT_MAKE_PRESET(UN, 64)
    VEXEL_FORMAT_MAKE_PRESET(I, 8)
    VEXEL_FORMAT_MAKE_PRESET(I, 16)
    VEXEL_FORMAT_MAKE_PRESET(I, 32)
    VEXEL_FORMAT_MAKE_PRESET(I, 64)

	static constexpr vexel_format_t TEXTURE(int dimension, bool has_lods = true, vexel_format_t format = vexel_format_t(0)) 
    {            
         return vexel_format_t(
            vexel_format_t(vexel_space_t::RGB, vexel_order_t::RGBA, vexel_interpolation_t::LINEAR).code
            |
            vexel_format_t(media_format_t::VEXEL_RESOURCE, vexel_resource_t(dimension), has_lods, vexel_struct_t::ONE).code
            |
            format.code
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

static inline constexpr vexel_format_t merge(const vexel_format_t& semantic,
                                             const vexel_format_t& resource,
                                             const vexel_format_t& structural) {
#if !defined(NDEBUG)
    assert(vexel_format_t::fits_group(semantic.code,  vexel_format_t::SEM_MASK) && "semantic partial has bits outside SEM_MASK");
    assert(vexel_format_t::fits_group(resource.code,  vexel_format_t::RES_MASK) && "resource partial has bits outside RES_MASK");
    assert(vexel_format_t::fits_group(structural.code, vexel_format_t::STR_MASK) && "structural partial has bits outside STR_MASK");
#endif
    return vexel_format_t{
        (semantic.code  & vexel_format_t::SEM_MASK) |
        (resource.code  & vexel_format_t::RES_MASK) |
        (structural.code & vexel_format_t::STR_MASK)
    };
}