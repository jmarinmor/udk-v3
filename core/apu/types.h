#pragma once

#include <stdint.h>

namespace UDK
{
	enum class ProcessUnitType 
    {
        UNKNOWN = 0, 
        HOST = 1 << 0, 
        CENTRAL = 1 << 1, 
        GRAPHICS = 1 << 2, 
        AUDIO = 1 << 3, 
        VIDEO = 1 << 4
    };

	enum class ProcessUnitDriverType 
    { 
        VULKAN, 
        OPENGL, 
        OPENCL, 
        OPENAL, 
        DIRECTX, 
        METAL 
    };

    struct DriverInfo
    {
        int32_t Major;
        int32_t Minor;
        char Vendor[128];
        char Version[128];
        char Renderer[128];
    };    
}


namespace UDK
{
	enum class compare_op
	{
		NEVER = 0,
		LESS = 1,
		EQUAL = 2,
		LESS_OR_EQUAL = 3,
		GREATER = 4,
		NOT_EQUAL = 5,
		GREATER_OR_EQUAL = 6,
		ALWAYS = 7
	};

}


namespace UDK
{
    enum class PrimitiveType
    {
        POINTS = 0,
        LINES = 1, LINE_LOOP = 2, LINE_STRIP = 3,
        TRIANGLES = 4, TRIANGLE_STRIP = 5, TRIANGLE_FAN = 6,
        QUADS = 7,
        LINE_ADJACENCY = 10, LINE_STRIP_ADJACENCY = 11,
        TRIANGLE_ADJACENCY = 12, TRIANGLE_STRIP_ADJACENCY = 13,
        PATCHES = 14
    };

    enum class BlendFactor
    {
        ZERO = 0, ONE = 1,
        SRC_COLOR = 100, SRC_ALPHA = 200, DST_COLOR = 300, DST_ALPHA = 400,
        CONSTANT_COLOR = 600, CONSTANT_ALPHA = 700,
        SRC1_ALPHA = 800, SRC1_COLOR = 900,
        SATURATE = 10000
    };

    enum class BlendOp
    {
        ADD, SUBTRACT, REVERSE_SUBTRACT, MIN, MAX
    };

    enum class CullMode
    {
        NONE, CCW, CW
    };

}


namespace UDK
{
    enum class PlayState
    {
        LOADED, PAUSED, PLAYING, STOPPED
    };

	enum queue_type_t 
    {
        UNKNOWN_QUEUE = 0, 
		TRANSFER_QUEUE = 1 << 0, RENDER_QUEUE = 1 << 1, ENCODE_QUEUE = 1 << 2, DECODE_QUEUE = 1 << 3, COMPUTE_QUEUE = 1 << 4, 
		SPARSE_RESOURCES = 1 << 5, PROTECTED_RESOURCES = 1 << 6, VIDE_RESOURCES = 1 << 7 
    };


	enum class present_mode 
	{
		IMMEDIATE = 0, 
		MAILBOX = 1, 
		FIFO = 2, 
		FIFO_RELAXED = 3, 
		SHARED_DEMAND_REFRESH = 1000111000, 
		SHARED_CONTINUOUS_REFRESH = 1000111001 
	};



}















/*


namespace rav { namespace hal {
	
	enum coordinate_system_e 
	{
		CANVAS, SHAPE, RECT 
	};

	enum brush_type_e { COLOR };
	enum standar_matrix_t { IDENTITY, TARGET_TO_HOMOGENEUS, HOMOGENEUS_TO_TARGET, RECT_TO_HOMOGENEUS, HOMOGENEUS_TO_RECT, SHAPE_TO_HOMOGENEUS, HOMOGENEUS_TO_SHAPE };
	
	enum class resource_view
	{
		IMAGE_1D = 0, 
		IMAGE_2D = 1, 
		IMAGE_3D = 2, 
		IMAGE_CUBE = 3, 
		IMAGE_1D_ARRAY = 4, 
		IMAGE_2D_ARRAY = 5, 
		IMAGECUBE_ARRAY = 6 
	};
	
	enum class frame_attachment 
	{
		COLOR = 0x00000001, 
		DEPTH = 0x00000002, 
		STENCIL = 0x00000004
	};
	

	


	enum class image_tiling 
	{
		OPTIMAL = 0,
		LINEAR = 1,
		DRM_FORMAT_MODIFIER = 1000158000
	};

	enum class format_feature 
	{
		SAMPLED_IMAGE = 0x00000001,
		STORAGE_IMAGE = 0x00000002,
		STORAGE_IMAGE_ATOMIC = 0x00000004,
		UNIFORM_TEXEL_BUFFER = 0x00000008,
		STORAGE_TEXEL_BUFFER = 0x00000010,
		STORAGE_TEXEL_BUFFER_ATOMIC = 0x00000020,
		VERTEX_BUFFER = 0x00000040,
		COLOR_ATTACHMENT = 0x00000080,
		COLOR_ATTACHMENT_BLEND = 0x00000100,
		DEPTH_STENCIL_ATTACHMENT = 0x00000200,
		BLIT_SRC = 0x00000400,
		BLIT_DST = 0x00000800,
		SAMPLED_IMAGE_FILTER_LINEAR = 0x00001000,
		TRANSFER_SRC = 0x00004000,
		TRANSFER_DST = 0x00008000,
		MIDPOINT_CHROMA_SAMPLES = 0x00020000,
		SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER = 0x00040000,
		SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER = 0x00080000,
		SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT = 0x00100000,
		SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE = 0x00200000,
		DISJOINT = 0x00400000,
		COSITED_CHROMA_SAMPLES = 0x00800000,
		SAMPLED_IMAGE_FILTER_MINMAX = 0x00010000,
		SAMPLED_IMAGE_FILTER_CUBIC_IMG = 0x00002000,
		VIDEO_DECODE_OUTPUT = 0x02000000,
		VIDEO_DECODE_DPB = 0x04000000,
		ACCELERATION_STRUCTURE_VERTEX_BUFFER = 0x20000000,
		FRAGMENT_DENSITY_MAP = 0x01000000,
		FRAGMENT_SHADING_RATE_ATTACHMENT = 0x40000000,
		VIDEO_ENCODE_INPUT = 0x08000000,
		VIDEO_ENCODE_DPB = 0x10000000
	};

	enum class memory_property {
		DEVICE_LOCAL = 0x00000001,
		HOST_VISIBLE = 0x00000002,
		HOST_COHERENT = 0x00000004,
		HOST_CACHED = 0x00000008,
		LAZILY_ALLOCATED = 0x00000010,
		PROTECTED = 0x00000020,
		DEVICE_COHERENT_AMD = 0x00000040,
		DEVICE_UNCACHED_AMD = 0x00000080,
		RDMA_CAPABLE_NV = 0x00000100
	};

	enum class sampler_filter
	{
		NEAREST = 0,
		LINEAR = 1,
		CUBIC_IMG = 1000015000
	};

	enum class mipmap_filter
	{
		NEAREST = 0,
		LINEAR = 1
	};

	enum class sampler_address
	{
		REPEAT = 0,
		MIRRORED_REPEAT = 1,
		CLAMP_TO_EDGE = 2,
		CLAMP_TO_BORDER = 3,
		MIRROR_CLAMP_TO_EDGE = 4
	};

	enum class image_layout
	{
    	UNDEFINED = 0,
	    GENERAL = 1,
		COLOR_ATTACHMENT_OPTIMAL = 2,
		DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
		DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
		SHADER_READ_ONLY_OPTIMAL = 5,
		TRANSFER_SRC_OPTIMAL = 6,
		TRANSFER_DST_OPTIMAL = 7,
		PREINITIALIZED = 8,
		DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 1000117000,
		DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 1000117001,
		DEPTH_ATTACHMENT_OPTIMAL = 1000241000,
		DEPTH_READ_ONLY_OPTIMAL = 1000241001,
		STENCIL_ATTACHMENT_OPTIMAL = 1000241002,
		STENCIL_READ_ONLY_OPTIMAL = 1000241003,
		PRESENT_SRC = 1000001002,
		VIDEO_DECODE_DST = 1000024000,
		VIDEO_DECODE_SRC = 1000024001,
		VIDEO_DECODE_DPB = 1000024002,
		SHARED_PRESENT = 1000111000,
		FRAGMENT_DENSITY_MAP_OPTIMAL = 1000218000,
		FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL = 1000164003,
		VIDEO_ENCODE_DST = 1000299000,
		VIDEO_ENCODE_SRC = 1000299001,
		VIDEO_ENCODE_DPB = 1000299002,
		READ_ONLY_OPTIMAL = 1000314000,
		ATTACHMENT_OPTIMAL = 1000314001
	};

	enum class load_op
	{
		LOAD = 0,
		CLEAR = 1,
		DONT_CARE = 2,
		NONE = 1000400000
	};

	enum class store_op 
	{
		STORE = 0,
		DONT_CARE = 1,
		NONE = 1000301000
	};

	enum class sample_count 
	{
		COUNT_1 = 0x00000001,
		COUNT_2 = 0x00000002,
		COUNT_4 = 0x00000004,
		COUNT_8 = 0x00000008,
		COUNT_16 = 0x00000010,
		COUNT_32 = 0x00000020,
		COUNT_64 = 0x00000040
	};

	enum class pipeline_stage 
	{
		TOP_OF_PIPE = 0x00000001,
		DRAW_INDIRECT = 0x00000002,
		VERTEX_INPUT = 0x00000004,
		VERTEX_SHADER = 0x00000008,
		TESSELLATION_CONTROL_SHADER = 0x00000010,
		TESSELLATION_EVALUATION_SHADER = 0x00000020,
		GEOMETRY_SHADER = 0x00000040,
		FRAGMENT_SHADER = 0x00000080,
		EARLY_FRAGMENT_TESTS = 0x00000100,
		LATE_FRAGMENT_TESTS = 0x00000200,
		COLOR_ATTACHMENT_OUTPUT = 0x00000400,
		COMPUTE_SHADER = 0x00000800,
		TRANSFER = 0x00001000,
		BOTTOM_OF_PIPE = 0x00002000,
		HOST = 0x00004000,
		ALL_GRAPHICS = 0x00008000,
		ALL_COMMANDS = 0x00010000,
		TRANSFORM_FEEDBACK = 0x01000000,
		CONDITIONAL_RENDERING = 0x00040000,
		ACCELERATION_STRUCTURE_BUILD = 0x02000000,
		RAY_TRACING_SHADER = 0x00200000,
		TASK_SHADER_NV = 0x00080000,
		MESH_SHADER_NV = 0x00100000,
		FRAGMENT_DENSITY_PROCESS = 0x00800000,
		FRAGMENT_SHADING_RATE_ATTACHMENT = 0x00400000,
		COMMAND_PREPROCESS_NV = 0x00020000,
		NONE = 0,
	};

	enum class shader_stage 
	{
		VERTEX = 0x00000001,
		TESSELLATION_CONTROL = 0x00000002,
		TESSELLATION_EVALUATION = 0x00000004,
		GEOMETRY = 0x00000008,
		PIXEL = 0x00000010,
		COMPUTE = 0x00000020,
		ALL_GRAPHICS = 0x0000001F,
		ALL = 0x7FFFFFFF,
		RAYGEN = 0x00000100,
		ANY_HIT = 0x00000200,
		CLOSEST_HIT = 0x00000400,
		MISS = 0x00000800,
		INTERSECTION = 0x00001000,
		CALLABLE = 0x00002000,
		TASK_NV = 0x00000040,
		MESH_NV = 0x00000080,
		SUBPASS_SHADING_HUAWEI = 0x00004000
	};

	enum class access_flags 
	{
		INDIRECT_COMMAND_READ = 0x00000001,
		INDEX_READ = 0x00000002,
		VERTEX_ATTRIBUTE_READ = 0x00000004,
		UNIFORM_READ = 0x00000008,
		INPUT_ATTACHMENT_READ = 0x00000010,
		SHADER_READ = 0x00000020,
		SHADER_WRITE = 0x00000040,
		COLOR_ATTACHMENT_READ = 0x00000080,
		COLOR_ATTACHMENT_WRITE = 0x00000100,
		DEPTH_STENCIL_ATTACHMENT_READ = 0x00000200,
		DEPTH_STENCIL_ATTACHMENT_WRITE = 0x00000400,
		TRANSFER_READ = 0x00000800,
		TRANSFER_WRITE = 0x00001000,
		HOST_READ = 0x00002000,
		HOST_WRITE = 0x00004000,
		MEMORY_READ = 0x00008000,
		MEMORY_WRITE = 0x00010000,
		TRANSFORM_FEEDBACK_WRITE = 0x02000000,
		TRANSFORM_FEEDBACK_COUNTER_READ = 0x04000000,
		TRANSFORM_FEEDBACK_COUNTER_WRITE = 0x08000000,
		CONDITIONAL_RENDERING_READ = 0x00100000,
		COLOR_ATTACHMENT_READ_NONCOHERENT = 0x00080000,
		ACCELERATION_STRUCTURE_READ = 0x00200000,
		ACCELERATION_STRUCTURE_WRITE = 0x00400000,
		FRAGMENT_DENSITY_MAP_READ = 0x01000000,
		FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 0x00800000,
		COMMAND_PREPROCESS_READ_NV = 0x00020000,
		COMMAND_PREPROCESS_WRITE_NV = 0x00040000,
		NONE = 0
	};

	enum class shader_source
	{
		SAMPLER = 0,
		COMBINED_IMAGE_SAMPLER = 1,
		SAMPLED_IMAGE = 2,
		STORAGE_IMAGE = 3,
		UNIFORM_TEXEL_BUFFER = 4,
		STORAGE_TEXEL_BUFFER = 5,
		UNIFORM_BUFFER = 6,
		STORAGE_BUFFER = 7,
		UNIFORM_BUFFER_DYNAMIC = 8,
		STORAGE_BUFFER_DYNAMIC = 9,
		INPUT_ATTACHMENT = 10,
		INLINE_UNIFORM_BLOCK = 1000138000,
		ACCELERATION_STRUCTURE = 1000150000,
		ACCELERATION_STRUCTURE_NV = 1000165000,
		MUTABLE_VALVE = 1000351000
	};

}}


namespace rav { namespace hal {
	typedef double color_t[4];
	struct device_t;
	struct queue_t;
	struct device_connection_t;
	struct shader_t;

	struct driver_connection_builder_t;
	struct device_connection_builder_t;
	struct render_command_builder_t;
	struct render_pass_builder_t;
	struct shader_builder_t;
	struct frame_builder_t;
	struct command_pool_builder_t;
	struct command_buffer_builder_t;
	struct fence_builder_t;
	struct semaphore_builder_t;
	struct sampler_builder_t;
	struct memory_builder_t;
	struct resource_builder_t;
	struct resource_view_builder_t;
	struct pipeline_inputs_layout_builder_t;
	struct swapchain_builder_t;
	struct namespace_builder_t;
}}

namespace rav { namespace hal {
/*
	enum class image_attachment 
	{
		VkAttachmentDescriptionFlags    flags;
		VkFormat                        format;
		VkSampleCountFlagBits           samples;
		VkAttachmentLoadOp              loadOp;
		VkAttachmentStoreOp             storeOp;
		VkAttachmentLoadOp              stencilLoadOp;
		VkAttachmentStoreOp             stencilStoreOp;
		VkImageLayout                   initialLayout;
		VkImageLayout                   finalLayout;
	};

}}


namespace rav { namespace hal {
#define HAL_ENUM(_T) \
	FORCE_INLINE _T operator | (_T a, _T b) {return _T(int(a)|int(b));} \
	FORCE_INLINE int operator & (_T a, _T b) {return int(int(a)&int(b));}

	HAL_ENUM(queue_type_t)
	HAL_ENUM(process_type_t)
	HAL_ENUM(frame_attachment)
	HAL_ENUM(format_feature)
	HAL_ENUM(memory_property)
	HAL_ENUM(image_tiling)
	HAL_ENUM(pipeline_stage)
	HAL_ENUM(shader_stage)
	HAL_ENUM(access_flags)
}}


namespace sgl
{
	struct depth_stencil_t 
	{
		bool	depth_enable;
		bool	depth_write_enable;
		bool	depth_clip_enable;
		bool	stencil_enable;
		bool	two_sided_stencil_enable;
		
		int32_t		stencil_ref;
		GLenum		depth_test_function;
		GLenum		stencil_front_test_function;
		int32_t		stencil_front_write_mask;
		int32_t		stencil_front_read_mask;
		GLenum		stencil_front_pass_action;
		GLenum		stencil_front_fail_action;
		GLenum		stencil_front_depth_fail_action;
		GLenum		stencil_back_test_function;
		uint16_t	stencil_back_write_mask;
		uint16_t	stencil_back_read_mask;
		GLenum		stencil_back_pass_action;
		GLenum		stencil_back_fail_action;
		GLenum		stencil_back_depth_fail_action;
		uint16_t	_stencil_action_hash;
		uint16_t	_stencil_back_action_hash;

		DLL_FNC(void)		configure_hashes();
		
		struct binder_t
		{
			int8_t		GL_DEPTH_TEST_INT = -1;
			int8_t		GL_STENCIL_TEST_INT = -1;
			int8_t		GL_STENCIL_TEST_TWO_SIDE_EXT_INT = -1;
			int8_t		glDepthMask_int = -1;
			GLenum		glDepthFunc_int = 0xFFFF;
			GLenum		glStencilFuncSeparate_FRONT_function = 0xFFFF;
			int32_t		glStencilFuncSeparate_FRONT_ref = -1;
			int32_t		glStencilFuncSeparate_FRONT_mask = -1;
			int32_t		glStencilMaskSeparate_FRONT = -1;
			uint32_t	glStencilOpSeparate_FRONT = 0xFFFFFFFF;
			GLenum		glStencilFuncSeparate_BACK_function = 0xFFFF;
			int32_t		glStencilFuncSeparate_BACK_ref = -1;
			int32_t		glStencilFuncSeparate_BACK_mask = -1;
			int32_t		glStencilMaskSeparate_BACK = -1;
			uint32_t	glStencilOpSeparate_BACK = 0xFFFFFFFF;
			
			
			DLL_FNC(void)	bind(const depth_stencil_t&);

			inline void invalidate()
			{
				GL_DEPTH_TEST_INT = -1;
				GL_STENCIL_TEST_INT = -1;
				GL_STENCIL_TEST_TWO_SIDE_EXT_INT = -1;
				glDepthMask_int = -1;
				glDepthFunc_int = 0xFFFF;
				glStencilFuncSeparate_FRONT_function = 0xFFFF;
				glStencilFuncSeparate_FRONT_ref = -1;
				glStencilFuncSeparate_FRONT_mask = -1;
				glStencilMaskSeparate_FRONT = -1;
				glStencilOpSeparate_FRONT = 0xFFFFFFFF;
				glStencilFuncSeparate_BACK_function = 0xFFFF;
				glStencilFuncSeparate_BACK_ref = -1;
				glStencilFuncSeparate_BACK_mask = -1;
				glStencilMaskSeparate_BACK = -1;
				glStencilOpSeparate_BACK = 0xFFFFFFFF;
			}
		};
	};
	
}

namespace sgl
{
	struct raster_t
	{
		bool	front_counter_clockwise;
		bool	conservative_raster_enable;
		
		GLenum		shade_mode;
		GLenum		fill_mode;
		GLenum		cull_mode;
		GLenum		enable_point_size;
		
		struct binder_t
		{
			GLenum	glShadeMode_int = -1;
			GLenum	glCullFace_int = -1;
			GLenum	glPolygonMode_int = -1;
			GLenum	glEnablePointSize_int = -1;

			DLL_FNC(void)	bind(const raster_t&);
			inline void invalidate()
			{
				glCullFace_int = -1;
				glPolygonMode_int = -1;
				glEnablePointSize_int = -1;
			}
		};
	};
	
}




*/