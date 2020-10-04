#pragma once

//#define BREG_USE_MKL

#if defined(BREG_USE_MKL)
#include <mkl.h>
#define EIGEN_USE_MKL_ALL
#endif // defined(BREG_USE_MKL)

#include <algorithm>
#include <discreture.hpp>
#include <Eigen/Dense>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

#define KANALYSIS_INLINE __forceinline

namespace kanalysis
{
	using Index = Eigen::Index;
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
