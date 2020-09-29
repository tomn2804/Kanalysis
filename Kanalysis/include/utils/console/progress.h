#pragma once

#include "include/config.h"

namespace kanalysis::utils::console
{
	void show_console_cursor(bool flag);

	class Progress
	{
	public:
		Progress() = default;
		Progress(UInt i, UInt n);
		Progress(UInt n);
		~Progress();

		void start();
		void update(UInt i);
		void stop();
	private:
		std::atomic<UInt> m_i;
		UInt m_n;
		std::mutex m_mutex;
	};
} // namespace kanalysis::utils::console

namespace kanalysis::utils::console
{
	KANALYSIS_INLINE Progress::Progress(UInt i, UInt n)
		: m_i(i)
		, m_n(n)
	{}

	KANALYSIS_INLINE Progress::Progress(UInt n)
		: Progress(0, n)
	{}

	KANALYSIS_INLINE Progress::~Progress()
	{
		std::cout << std::flush;
		show_console_cursor(true);
	}

	KANALYSIS_INLINE void Progress::start()
	{
		show_console_cursor(false);
		std::cout << "Progress: 0%\r";
	}

	KANALYSIS_INLINE void Progress::update(UInt i)
	{
		m_i += i;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			int percent = double(m_i) / double(m_n) * 100;
			std::cout << "Progress: " << percent << "%\r";
			std::cout << std::flush;
		}
	}

	KANALYSIS_INLINE void Progress::stop()
	{
		std::cout << "Progress: 100%\r" << std::endl;
		console::show_console_cursor(true);
	}
} // namespace kanalysis::utils::console