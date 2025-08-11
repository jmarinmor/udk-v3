#pragma once

#include <atomic>

#include <stdint.h>
#include <pre.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Handles
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct handle_t
{
	enum type_t
	{
		UNKNOWN,
		HANDLE_WINDOWS,
		HANDLE_OPENGL,
		HANDLE_VULKAN,
		HANDLE_DIRECTX,

		VULKAN,
		VULKAN_VkSurfaceKHR,
		VULKAN_VkInstance,
		VULKAN_VkPhysicalDevice,
		VULKAN_VkDevice,
		VULKAN_VkRenderPass,
		VULKAN_VkSwapchainKHR,
		VULKAN_VkFramebuffer,
		VULKAN_VkImage,
		VULKAN_VkBuffer,
		VULKAN_VkImageView,
		VULKAN_VkPipeline,
		VULKAN_VkPipelineLayout,
		VULKAN_VkShaderModule,
		VULKAN_VkQueue,
		VULKAN_VkCommandPool,
		VULKAN_VkFence,
		VULKAN_VkSemaphore,
		VULKAN_VkCommandBuffer,
		VULKAN_VkFormat,
		VULKAN_VkColorSpaceKHR,
		VULKAN_VkSampler,
		VULKAN_VkDeviceMemory,
		VULKAN_VkDescriptorSetLayout,
		VULKAN_VkDescriptorSet,
		VULKAN_VkDescriptorPool,

		GLFW_GLFWwindow
	};

	inline handle_t() {}
	inline handle_t(void* h, type_t t)
	{
		handle = h;
		type = t;
	}
	inline bool is_null() const { return handle == nullptr; }

	void* handle;
	type_t type;
};






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Objects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct object_t
{
	virtual void retain() const = 0;
	virtual void release() const = 0;
	virtual int ref_count() const = 0;

protected:
	virtual ~object_t() = default;
};

struct handled_object_t : public object_t
{
	virtual inline ~handled_object_t() {}
	virtual auto handle() const -> handle_t;
	virtual auto handle(handle_t::type_t) const -> void* const;
	virtual auto handle_value() const -> void* const;
	inline auto handle_int(handle_t::type_t t) const -> uint64_t const { return reinterpret_cast<uint64_t>(handle(t)); }
};








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct string_t
{
private:
	void* m;

public:
	PRE_CTORS(string_t)
		string_t(const char*);

public:
	auto c_str() const -> const char*;
};










////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ref
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Tag para adoptar un puntero que ya tiene una ref (p.ej., tras `new`)
struct adopt_t
{
	explicit adopt_t() = default;
};
inline constexpr adopt_t adopt{};

//--------------------------------------------------------------
// ref<T, Atomic=false>: versión no atómica (rápida)
//--------------------------------------------------------------
template <typename T, bool Atomic = false>
class ref
{
	static_assert(std::is_base_of<object_t, T>::value, "T debe derivar de object_t");

public:
	using element_type = T;

	// Constructores
	ref() noexcept : p_(nullptr) {}
	ref(std::nullptr_t) noexcept : p_(nullptr) {}

	// Construcción reteniendo (si ptr!=nullptr)
	explicit ref(T* ptr) noexcept : p_(ptr)
	{
		if (p_)
			p_->retain();
	}

	// Construcción por adopción (no hace retain)
	ref(adopt_t, T* ptr) noexcept : p_(ptr) {}

	// Copia: comparte y retain
	ref(const ref& other) noexcept : p_(other.p_)
	{
		if (p_)
			p_->retain();
	}

	// Movimiento: transfiere
	ref(ref&& other) noexcept : p_(other.p_)
	{
		other.p_ = nullptr;
	}

	// Destructor
	~ref() { reset(); }

	// Asignación por copia
	ref& operator=(const ref& other) noexcept
	{
		if (this == &other)
			return *this;
		T* newp = other.p_;
		if (newp)
			newp->retain();
		T* old = p_;
		p_ = newp;
		if (old)
			old->release();
		return *this;
	}

	// Asignación por movimiento
	ref& operator=(ref&& other) noexcept
	{
		if (this == &other)
			return *this;
		T* old = p_;
		p_ = other.p_;
		other.p_ = nullptr;
		if (old)
			old->release();
		return *this;
	}

	// Asignar desde puntero crudo (retiene)
	ref& reset_to(T* ptr) noexcept
	{
		if (ptr)
			ptr->retain();
		T* old = p_;
		p_ = ptr;
		if (old)
			old->release();
		return *this;
	}

	// Adoptar puntero crudo (no retiene)
	ref& adopt_ptr(T* ptr) noexcept
	{
		T* old = p_;
		p_ = ptr;
		if (old)
			old->release();
		return *this;
	}

	// Liberar y dejar a nullptr
	void reset() noexcept
	{
		T* old = p_;
		p_ = nullptr;
		if (old)
			old->release();
	}

	// Suelta el puntero sin hacer release (¡cuidado!)
	T* detach() noexcept
	{
		T* r = p_;
		p_ = nullptr;
		return r;
	}

	// Observadores
	T* get() const noexcept { return p_; }
	T& operator*() const noexcept { return *p_; }
	T* operator->() const noexcept { return p_; }
	explicit operator bool() const noexcept { return p_ != nullptr; }

	// Comparaciones
	friend bool operator==(const ref& a, const ref& b) { return a.p_ == b.p_; }
	friend bool operator!=(const ref& a, const ref& b) { return a.p_ != b.p_; }
	friend bool operator==(const ref& a, std::nullptr_t) { return a.p_ == nullptr; }
	friend bool operator!=(const ref& a, std::nullptr_t) { return a.p_ != nullptr; }

	// Swap
	void swap(ref& other) noexcept { std::swap(p_, other.p_); }

private:
	T* p_;
};

//--------------------------------------------------------------
// ref<T, Atomic=true>: versión con puntero atómico
//
// Nota: esto hace seguras las *asignaciones/lecturas* concurrentes
// sobre la MISMA instancia de `ref`. Aún así, la seguridad total
// requiere que las implementaciones de retain/release en T sean
// thread-safe (p.ej., usando std::atomic<int>).
//--------------------------------------------------------------
template <typename T>
class ref<T, /*Atomic=*/true>
{
	static_assert(std::is_base_of<object_t, T>::value, "T debe derivar de object_t");

private:
	std::atomic<T*> p_;

public:
	using element_type = T;

	ref() noexcept : p_(nullptr) {}
	ref(std::nullptr_t) noexcept : p_(nullptr) {}

	explicit ref(T* ptr) noexcept : p_(nullptr)
	{
		if (ptr)
			ptr->retain();
		p_.store(ptr, std::memory_order_release);
	}

	ref(adopt_t, T* ptr) noexcept : p_(ptr) {}

	ref(const ref& other) noexcept : p_(nullptr)
	{
		T* observed = other.p_.load(std::memory_order_acquire);
		if (observed)
			observed->retain();
		p_.store(observed, std::memory_order_release);
	}

	ref(ref&& other) noexcept : p_(other.p_.exchange(nullptr, std::memory_order_acq_rel)) {}

	~ref() { reset(); }

	ref& operator=(const ref& other) noexcept
	{
		if (this == &other)
			return *this;
		T* newp = other.p_.load(std::memory_order_acquire);
		if (newp)
			newp->retain();
		T* old = p_.exchange(newp, std::memory_order_acq_rel);
		if (old)
			old->release();
		return *this;
	}

	ref& operator=(ref&& other) noexcept
	{
		if (this == &other)
			return *this;
		T* newp = other.p_.exchange(nullptr, std::memory_order_acq_rel);
		T* old = p_.exchange(newp, std::memory_order_acq_rel);
		if (old)
			old->release();
		return *this;
	}

	ref& reset_to(T* ptr) noexcept
	{
		if (ptr)
			ptr->retain();
		T* old = p_.exchange(ptr, std::memory_order_acq_rel);
		if (old)
			old->release();
		return *this;
	}

	ref& adopt_ptr(T* ptr) noexcept
	{
		T* old = p_.exchange(ptr, std::memory_order_acq_rel);
		if (old)
			old->release();
		return *this;
	}

	void reset() noexcept
	{
		T* old = p_.exchange(nullptr, std::memory_order_acq_rel);
		if (old)
			old->release();
	}

	T* detach() noexcept
	{
		return p_.exchange(nullptr, std::memory_order_acq_rel);
	}

	// Observadores
	T* get() const noexcept { return p_.load(std::memory_order_acquire); }
	T& operator*() const noexcept { return *get(); }
	T* operator->() const noexcept { return get(); }
	explicit operator bool() const noexcept { return get() != nullptr; }

	// Comparaciones
	friend bool operator==(const ref& a, const ref& b)
	{
		return a.get() == b.get();
	}
	friend bool operator!=(const ref& a, const ref& b)
	{
		return a.get() != b.get();
	}
	friend bool operator==(const ref& a, std::nullptr_t)
	{
		return a.get() == nullptr;
	}
	friend bool operator!=(const ref& a, std::nullptr_t)
	{
		return a.get() != nullptr;
	}

	void swap(ref& other) noexcept
	{
		// Intercambio lock-free
		T* a = p_.load(std::memory_order_acquire);
		T* b = other.p_.load(std::memory_order_acquire);
		while (!p_.compare_exchange_weak(a, b, std::memory_order_acq_rel))
		{
		}
		while (!other.p_.compare_exchange_weak(b, a, std::memory_order_acq_rel))
		{
		}
	}
};

//--------------------------------------------------------------
// Helpers
//--------------------------------------------------------------
template <typename T, bool A>
inline void swap(ref<T, A>& a, ref<T, A>& b) noexcept { a.swap(b); }

// Un alias práctico para punteros de solo lectura
template <typename T, bool A = false>
using const_ref = ref<const T, A>;