#include "include/stats/decomposition_weight.h"
#include "include/utils/utils.h"

// The DecompositionWeight operates very similiar to the regular Decomposition class.
// Here is the differences between the 2 classes.
int main()
{
	using namespace kanalysis;
	using namespace kanalysis::stats;
	using namespace kanalysis::utils;

	int rows = 5;
	int cols = 5;

	// Create a model matrix with randomized data
	Matrix x = as_model_matrix(Matrix::Random(rows, cols));

	// Construct the decomposition
	Decomposition<Matrix> qr(x); // x must be a model matrix
	std::cout << qr.householder_qr().matrixQR() << std::endl; // Print out the QR decomposition of x

	// Create a new model matrix
	Matrix new_x = as_model_matrix(Matrix::Random(rows, cols));
	qr.compute(new_x); // Re-compute the QR decomposition to the new model matrix
	std::cout << qr.householder_qr().matrixQR() << std::endl; // Print out the new QR decomposition

	{
		Matrix user_responsibility = x; // Copy the matrix

		// Using the Eigen Ref wrapper class,
		// this Decomposition object will store a reference instead of a copy to the user_responsibility matrix.
		//
		// However, the user must keep this matrix alives as long as this Decomposition object lives.
		Decomposition<Ref<Matrix>> qr(user_responsibility);

		user_responsibility.~Matrix(); // Calling destructor
		// std::cout << qr.rows() << std::endl; // Undefined behavior!
	}

	std::system("pause");
}