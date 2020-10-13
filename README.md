# Development in Progress

**LICENSE file has not been added**.
This library is not ready for distribution, as further documentations and adjustments are still needed.

This project repository is only made public for preview purposes.

---

# Kanalysis

A simple and high-performance C++17 header-only library, used to compute Kruskal's Relative Importance Analysis and other statistical models.

If you have a lot of CPU processors and would like to compute the full Kruskal's formula, then this library is just for you.

**Feature highlights:**

- Weighted statistics
- Multi-threads
- Intel MKL

---

## Table of contents

- [Quick preview](#Quick-preview)
  - [Unweighted](#Unweighted)
  - [Weighted](#Weighted)
- [Installation](#Installation)
  - [Quick installation](#Quick-installation)
  - [Manual installation](#Manual-installation)
    - [Installing dependencies](#Installing-dependencies)
    - [Installing Kanalysis](#Installing-Kanalysis)
- [Using Kanalysis within the stats namespace](#Using-Kanalysis-within-the-stats-namespace)
  - [The Decomposition class](#The-Decomposition-class)
  - [The DecompositionWeight class](#The-DecompositionWeight-class)
    - [Standardizing data to weights](#Standardizing-data-to-weights)
  - [Solver classes](#Solver-classes)
    - [Coefficient and CoefficientWeight classes](#Coefficient-and-CoefficientWeight-classes)
    - [FittedValue and FittedValueWeight classes](#FittedValue-and-FittedValueWeight-classes)
    - [Residual and ResidualWeight classes](#Residual-and-ResidualWeight-classes)
    - [Correlation and CorrelationWeight classes](#Coefficient-and-CoefficientWeight-classes)
    - [PartialCorrelation and PartialCorrelationWeight classes](#Coefficient-and-CoefficientWeight-classes)
    - [Kruskal and KruskalWeight classes](#Kruskal-and-KruskalWeight-classes)
  - [Benchmarks](#Benchmarks)
  - [Acknowledgements and references](#Acknowledgements-and-references)

---

## Quick preview

Note: All data used in this documentation are generated randomly at compile time, so the output values will vary on each compile.

### Unweighted

```c++
#include "examples/titanic.h"
#include "include/kanalysis.h"

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
}
```

The above code will produce the following output:

```
Progress: 100%
 0.246397
 0.593792
0.0573603
0.0149979
0.0106356
0.0768176
```

### Weighted

```c++
#include "examples/titanic.h"
#include "include/kanalysis.h"

using namespace kanalysis::stats;
using namespace kanalysis::utils; // For as_model_matrix
using namespace kanalysis;

int main()
{
	int threads = 8;

	Titanic data; // Load in example data set
	Array weights = Array::Constant(data.rows(), 1);

	Matrix std_x = WeightFunction::standardize(as_model_matrix(data.x), weights);
	Matrix std_y = WeightFunction::standardize(data.y, weights);

	auto qr = decomposition(std_x, weights);
	auto results = kruskal(qr).solve(std_y, threads);

	std::cout << results << '\n';
}
```

The above code will produce the following output:

```
Progress: 100%
 0.246397
 0.593792
0.0573603
0.0149979
0.0106356
0.0768177
```

Note: ```weights``` is an array of constant value of 1, so the results will remain the same as unweighted.

---

## Installation

### Quick installation

For a quick a simple installation, just use the Git command below to clone this repository and all it's dependencies.

```
cd users/local/folder
git clone --recurse https://github.com/tomn2804/Kanalysis.git
```

Then add the following macro to the top of your project header ```.h``` or source ```.cpp``` files.

```c++
#include <users/local/folder/Kanalysis/Kanalysis/include/kanalysis.h>
```

### Manual installation

The quick installation process above requires navigating through absolute path and is cumbersome.
So, for a relative path approach, this is 2 steps process.

The first step is to install the dependencies and intergrate it into your project.
Then the second step is to install the Kanalysis library itself.

#### Installing dependencies

C++17 or later is obviously required. Other than that, the following list is the external/third-party libraries needed to be install before using the ```#include <kanalysis.h>```.

- [Eigen](http://eigen.tuxfamily.org)
  - [Intel MKL](https://software.intel.com/content/www/us/en/develop/tools/math-kernel-library.html) (Optional for improving performance on Intel processors)
- [Discreture](https://github.com/mraggi/discreture)
  - [Boost](https://www.boost.org/) (Comes **pre-installed** if clone directly from this repository, so no need to take action here)
- [Armadillo](http://arma.sourceforge.net) (Optional for reading data from ```.csv``` file in test cases)

This documentation won't go into details on how to install each of these external/third-party libraries, but the instruction for each libraries can be found within their respective websites.

By default, the Kanalysis's "config.h" file will contain the ```#include``` macros for each of these libraries.
If this is not desired, then the user can disable this by providing a ```#undef KANALYSIS_INCLUDE_DEPENDENCIES``` macro before including the "kanalysis.h" file.
After disabling, the user is responsible for including each of these libraries manually.

####  Installing Kanalysis

Since this is a header-only library, just copy the [include](https://github.com/tomn2804/Kanalysis/tree/master/Kanalysis/include) folder to your project's include folder or wherever you like.
Then add the following include macro to the top of your project header ```.h``` or source ```.cpp``` files.

```#include <kanalysis.h>```

Make sure the path of other files within the [include](https://github.com/tomn2804/Kanalysis/tree/master/Kanalysis/include) folder remains the same, relative to the "kanalysis.h" file.

---

## Using Kanalysis within the stats namespace

This library was solely written to solve the Kruskal's Relative Importance Analysis.
However, this complex computation had led to many other objects being created as a by-product of refactoring out codes.
These objects can be reused for other general purposes.

Just like how the [Matrix](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html) class is the main workhorse for the [Eigen](http://eigen.tuxfamily.org) library.
The main workhorse for this Kanalysis library are:

- [Decomposition](#The-Decomposition-Class)
- [DecompositionWeight](#The-DecompositionWeight-Class)

These 2 classes are a wrapper class around the Eigen [Matrix](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html) class and it's [QR decomposition](https://eigen.tuxfamily.org/dox/classEigen_1_1HouseholderQR.html).

### The Decomposition class

This class is used to construct the following solver classes/objects:

- [Coefficient](#Coefficient-and-CoefficientWeight-Classes)
- [FittedValue](#FittedValue-and-FittedValueWeight-Classes)
- [Residual](#Residual-and-ResidualWeight-Classes)
- [Correlation](#Correlation-and-CorrelationWeight-Classes)
- [PartialCorrelation](#PartialCorrelation-and-PartialCorrelationWeight-Classes)
- [Kruskal](#Kruskal-and-KruskalWeight-Classes)

#### Example

```c++
#include <kanalysis.h>

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

// CRTP is implemented for all solver classes.
template<typename Derived>
void print_dimension(const SolveHolderBase<Derived>& x)
{
	const auto& solver = x.derived();
	std::cout << solver.rows() << ' ' << solver.cols() << std::endl;
}

int main()
{
	Matrix x = as_model_matrix(Matrix::Random(100, 5)); // Independent variables
	Vector y = Vector::Random(100); // Dependent variable

	auto qr = decomposition(x);

	Coefficient<Decomposition<Matrix>> b(qr);
	print_dimension(b);

	FittedValue<Decomposition<Matrix>> y_hat(qr);
	print_dimension(y_hat);

	Residual<Decomposition<Matrix>> e(qr);
	print_dimension(e);

	Correlation<Decomposition<Matrix>> r(qr);
	print_dimension(r);

	PartialCorrelation<Decomposition<Matrix>> pr(qr);
	print_dimension(pr);

	Kruskal<Decomposition<Matrix>> k(qr);
	print_dimension(k);
}
```

The output of the above code will be in the following:

```
Number of rows: 100
Number of columns: 6

Number of rows: 100
Number of columns: 6

Number of rows: 100
Number of columns: 6

Number of rows: 100
Number of columns: 6

Number of rows: 100
Number of columns: 6

Number of rows: 100
Number of columns: 6
```

Note: The number of columns is 6 instead of 5 because ```as_model_matrix``` is being called to convert a regular matrix into a simple or multiple regression [model matrix](https://en.wikipedia.org/wiki/Design_matrix).

### The DecompositionWeight class

This class is used to construct the following solver classes/objects:

- [CoefficientWeight](#Coefficient-and-CoefficientWeight-Classes)
- [FittedValueWeight](#FittedValue-and-FittedValueWeight-Classes)
- [ResidualWeight](#Residual-and-ResidualWeight-Classes)
- [CorrelationWeight](#Correlation-and-CorrelationWeight-Classes)
- [PartialCorrelationWeight](#PartialCorrelation-and-PartialCorrelationWeight-Classes)
- [KruskalWeight](#Kruskal-and-KruskalWeight-Classes)

#### Standardizing data to weights

When inputing data into an object with "Weight" as a suffix, **all data must be standardized**. This applies for the **DecompositionWeight** class and all it's solver classes.

To know for sure, which parameter requires the data to be standardize, just check if the parameter names contain a ```std_``` prefix to it. **The user is responsible for checking this**.

If the parameter names contain a ```std_``` prefix then the input data must be standardize using the ```WeightFunction::standardize``` fuction or the ```standardize``` member function of the decomposition class.
The following example will illustrate this.

#### Example

```c++
#include <kanalysis.h>

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

int main()
{
	Matrix x = Matrix::Random(100, 5); // Independent variables
	Vector y = Vector::Random(100); // Dependent variable
	Array w = Array::Constant(100, 1); // Weights

	auto unweighted_qr = decomposition(x); // Valid
	//auto weighted_qr = decomposition(x, w); // Invalid, x is not weighted/standardized

	Matrix std_x = WeightFunction::standardize(x, w);
	auto weighted_qr = decomposition(std_x, w); // Valid, x is now weighted/standardized

	Vector results_for_unweighted_data = coefficient(unweighted_qr).solve(y); // Valid
	//Vector results_for_weighted_data = coefficient(weighted_qr).solve(y); // Invalid, y is not weighted/standardized

	Vector std_y = WeightFunction::standardize(y, w);
	Vector results_for_weighted_data = coefficient(weighted_qr).solve(std_y); // Invalid, y is now weighted/standardized

	// A more efficient way to standardize the data is
	// by using the pre-computed weighting data inside the decomposition object.
	Vector also_std_y = weighted_qr.standardize(y);
	Vector same_results_for_weighted_data = coefficient(weighted_qr).solve(also_std_y); // Valid, y is still weighted/standardized
}
```

Note: The example above applies for all weighted solver classes and the DecompositionWeight class itself.

---

### Solver classes

For this section, the following code will be used as the global variables all examples.

```c++
Matrix x(7, 5); // Data with 5 independent variables, and 7 observations
for (Index i = 0; i < x.cols(); ++i)
{
	x.col(i).fill(i + 40);
}

Vector y = Vector::Constant(8, 33); // Dependent variable
Array w = Array::Constant(8, 1); // Weights

Matrix std_x = WeightFunction::standardize(x, w);
Matrix std_y = WeightFunction::standardize(y, w);

std::cout << 'y' << std::endl;
std::cout << "--" << std::endl;
std::cout << y << std::endl << std::endl;

std::cout << "x1 x2 x3 x4 x5" << std::endl;
std::cout << "--------------" << std::endl;
std::cout << x << std::endl;
```

The output of the above code will be in the following:

```
y
--
33
33
33
33
33
33
33
33

x1 x2 x3 x4 x5
--------------
40 41 42 43 44
40 41 42 43 44
40 41 42 43 44
40 41 42 43 44
40 41 42 43 44
40 41 42 43 44
40 41 42 43 44
```

The data is initialized in this particular way instead of being ramdonly generated is for clearer visualization later on.

#### Coefficient and CoefficientWeight classes

These 2 classes are used to find the beta coefficients **b** of a linear model.

y = **b**X + e

##### Example

```c++
auto solver = coefficient(decomposition(x)); // Returns a Coefficient object
Vector beta_coefficients = solver.solve(y);

auto weighted_solver = coefficient(decomposition(std_x, w)); // Returns a CoefficientWeight object
Vector weighted_beta_coefficients = weighted_solver.solve(std_y);
```

#### FittedValue and FittedValueWeight classes

These 2 classes are used to find the fitted values **ŷ** of a linear model.

**ŷ** = bX

##### Example

```c++
auto solver = fitted_value(decomposition(x)); // Returns a FittedValue object
Vector fitted_values = solver.solve(y);

auto weighted_solver = fitted_value(decomposition(std_x, w)); // Returns a FittedValueWeight object
Vector weighted_fitted_values = weighted_solver.solve(std_y);
```

#### Residual and ResidualWeight classes

These 2 classes are used to find the residuals **e** of a linear model.

**e** = y - ŷ

##### Example

```c++
auto solver = residual(decomposition(x)); // Returns a Residual object
Vector residuals = solver.solve(y);

auto weighted_solver = residual(decomposition(std_x, w)); // Returns a ResidualWeight object
Vector weighted_residuals = weighted_solver.solve(std_y);
```

#### Correlation and CorrelationWeight classes

These 2 classes are used to find the [Pearson correlation coefficient](https://en.wikipedia.org/wiki/Pearson_correlation_coefficient) **r** between 2 variables.

##### Example

```c++
auto solver = correlation(decomposition(x)); // Returns a Correlation object
Scalar r = solver.solve(y), 2); // Finds the correlation between y and x
Scalar r_squared = std::pow(r, 2);

auto weighted_solver = correlation(decomposition(std_x, w)); // Returns a CorrelationWeight object
Scalar weighted_r = weighted_solver.solve(std_y), 2); // Finds the correlation between y and x, given weights
Scalar weighted_r_squared = std::pow(weighted_r, 2);
```

#### PartialCorrelation and PartialCorrelationWeight classes

These 2 classes are used to find the correlation **r** between 2 variables while controlling for other variables, also known as [partial correlation](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4681537/).

##### Example

Here is an example of how to find the partial correlation between y and x1, while controlling for x2, x3, x4, and x5.

```c++
Vector x1 = x.col(0); // Takes the first independent variable x1 out, to be correlate with y
Matrix x_to_ctrl = x.rightCols(x.cols() - 1); // Creates a new matrix without x1

std::cout << x1 << std::endl;
std::cout << x_to_ctrl << std::endl;
```

The output of the above code will be in the following:

```
x1
--
40
40
40
40
40
40
40

x2 x3 x4 x5
-----------
41 42 43 44
41 42 43 44
41 42 43 44
41 42 43 44
41 42 43 44
41 42 43 44
41 42 43 44
```

With the variables set, the following code is how to solve it.

```c++
auto partial_r_squared = partial_correlation(decomposition(x_to_ctrl)); // Returns a PartialCorrelation object
Scalar partial_r = r.solve(y, x1); // Finds the correlation between y and x1 while controlling for x2, x3, x4, x5.
Scalar partial_r_squared = std::pow(partial_r, 2);

// Standardize the data to weights
Matrix std_x_to_ctrl = WeightFunction::standardize(x_to_ctrl, w);
Vector std_x1 = WeightFunction::standardize(x1, w);

auto weighted_partial_r_squared = correlation(decomposition(std_x_to_ctrl, w)); // Returns a PartialCorrelationWeight object
Scalar weighted_partial_r_squared = std::pow(weighted_r.solve(std_x1), 2); // Finds the correlation between y and x1 while controlling for x2, x3, x4, x5, given weights.
```

#### Kruskal and KruskalWeight classes

These 2 classes are used to find the relative importance using the Kruskal's method.

These are the only classes that have multi-threading built in.

**Warning: The matrix x must be a [model matrix](https://en.wikipedia.org/wiki/Design_matrix)!**

In order words, there must be a y-intercept column appended to the front of the matrix **x**.
If the y-intercept is not desired, then fill the column with the value 0.

##### Example

```c++
int threads = 8; // Number of threads to be run in parallel

Matrix model_matrix = utils::as_model_matrix(x); // Create a model matrix from x

auto solver = kruskal(decomposition(model_matrix)); // Returns a Kruskal object
Vector relative_importances = solver.solve(y, threads); // Finds the relative importances each x variables toward y

Matrix std_model_matrix = WeightFunction::standardize(model_matrix, w);

auto weighted_solver = kruskal(decomposition(std_model_matrix, w)); // Returns a KruskalWeight object
Vector weighted_relative_importances = weighted_solver.solve(std_y, threads); // Finds the relative importances each x variables toward y
```

The order of the results vector ```relative_importances``` correspond to the ```model_matrix``` by columns, excluding the y-intercept column.

To illustrate:

If the ```model_matrix``` looks like this

| y-intercept | x1 | x2 | x3 | x4 | x5 |
|-------------|----|----|----|----|----|
| 1           | 1  | 2  | 3  | 4  | 5  |
| 1           | 6  | 7  | 8  | 9  | 10 |
| 1           | 11 | 12 | 13 | 14 | 15 |

Then the output vector ```relative_importances``` will be the "Relative Importance" column below, where the variables correspond accordingly.

| Variable | Relative Importance |
|----------|---------------------|
| x1       | 0.130               |
| x2       | 0.430               |
| x3       | 0.252               |
| x4       | 0.142               |
| x5       | 0.460               |

---

## Benchmarks

Throwing in as many CPU processors as you can into the problems, is the mindset behind this library design.

---

## Acknowledgements and references

- [Nguyen Do](https://vn.linkedin.com/in/nguyen-do-a7828294) for providing resources to calculating the partial correlation and providing the Kruskal's formula.
He is the one that got this project started off the ground.

- Kim, S. (2015). Ppcor: An R Package for a Fast Calculation to Semi-partial Correlation Coefficients. Communications for Statistical Applications and Methods, 22(6), 665-674. doi:10.5351/csam.2015.22.6.665

- [OLS in Matrix Form](https://web.stanford.edu/~mrosenfe/soc_meth_proj3/matrix_OLS_NYU_notes.pdf)
