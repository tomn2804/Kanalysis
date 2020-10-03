#pragma once

#include "include/config.h"

namespace kanalysis::utils::console
{
	class Progress
	{
	public:
		Progress() = default;
		Progress(UInt i, UInt n);
		Progress(UInt n);
		~Progress();

		void start();
		void increment();
		void stop();
	private:
		UInt m_i;
		UInt m_n;
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
	}

	KANALYSIS_INLINE void Progress::start()
	{
		std::cout << "Progress: 0%\r";
	}

	KANALYSIS_INLINE void Progress::increment()
	{
		++m_i;
		int percent = double(m_i) / double(m_n) * 100;
		std::cout << "Progress: " << percent << "%\r";
		assert(m_i <= m_n);
	}

	KANALYSIS_INLINE void Progress::stop()
	{
		std::cout << "Progress: 100%\r" << std::endl;
	}
} // namespace kanalysis::utils::console
