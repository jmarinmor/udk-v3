#pragma once


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
	NONE = 0,
	NEAREST = 1,
	LINEAR = 2
};

enum class sampler_wrap_mode_t
{
	REPEAT = 0,
	MIRROR = 1,
	CLAMP_TO_BORDER,
	CLAMP_TO_EDGE,
	MIRRORED_REPEAT
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
