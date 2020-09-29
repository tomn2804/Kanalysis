#pragma once

#include "include/config.h"

namespace kanalysis::utils::console
{
	template<typename ForwardIterator>
	void print(ForwardIterator first, ForwardIterator last);

	template<typename T>
	void prompt(const std::string& message, T& out);

	template<typename T, typename UnaryPredicate>
	void prompt(const std::string& message, T& out, UnaryPredicate pred);

	void show_console_cursor(bool flag);
} // namespace kanalysis::utils::console

namespace kanalysis::utils::console
{
	template<typename ForwardIterator>
	void print(ForwardIterator first, ForwardIterator last)
	{
		assert(std::distance(first, last) >= 0);

		std::cout << "[ " << *first;
		for (++first; first != last; ++first)
		{
			std::cout << ", " << *first;
		}
		std::cout << " ]\n";
	}

	template<typename T>
	void prompt(const std::string& message, T& out)
	{
		prompt(message, out, [](T& out) { return true; });
	}

	// https://stackoverflow.com/a/41476293
	template<typename T, typename UnaryPredicate>
	void prompt(const std::string& message, T& out, UnaryPredicate pred)
	{
		while (true)
		{
			std::cout << message;
			std::cin >> out;
			if (std::cin.fail() || !pred(out))
			{
				std::cout << "Invalid Entry\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else break;
		}
	}

	// https://stackoverflow.com/a/18028927
	KANALYSIS_INLINE void show_console_cursor(bool flag)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(out, &cursor_info);
		cursor_info.bVisible = flag;
		SetConsoleCursorInfo(out, &cursor_info);
	}
} // namespace kanalysis::utils::console