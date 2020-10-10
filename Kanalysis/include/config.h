#pragma once

//
// User settings
//
#define KANALYSIS_FORCE_INLINE
#define KANALYSIS_INCLUDE_DEPENDENCIES
//#define KANALYSIS_USE_MKL

//
// Global macros and headers
//
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#ifdef KANALYSIS_INCLUDE_DEPENDENCIES

#ifdef KANALYSIS_USE_MKL
#include <mkl.h>
#define EIGEN_USE_MKL_ALL
#endif // KANALYSIS_USE_MKL

#define EIGEN_MAX_CPP_VER 17

#include <discreture.hpp>
#include <Eigen/Dense>

#endif // KANALYSIS_INCLUDE_DEPENDENCIES

#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

#ifdef KANALYSIS_FORCE_INLINE
#define KANALYSIS_INLINE __forceinline
#else
#define KANALYSIS_INLINE inline
#endif // KANALYSIS_FORCE_INLINE

//
// Global types and aliases
//
namespace kanalysis
{
	using Index = Eigen::Index; // Signed interger
	using Scalar = float;
	using UInt = std::uintmax_t;

	using Array = Eigen::Array<Scalar, Eigen::Dynamic, 1>;
	using Matrix = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;
	using Vector = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;

	template<typename DenseType>
	using Map = Eigen::Map<DenseType>;

	template<typename DenseType>
	using Ref = Eigen::Ref<DenseType>;

	template<typename Derived>
	using DenseBase = Eigen::DenseBase<Derived>;

	template<typename Derived>
	using ArrayBase = Eigen::ArrayBase<Derived>;

	template<typename Derived>
	using MatrixBase = Eigen::MatrixBase<Derived>;

	template<typename Derived>
	using VectorBase = Eigen::MatrixBase<Derived>;

	template<typename Derived>
	using MapBase = Eigen::MapBase<Derived>;

	template<typename MatrixType>
	using HouseholderQR = Eigen::HouseholderQR<MatrixType>;
}
