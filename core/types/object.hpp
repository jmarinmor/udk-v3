#pragma once

namespace rav
{
	struct string_t
	{
		private:
		void* m;
		
		public:
		CTORS(string_t)
		string_t(const char*);

		public:
		auto c_str() const -> const char*;
	};

	struct object_t
	{
		virtual inline ~object_t() {}
		virtual void retain() const = 0;
		virtual void release() const = 0;
		virtual auto handle() const -> handle_t;
		virtual auto handle(handle_t::type_t) const -> void* const;
		virtual auto handle_value() const -> void* const;
		inline	auto handle_int(handle_t::type_t t) const -> uint64_t const {return reinterpret_cast<uint64_t>(handle(t));}
		virtual auto ref_counter() const -> int = 0;
	    virtual	auto to_string() const -> string_t;
	};
}

