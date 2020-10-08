#pragma once

#include "include/config.h"

namespace kanalysis::utils
{
	///
	/// \brief A class for displaying the progress percentage status to the console.
	/// \details Example console output would be "Progress: 50%"
	///
	class Progress
	{
	public:
		Progress() = default;
		Progress(UInt n);
		~Progress();

		void start();
		void increment(UInt i);
		void increment();
		void complete();

		UInt n() const;
		UInt i() const;
	private:
		UInt m_n = 0;
		UInt m_i = 0;
	};
} // namespace kanalysis::utils

namespace kanalysis::utils
{
	///
	/// \brief A constructor.
	///
	/// \param n An unsigned interger representing the total amount of tasks or size of something.
	///
	KANALYSIS_INLINE Progress::Progress(UInt n)
		: m_n(n)
	{}

	///
	/// \brief A destructor.
	///
	KANALYSIS_INLINE Progress::~Progress()
	{
		std::cout << std::flush;
	}

	///
	/// \brief Display the progress percentage at 0%.
	///
	KANALYSIS_INLINE void Progress::start()
	{
		std::cout << "Progress: 0%\r";
	}

	///
	/// \brief Increment the percentage by \a i.
	///
	/// \details The updated percentage will be i divide by n.
	///
	/// \param i An unsigned interger. Default is 1.
	///
	KANALYSIS_INLINE void Progress::increment(UInt i)
	{
		m_i += i;
		int percent = double(m_i) / double(m_n) * 100;
		assert(percent <= 100);
		std::cout << "Progress: " << percent << "%\r";
	}

	///
	/// \overload void Progress::increment(UInt i)
	///
	KANALYSIS_INLINE void Progress::increment()
	{
		increment(1);
	}

	///
	/// \brief Display the progress percentage at 100%.
	///
	KANALYSIS_INLINE void Progress::complete()
	{
		std::cout << "Progress: 100%\r" << std::endl;
	}

	///
	/// \brief Returns \a n , which is the total number of tasks.
	/// \a n is used as the denomintor when calculating percentage.
	///
	KANALYSIS_INLINE UInt Progress::n() const
	{
		return m_n;
	}

	///
	/// \brief Returns \a r , which is the current number of completed tasks.
	/// \a i is used as the numerator when calculating percentage.
	///
	KANALYSIS_INLINE UInt Progress::i() const
	{
		return m_i;
	}
} // namespace kanalysis::utils
