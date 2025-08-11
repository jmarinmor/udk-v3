// mutex.h
#pragma once
#include <atomic>
#include <thread>
#include <cstdint>

//
// --- Hints por arquitectura para espera activa ---
#if defined(__x86_64__) || defined(_M_X64)
// x86-64: PAUSE garantizado
  #include <immintrin.h>
  inline void cpu_relax() noexcept { _mm_pause(); }

#elif defined(__i386) || defined(_M_IX86)
// x86-32: usa _mm_pause si hay SSE2; si no, cae a yield
  #if defined(__SSE2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    #include <immintrin.h>
    inline void cpu_relax() noexcept { _mm_pause(); }
  #else
    inline void cpu_relax() noexcept { std::this_thread::yield(); }
  #endif

#elif defined(__aarch64__) || defined(__arm__)
// ARM / AArch64: YIELD
  inline void cpu_relax() noexcept { asm volatile("yield" ::: "memory"); }

#else
// Fallback genérico
  inline void cpu_relax() noexcept { std::this_thread::yield(); }
#endif

//
// --- Tamaño de línea de caché ---
#if defined(__cpp_lib_hardware_interference_size)
  #include <new>
  constexpr std::size_t CACHELINE = std::hardware_destructive_interference_size;
#else
  constexpr std::size_t CACHELINE = 64; // valor típico
#endif

//
// --- Spinlock TTAS base (baja contención) ---
struct spinlock_t 
{
    std::atomic<bool> locked{false};
    static constexpr int kMaxSpins = 1 << 10; // tope de backoff

    // TTAS con backoff exponencial y cpu_relax
    inline void lock() noexcept 
	{
        int spins = 1;
        for (;;) 
		{
            // Fase de lectura (no invalida caché)
            while (locked.load(std::memory_order_relaxed)) 
			{
                for (int i = 0; i < spins; ++i) cpu_relax();
                if (spins < kMaxSpins) spins <<= 1;
            }
            // Intento de adquirir (única escritura)
            bool expected = false;
            if (locked.compare_exchange_weak(expected, true,
                                             std::memory_order_acquire,
                                             std::memory_order_relaxed)) {
                return;
            }
            // otro hilo se adelantó; reintenta
        }
    }

    // Variante "busy" pero con relax + yield periódico por cortesía
    inline void lock_strong() noexcept {
        int spins = 0;
        while (!try_lock()) {
            cpu_relax();
            if (++spins >= 1024) { std::this_thread::yield(); spins = 0; }
        }
    }

    [[nodiscard]] inline bool try_lock() noexcept {
        bool expected = false;
        return locked.compare_exchange_strong(expected, true,
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed);
    }

    inline void unlock() noexcept {
        locked.store(false, std::memory_order_release);
    }
};

//
// --- Spinlock alineado para evitar false sharing en arrays/vectores ---
// Nota: sizeof(T) será múltiplo de alignof(T), por lo que en contenedores
//       cada elemento quedará en su propia línea de caché.
struct alignas(CACHELINE) spinlock_aligned_t : public spinlock_t {
};

// Comprobaciones de alineación/tamaño (recomendadas si vas a tener arrays)
static_assert(alignof(spinlock_aligned_t) >= CACHELINE, "spinlock_aligned_t debe alinearse a línea de caché");
static_assert(sizeof(spinlock_aligned_t) % alignof(spinlock_aligned_t) == 0, "sizeof(spinlock_aligned_t) debe ser múltiplo de su alineación");


template<class Spinlock>
struct lock_guard_spin {
    explicit lock_guard_spin(Spinlock& s) : s_(s) { s_.lock(); }
    ~lock_guard_spin() { s_.unlock(); }
    lock_guard_spin(const lock_guard_spin&) = delete;
    lock_guard_spin& operator=(const lock_guard_spin&) = delete;
private:
    Spinlock& s_;
};




void enter_to_critical_section();
void exit_from_critical_section();

void memory_lock(const void*);
void memory_unlock(const void*);
void memory_lock();
void memory_unlock();
