#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>
namespace life {

    template<typename T>
	class Timer {
	private:
		using Clock = std::chrono::high_resolution_clock;
		using ms = std::chrono::duration<T, std::milli>;
		using sec = std::chrono::duration<T, std::chrono::seconds>;
		
		Clock::time_point epoch;

	public:
        explicit Timer(bool initialize_on_construction = true) : epoch{} { if(initialize_on_construction) start(); }
		void start() {
			epoch = Clock::now(); 
		}
		[[nodiscard]] Clock::duration time_elapsed() const {
			return Clock::now() - epoch; 
		}
		[[nodiscard]] T milliseconds_elapsed() const {
			ms my_ms = std::chrono::duration_cast<ms>(time_elapsed());
			return my_ms.count();
		}
		[[nodiscard]] T seconds_elapsed() const {
			ms my_s = std::chrono::duration_cast<ms>(time_elapsed());
			return my_s.count() / 1000;
		}
	};

}
#endif //TIMER_HPP
