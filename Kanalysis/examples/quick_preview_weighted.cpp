#include "kanalysis.h"
#include "titanic.h"

using namespace kanalysis::stats;
using namespace kanalysis::utils; // For as_model_matrix
using namespace kanalysis;

int main()
{
	int threads = 8;

	Titanic data; // Load in example data set
	Array weights = Array::Constant(data.rows(), 1);

	Matrix std_x = WeightFunction::standardize(as_model_matrix(data.x), weights);
	Vector std_y = WeightFunction::standardize(data.y, weights);

	auto qr = decomposition(std_x, weights);
	Vector results = kruskal(qr).solve(std_y, threads);

	std::cout << results << '\n';
	std::system("pause"); // Prevent console from closing
}

// The above code will produce the following output:

//Progress: 100 %
//0.246397
//0.593792
//0.0573603
//0.0149979
//0.0106356
//0.0768176
//Press any key to continue . . .