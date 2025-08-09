#ifndef __CORE_PTR_HEADER__
#define __CORE_PTR_HEADER__

#include <atomic/critical_section.h>

namespace rav
{
	struct ptr_lock_owner_t
	{
		static	volatile int	mMutex;
	};


	template<typename T>
	inline	void release_atomic(volatile T*& AnObject)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T* p = (T*)AnObject;
		AnObject = nullptr;
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	
	template<typename T>
	inline	void release_atomic(T*& AnObject)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T* p = (T*)AnObject;
		AnObject = nullptr;
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	
	template<typename T1, typename T2>
	inline	void retain_atomic(volatile T1*& ADestination, volatile T2*& ASource)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T1* p = (T1*)ADestination;
		if (ASource)
			((T1*)(ADestination = ASource))->retain();
		else
			ADestination = nullptr;
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	
	template<typename T1, typename T2>
	inline	void retain_atomic(T1*& ADestination, volatile T2*& ASource)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T1* p = ADestination;
		if ((ADestination = (T2*)ASource))
			ADestination->retain();
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	
	template<typename T1, typename T2>
	inline	void retain_atomic(volatile T1*& ADestination, T2*& ASource)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T1* p = (T1*)ADestination;
		if (ASource)
			((T1*)(ADestination = (T1*)ASource))->retain();
		else
			ADestination = nullptr;
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	

	template<typename T1, typename T2>
	inline	void retain_atomic(T1*& ADestination, T2*& ASource)
	{
		spinlock_lock(&ptr_lock_owner_t::mMutex);
		T1* p = ADestination;
		if ((ADestination = ASource))
			ADestination->retain();
		spinlock_unlock(&ptr_lock_owner_t::mMutex);
		if (p)
			p->release();
	}
	
	template<typename T>
	inline	void retain_atomic(volatile T*& AnObject)
	{
		if (AnObject)
			AnObject->retain();
	}
	
	template<typename T>
	inline	void release_non_atomic(volatile T*& AnObject)
	{
		T* p = (T*)AnObject;
		AnObject = nullptr;
		if (p)
			p->release();
	}
	
	template<typename T1, typename T2>
	inline	void retain_non_atomic(volatile T1*& ADestination, volatile T2*& ASource)
	{
		T1* p = ADestination;
		if (ASource)
			((T1*)(ADestination = ASource))->retain();
		else
			ADestination = nullptr;
		if (p)
			p->release();
	}
	
	template<typename T>
	inline	void retain_non_atomic(volatile T*& AnObject)
	{
		if (AnObject)
			AnObject->retain();
	}
	
	template <bool Atomic1, bool Atomic2>
	constexpr static	bool	should_use_atomic_ptr_operation()
	{
		return Atomic1 || Atomic2;
	}
	
	template <typename T, bool Atomic>
	struct ptr_storage
	{
	};
	
	template <typename T>
	struct ptr_storage<T, true>
	{
		volatile T*	m;
		inline	void	retain() const
		{
			if (m)
				const_cast<T*>((const T*)m)->retain();
		}
		inline	void	release() const
		{
			if (m)
				const_cast<T*>((const T*)m)->release();
		}
		inline	void	set(const T* p)
		{
			m = const_cast<T*>((const T*)p);
		}
		template <typename T2>
		inline	void	set(const T2* p)
		{
			m = const_cast<T*>((const T*)p);
		}
		inline	T*	get()
		{
			return (T*)m;
		}
		inline	const T*	get() const
		{
			return (const T*)m;
		}
		template<typename T2>
		inline	void init_atomic(volatile T2*& APointer)
		{
			spinlock_lock(&ptr_lock_owner_t::mMutex);
			if ((m = static_cast<volatile T*>(APointer)))
				get()->retain();
			spinlock_unlock(&ptr_lock_owner_t::mMutex);
		}
		template<typename T2>
		inline	void init_atomic(T2*& APointer)
		{
			spinlock_lock(&ptr_lock_owner_t::mMutex);
			if ((m = APointer))
				get()->retain();
			spinlock_unlock(&ptr_lock_owner_t::mMutex);
		}
		template<typename T2>
		inline	void init_non_atomic(volatile T2*& APointer)
		{
			if ((m = static_cast<volatile T*>(APointer)))
				get()->retain();
		}
		template<typename T2>
		inline	void init_non_atomic(T2*& APointer)
		{
			if ((m = APointer))
				get()->retain();
		}
		
	};
	
	template <typename T>
	struct ptr_storage<T, false>
	{
		T*	m;
		
		inline	void	retain() const
		{
			if (m)
				const_cast<T*>(m)->retain();
		}
		inline	void	release() const
		{
			if (m)
				const_cast<T*>(m)->release();
		}
		inline	void	set(const T* p)
		{
			m = const_cast<T*>(p);
		}
		template <typename T2>
		inline	void	set(const T2* p)
		{
			m = const_cast<T2*>(p);
		}
		inline	T*	get()
		{
			return m;
		}
		inline	const T*	get() const
		{
			return m;
		}
		template<typename T2>
		inline	void init_atomic(volatile T2*& APointer)
		{
			spinlock_lock(&ptr_lock_owner_t::mMutex);
			if ((m = (T*)APointer))
				m->retain();
			spinlock_unlock(&ptr_lock_owner_t::mMutex);
		}
		template<typename T2>
		inline	void init_atomic(T2*& APointer)
		{
			spinlock_lock(&ptr_lock_owner_t::mMutex);
			if ((m = APointer))
				m->retain();
			spinlock_unlock(&ptr_lock_owner_t::mMutex);
		}
		template<typename T2>
		inline	void init_non_atomic(volatile T2*& APointer)
		{
			if ((m = (T*)APointer))
				m->retain();
		}
		template<typename T2>
		inline	void init_non_atomic(T2*& APointer)
		{
			if ((m = (T*)APointer))
				m->retain();
		}
	};

	template <typename T, bool Atomic = true>
	struct ptr
	{
	//private:
		ptr_storage<T,Atomic>	mPointer;
	public:
		FORCE_INLINE	T*	get()
		{
			return mPointer.get();
		}
		FORCE_INLINE	const T*	get() const
		{
			return mPointer.get();
		}
		inline	ptr()
		{
			mPointer.m = nullptr;
		}
		inline	ptr(const T* p)
		{
			mPointer.set(p);
			mPointer.retain();
		}
		template <typename T2>
		inline	ptr(const T2* p)
		{
			mPointer.set(p);
			mPointer.retain();
		}
		ptr(const ptr& p)
		{
			if (Atomic)
				mPointer.init_atomic(const_cast<ptr&>(p).mPointer.m);
			else
				mPointer.init_non_atomic(const_cast<ptr&>(p).mPointer.m);
		}
		ptr(ptr&& p)
		{
			mPointer.m = p.mPointer.m;
			p.mPointer.m = nullptr;
		}
		/*
		ptr(ptr&& p)
		{
			mPointer.set(const_cast<ptr&>(p).mPointer.get());
			p.mPointer.m = nullptr;
		}*/
		template <typename T2, bool Atomic2>
		ptr(const ptr<T2, Atomic2>& p)
		{
			if (Atomic2)
				mPointer.init_atomic(const_cast<ptr<T2, Atomic2>&>(p).mPointer.m);
			else
				mPointer.init_non_atomic(const_cast<ptr<T2, Atomic2>&>(p).mPointer.m);
		}
		template <typename T2, bool Atomic2>
		ptr(ptr<T2, Atomic2>&& p)
		{
			mPointer.set(p.get());
			p.mPointer.set(nullptr);
		}
		inline ~ptr()
		{
			if (Atomic)
				release_atomic(mPointer.m);
			else
				mPointer.release();
		}
		/*
		inline operator T* ()
		{
			return mPointer.get();
		}
		inline operator const T* () const
		{
			return mPointer.get();
		}*/

		FORCE_INLINE explicit operator bool() const
		{
			return mPointer.m != nullptr;
		}
		ptr& operator = (const T* p)
		{
			if (p != mPointer.m)
			{
				if (Atomic)
				{
					T* pp = const_cast<T*>(p);
					retain_atomic(mPointer.m, pp);
				}
				else
				{
					mPointer.release();
					if ((mPointer.m = const_cast<T*>(p)))
						((T*)mPointer.m)->retain();
				}
			}
			return *this;
		}
		template <typename T2>
		ptr& operator = (const T2* p)
		{
			if (Atomic)
				retain_atomic(mPointer.m, p);
			else
			{
				mPointer.release();
				if ((mPointer.m = const_cast<T2*>(p)))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		inline ptr& operator = (const ptr& p)
		{
			if (Atomic)
				retain_atomic(mPointer.m, const_cast<ptr&>(p).mPointer.m);
			else
			{
				mPointer.release();
				if ((mPointer.m = p.mPointer.m))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		template <typename T2, bool Atomic2>
		inline ptr& operator = (const ptr<T2, Atomic2>& p)
		{
			if (Atomic || Atomic2)
				retain_atomic(mPointer.m, const_cast<ptr<T2, Atomic2>&>(p).mPointer.m);
			else
			{
				mPointer.release();
				if ((mPointer.m = static_cast<T*>((T2*)const_cast<ptr<T2, Atomic2>&>(p).mPointer.m)))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		inline ptr& operator = (ptr&& p)
		{
			if (Atomic)
			{
				spinlock_lock(&ptr_lock_owner_t::mMutex);
				auto pp = get();
				mPointer.set(p.get());
				spinlock_unlock(&ptr_lock_owner_t::mMutex);
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			else
			{
				auto pp = get();
				mPointer.set(p.get());
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			return *this;
		}
		template <typename T2, bool Atomic2>
		inline ptr& operator = (ptr<T2, Atomic2>&& p)
		{
			if (Atomic || Atomic2)
			{
				spinlock_lock(&ptr_lock_owner_t::mMutex);
				auto pp = get();
				mPointer.set(p.get());
				spinlock_unlock(&ptr_lock_owner_t::mMutex);
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			else
			{
				auto pp = get();
				mPointer.set(p.get());
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			return *this;
		}
		template <typename T2>
		FORCE_INLINE bool operator == (const T2* p) const
		{
			return mPointer.m == p;
		}
		template <typename T2, bool Atomic2>
		FORCE_INLINE bool operator == (const ptr<T2, Atomic2>& p) const
		{
			return mPointer.m == p.mPointer;
		}
		FORCE_INLINE bool operator == (const T* p) const
		{
			return mPointer.m == p;
		}
		FORCE_INLINE bool operator == (const ptr& p) const
		{
			return mPointer.m == p.mPointer.m;
		}

		template <typename T2>
		FORCE_INLINE bool operator != (const T2* p) const
		{
			return mPointer.m != p;
		}
		template <typename T2, bool Atomic2>
		FORCE_INLINE bool operator != (const ptr<T2, Atomic2>& p) const
		{
			return mPointer.m != p.mPointer;
		}
		FORCE_INLINE bool operator != (const T* p) const
		{
			return mPointer.m != p;
		}
		FORCE_INLINE bool operator != (const ptr& p) const
		{
			return mPointer.m != p.mPointer.m;
		}

		FORCE_INLINE T*	operator ->()
		{
			return get();
		}
		FORCE_INLINE const T*	operator ->()const
		{
			return get();
		}

	};

	template <typename T, bool Atomic = true>
	struct const_ptr
	{
	//private:
		ptr_storage<T,Atomic>	mPointer;
	public:
		FORCE_INLINE	const T*	get() const
		{
			return mPointer.get();
		}
		inline	const_ptr()
		{
			mPointer.m = nullptr;
		}
		inline	const_ptr(const T* p)
		{
			mPointer.set(p);
			mPointer.retain();
		}
		template <typename T2>
		inline	const_ptr(const T2* p)
		{
			mPointer.set(p);
			mPointer.retain();
		}
		const_ptr(const const_ptr& p)
		{
			if (Atomic)
				mPointer.init_atomic(const_cast<const_ptr&>(p).mPointer.m);
			else
				mPointer.init_non_atomic(const_cast<const_ptr&>(p).mPointer.m);
		}
		const_ptr(const_ptr&& p)
		{
			mPointer.m = p.mPointer.m;
			p.mPointer.m = nullptr;
		}
		/*
		ptr(ptr&& p)
		{
			mPointer.set(const_cast<ptr&>(p).mPointer.get());
			p.mPointer.m = nullptr;
		}*/
		template <typename T2, bool Atomic2>
		const_ptr(const const_ptr<T2, Atomic2>& p)
		{
			if (Atomic2)
				mPointer.init_atomic(const_cast<ptr<T2, Atomic2>&>(p).mPointer.m);
			else
				mPointer.init_non_atomic(const_cast<ptr<T2, Atomic2>&>(p).mPointer.m);
		}
		template <typename T2, bool Atomic2>
		const_ptr(const_ptr<T2, Atomic2>&& p)
		{
			mPointer.set(p.get());
			p.mPointer.set(nullptr);
		}
		inline ~const_ptr()
		{
			if (Atomic)
				release_atomic(mPointer.m);
			else
				mPointer.release();
		}
		/*
		inline operator T* ()
		{
			return mPointer.get();
		}
		inline operator const T* () const
		{
			return mPointer.get();
		}*/

		FORCE_INLINE explicit operator bool() const
		{
			return mPointer.m != nullptr;
		}
		const_ptr& operator = (const T* p)
		{
			if (p != mPointer.m)
			{
				if (Atomic)
				{
					T* pp = const_cast<T*>(p);
					retain_atomic(mPointer.m, pp);
				}
				else
				{
					mPointer.release();
					if ((mPointer.m = const_cast<T*>(p)))
						((T*)mPointer.m)->retain();
				}
			}
			return *this;
		}
		template <typename T2>
		const_ptr& operator = (const T2* p)
		{
			if (Atomic)
				retain_atomic(mPointer.m, p);
			else
			{
				mPointer.release();
				if ((mPointer.m = const_cast<T2*>(p)))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		inline const_ptr& operator = (const const_ptr& p)
		{
			if (Atomic)
				retain_atomic(mPointer.m, const_cast<const_ptr&>(p).mPointer.m);
			else
			{
				mPointer.release();
				if ((mPointer.m = p.mPointer.m))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		template <typename T2, bool Atomic2>
		inline const_ptr& operator = (const const_ptr<T2, Atomic2>& p)
		{
			if (Atomic || Atomic2)
				retain_atomic(mPointer.m, const_cast<const_ptr<T2, Atomic2>&>(p).mPointer.m);
			else
			{
				mPointer.release();
				if ((mPointer.m = static_cast<T*>((T2*)const_cast<ptr<T2, Atomic2>&>(p).mPointer.m)))
					((T*)mPointer.m)->retain();
			}
			return *this;
		}
		inline const_ptr& operator = (const_ptr&& p)
		{
			if (Atomic)
			{
				spinlock_lock(&ptr_lock_owner_t::mMutex);
				auto pp = get();
				mPointer.set(p.get());
				spinlock_unlock(&ptr_lock_owner_t::mMutex);
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			else
			{
				auto pp = get();
				mPointer.set(p.get());
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			return *this;
		}
		template <typename T2, bool Atomic2>
		inline const_ptr& operator = (const_ptr<T2, Atomic2>&& p)
		{
			if (Atomic || Atomic2)
			{
				spinlock_lock(&ptr_lock_owner_t::mMutex);
				auto pp = get();
				mPointer.set(p.get());
				spinlock_unlock(&ptr_lock_owner_t::mMutex);
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			else
			{
				auto pp = get();
				mPointer.set(p.get());
				p.mPointer.set(nullptr);
				if (pp)
					pp->release();
			}
			return *this;
		}
		template <typename T2>
		FORCE_INLINE bool operator == (const T2* p) const
		{
			return mPointer.m == p;
		}
		template <typename T2, bool Atomic2>
		FORCE_INLINE bool operator == (const const_ptr<T2, Atomic2>& p) const
		{
			return mPointer.m == p.mPointer;
		}
		FORCE_INLINE bool operator == (const T* p) const
		{
			return mPointer.m == p;
		}
		FORCE_INLINE bool operator == (const const_ptr& p) const
		{
			return mPointer.m == p.mPointer.m;
		}

		template <typename T2>
		FORCE_INLINE bool operator != (const T2* p) const
		{
			return mPointer.m != p;
		}
		template <typename T2, bool Atomic2>
		FORCE_INLINE bool operator != (const const_ptr<T2, Atomic2>& p) const
		{
			return mPointer.m != p.mPointer;
		}
		FORCE_INLINE bool operator != (const T* p) const
		{
			return mPointer.m != p;
		}
		FORCE_INLINE bool operator != (const const_ptr& p) const
		{
			return mPointer.m != p.mPointer.m;
		}

		FORCE_INLINE const T*	operator ->()const
		{
			return get();
		}

	};

	template<typename T>
	struct unsafe_ptr
	{
		private:
		T* ptr;

		public:
		FORCE_INLINE	unsafe_ptr() {ptr = nullptr;}
		FORCE_INLINE	unsafe_ptr(const rav::ptr<T>& p) {ptr = const_cast<T*>(p.get());}
		template <typename T2>
		FORCE_INLINE	unsafe_ptr(const rav::ptr<T2>& p) {ptr = const_cast<T*>(static_cast<const T*>(p.get()));}
		FORCE_INLINE	unsafe_ptr(const unsafe_ptr<T>& p) {ptr = const_cast<T*>(p.get());}
		template <typename T2>
		FORCE_INLINE	unsafe_ptr(const unsafe_ptr<T2>& p) {ptr = const_cast<T*>(static_cast<const T*>(p.get()));}
		FORCE_INLINE	unsafe_ptr(const T* p) {ptr = const_cast<T*>(p);}
		FORCE_INLINE	~unsafe_ptr() {}

		public:
		FORCE_INLINE	bool operator == (const T* p) const {return ptr == p;}
		FORCE_INLINE	bool operator != (const T* p) const {return ptr != p;}
		FORCE_INLINE	void operator = (const T* p) {if (p == ptr) return; ptr = const_cast<T*>(static_cast<const T*>(p));}
		template <typename T2>
		FORCE_INLINE	void operator = (const T2* p) {if (p == ptr) return; ptr = const_cast<T*>(static_cast<const T*>(p));}
		FORCE_INLINE	void operator = (const rav::ptr<T>& p) {ptr = const_cast<T*>(p.get()); }
		template <typename T2>
		FORCE_INLINE	void operator = (const rav::ptr<T2>& p) {ptr = const_cast<T*>(static_cast<const T*>(p.get())); }
		FORCE_INLINE	void operator = (const unsafe_ptr<T>& p) {ptr = const_cast<T*>(p.get()); }
		template <typename T2>
		FORCE_INLINE	void operator = (const unsafe_ptr<T2>& p) {ptr = const_cast<T*>(static_cast<const T*>(p.get())); }
		//FORCE_INLINE	void operator = (ptr<T>&& p) {ptr = p.ptr; p.ptr = nullptr;}
		FORCE_INLINE	auto operator -> () -> T* {return ptr;}
		FORCE_INLINE	auto operator -> () const -> const T* {return ptr;}

		public:
		FORCE_INLINE	auto get() -> T* {return ptr;}
		FORCE_INLINE	auto get() const -> const T* {return ptr;}
	};
}

#endif


