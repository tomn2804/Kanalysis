# Kanalysis

A fast and simple C++17 header-only library to compute Kruskal's Relative Importance Analysis.

## Quick preview:

```c++
#include <kanalysis.h>

using namespace kanalysis;
using namespace kanalysis::stats;

int main()
{
	Matrix data = Matrix::Random(100, 5);
	Matrix x = utils::as_model_matrix(data);
	Vector y = Vector::Random(100);

	Decomposition<Matrix> qr(x);
	auto k = kruskal(qr);

	int threads = 8;
	Vector results = k.solve(y, threads);

	std::cout << results << std::endl;
}
```

The output will be:

```
Progress: 100%
0.00407495
0.00613109
  0.249574
  0.174478
  0.565742
```

Note: The data is generated randomly so the actual output values will vary.

---

## Installation

### Quick and Dirty Installation
For a quick a simple process, just use the Git command below to clone this repository and all it's dependencies.

```
cd users/local/folder
git clone --recurse https://github.com/tomn2804/Kanalysis.git
```

Then add the C++ code line below to the top of your project header ```.h``` or source ```.cpp``` files.
```c++
#include <users/local/folder/Kanalysis/Kanalysis/include/kanalysis.h>
```

### Manual Installation

The quick installation process above requires absolute path and cumbersome.
So, for a relative path installation, this is 2 steps process.

The first step is to install the dependencies, and the second step is to install the Kanalysis library itself.

#### First Step - Installing Dependencies

C++17 or later is obviously required. Other than that, here is the following list of external libraries needed to be install before using ```#include <kanalysis.h>```.

- [Eigen](http://eigen.tuxfamily.org)
  - [Intel MKL](https://software.intel.com/content/www/us/en/develop/tools/math-kernel-library.html) (Optional for improve performance on Intel processors)
- [Discreture](https://github.com/mraggi/discreture)
  - [Boost](https://www.boost.org/) (Comes pre-installed if clone from this repository, so no need to take action here)
- [Armadillo](http://arma.sourceforge.net) (Optional for reading data from ```.csv``` files for test case)

By default, the Kanalysis's "config.h" file will ```#include``` these dependencies automatically.
To disable this, just provide ```#undef KANALYSIS_INCLUDE_DEPENDENCIES``` macro before including the "kanalysis.h" file.
After disabling this, the user is responsible for including each dependencies manually.

#### Second Step - Installing Kanalysis

Since this is a header-only library, just copy the [include](https://github.com/tomn2804/Kanalysis/tree/master/Kanalysis/include) folder to your project's include folders or wherever you like.
Then include the "kanalysis.h" file at the top of your project header ```.h``` or source ```.cpp``` files.

Make sure the path of other files within the [include](https://github.com/tomn2804/Kanalysis/tree/master/Kanalysis/include) folder remains the same, relative to the "kanalysis.h" file.

---

## Using the Library
