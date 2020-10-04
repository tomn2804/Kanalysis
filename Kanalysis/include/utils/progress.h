#pragma once

#include "include/config.h"

namespace kanalysis::utils
{
	class Progress
	{
	public:
		Progress() = default;
		Progress(UInt i, UInt n);
		Progress(UInt n);
		~Progress();

		void start();
		void increment(UInt i);
		void increment();
		void complete();
	private:
		UInt m_i;
		UInt m_n;
	};
} // namespace kanalysis::utils

namespace kanalysis::utils
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

	KANALYSIS_INLINE void Progress::increment(UInt i)
	{
		m_i += i;
		int percent = double(m_i) / double(m_n) * 100;
		assert(percent <= 100);
		std::cout << "Progress: " << percent << "%\r";
	}

	KANALYSIS_INLINE void Progress::increment()
	{
		increment(1);
	}

	KANALYSIS_INLINE void Progress::complete()
	{
		std::cout << "Progress: 100%\r" << std::endl;
	}
} // namespace kanalysis::utils
