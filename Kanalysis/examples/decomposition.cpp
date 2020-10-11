#include "include/stats/decomposition.h"
#include "include/stats/decomposition_weight.h"
#include "include/utils/utils.h"

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

// The DecompositionBase class can be used as function parameters to accept a regular Decomposition object or a weighted DecompositionWeight object.
template<typename Dervied>
void print_matrix_qr(const DecompositionBase<Dervied>& qr_base)
{
	const auto& qr = qr_base.derived(); // Can be a Decomposition object or a DecompositionWeight object
	std::cout << qr.householder_qr().matrixQR() << std::endl; // Print out the QR decomposition of x
}

// The Decomposition class has a very similar interface to the Eigen QR module.
int main()
{
	int rows = 5;
	int cols = 5;

	// Create a model matrix with randomized data
	Matrix x = as_model_matrix(Matrix::Random(rows, cols));

	// Construct the decomposition
	Decomposition<Matrix> qr(x); // x must be a model matrix

	// Create a new model matrix
	Matrix new_x = as_model_matrix(Matrix::Random(rows, cols));
	qr.compute(new_x); // Re-compute the QR decomposition to the new model matrix
	print_matrix_qr(qr);

	{
		Matrix user_responsibility = x; // Copy the matrix

		// Using the Eigen Ref wrapper class,
		// this Decomposition object will store a reference instead of a copy to the user_responsibility matrix.
		//
		// However, the user must keep this matrix alives as long as this Decomposition object lives.
		Decomposition<Ref<Matrix>> qr(user_responsibility);

		user_responsibility.~Matrix(); // Calling destructor
		// 	print_matrix_qr(qr); // Undefined behavior!
	}

	// For weight
	Array w = Array::Constant(rows, 1);

	Matrix std_x = WeightFunction::standardize(x, w); // Standardize the matrix x
	DecompositionWeight<Matrix, Array> weighted_qr(std_x, w);

	std::system("pause");
}