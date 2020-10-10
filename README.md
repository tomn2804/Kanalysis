# Kanalysis
A fast and simple library to compute Kruskal's Relative Importance Analysis.

## Quick preview:
```c++
#include <kanalysis.h>

int main()
{
	using namespace kanalysis;
	Matrix x = Matrix::Random(5, 5);
	Matrix y = Matrix::Random(5, 5);

	Decomposition<Matrix> qr(x);
}
```