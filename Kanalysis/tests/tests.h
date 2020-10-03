#pragma once

#include "include/config.h"

#include <chrono>
#include <iostream>
#include <vector>

#define BENCHMARK_WARMUP_RUNS 10
#define DATA_DIRECTORY "../../../../tests/data/"

#define DEBUG_COUT(x) std::cout << x
#define DEBUG_PRINT(...) kanalysis::utils::console::print(__VA_ARGS__)
#define DEBUG_TIME() kanalysis::tests::Time T

namespace kanalysis::tests
{
	class Time
	{
	public:
		Time();
		~Time();
		const std::chrono::time_point<std::chrono::steady_clock>& start();
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_start;
		friend std::ostream& operator<<(std::ostream& os, const Time& rhs);
	};

	template<typename F>
	void benchmark(const std::vector<F>& functions, int replications);

	class Data
	{
	public:
		Data(const std::string& filename, int variables);
		Data(const std::string& filename, int variables, int nth_order);
		Data(const std::string& filename, int variables, int nth_order, int threads);

		const std::string& filename() const;
		int variables() const;
		int nth_order() const;
		int threads() const;

		const Matrix& model_matrix() const;
		const Vector& y() const;
		const Vector& weights() const;
	private:
		std::string m_filename;
		int m_variables;
		int m_nth_order;
		int m_threads;

		Matrix m_model_matrix;
		Vector m_y;
		Vector m_weights;
	};
} // namespace kanalysis::tests

namespace kanalysis::tests
{
	template<typename F>
	void benchmark(const std::vector<F>& functions, int replications)
	{
		using namespace std::chrono;

		std::vector<std::vector<milliseconds>> durations(functions.size(), std::vector<milliseconds>(replications));

		for (int i = 0; i < BENCHMARK_WARMUP_RUNS; ++i)
		{
			for (const auto& func : functions)
			{
				func();
			}
		}

		for (int i = 0; i < functions.size(); ++i)
		{
			const F& func = functions[i];
			std::vector<milliseconds>& elapses = durations[i];

			for (auto& elapsed : elapses)
			{
				auto start = high_resolution_clock::now();
				func();
				auto stop = high_resolution_clock::now();
				elapsed = duration_cast<milliseconds>(stop - start);
			}
		}

		for (int i = 0; i < durations.size(); ++i)
		{
			std::vector<milliseconds>& elapses = durations[i];
			std::sort(elapses.begin(), elapses.end());

			std::cout << "Time Elapsed - Function " << i + 1 << ":\n";

			Scalar min = elapses[0].count();
			Scalar med = elapses[elapses.size() / 2].count();
			Scalar max = elapses[elapses.size() - 1].count();

			std::cout << "\t Min: " << min << "ms\n";
			std::cout << "\t Med: " << med << "ms\n";
			std::cout << "\t Max: " << max << "ms\n";
		}
	}
} // namespace kanalysis::tests
