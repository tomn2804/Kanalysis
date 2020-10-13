#include "kanalysis.h"
#include "titanic.h"

using namespace kanalysis::stats;
using namespace kanalysis::utils; // For as_model_matrix
using namespace kanalysis;

int main()
{
	int threads = 8;

	Titanic data; // Load in example data set

	auto qr = decomposition(as_model_matrix(data.x));
	auto results = kruskal(qr).solve(data.y, threads);

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