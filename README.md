# Development in Progress

LICENSE file has not been added yet. This project is only made public for preview purposes.

# Kanalysis

A fast and simple C++17 header-only library to compute Kruskal's Relative Importance Analysis and linear regression.

If you have a lot of CPU processors and would like to compute the full Kruskal's formula in brute force, then this library is just for you.

**Support Key Features:**

- Weight
- Multi-threads
- Intel MKL

---

## Table of Contents

- [Quick Preview](#Quick-Preview)
  - [Unweighted](#Unweighted)
  - [Weighted](#Weighted)
- [Installation](#Installation)
  - [Quick Installation](#Quick-Installation)
  - [Manual Installation](#Manual-Installation)
    - [Installing Dependencies](#Installing-Dependencies)
    - [Installing Kanalysis](#Installing-Kanalysis)
- [Using Kanalysis Within the Stats Namespace](#Using-Kanalysis-Within-the-Stats-Namespace)
  - [The Decomposition Class](#The-Decomposition-Class)
    - [Example](#example1)
  - [The DecompositionWeight Class](#The-DecompositionWeight-Class)
    - [Key Important Differences Between the Unweighted Class and the Weighted Class](#Key-Important-Differences-Between-the-Unweighted-Class-and-the-Weighted-Class)
    - [Example](#example2)
  - [Solver Classes](#Solver-Classes)
    - [Coefficient and CoefficientWeight Classes](#Coefficient-and-CoefficientWeight-Classes)
    - [FittedValue and FittedValueWeight Classes](#FittedValue-and-FittedValueWeight-Classes)
    - [Residual and ResidualWeight Classes](#Residual-and-ResidualWeight-Classes)
    - [Correlation and CorrelationWeight Classes](#Coefficient-and-CoefficientWeight-Classes)
    - [PartialCorrelation and PartialCorrelationWeight Classes](#Coefficient-and-CoefficientWeight-Classes)
    - [Kruskal and KruskalWeight Classes](#Kruskal-and-KruskalWeight-Classes)
  - Benchmarks


---

## Quick Preview

Note: All data used in this documentation are generated randomly at compile time, so the output values will vary on each compile.

### Unweighted

```c++
#include <kanalysis.h>

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

int main()
{
	int threads = 8;

	Matrix x = Matrix::Random(100, 5); // Independent variables
	Vector y = Vector::Random(100); // Dependent variable

	auto qr = decomposition(as_model_matrix(x));
	Vector results = kruskal(qr).solve(y, threads);

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

### Weighted

```c++
#include <kanalysis.h>

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

int main()
{
	int threads = 8;

	Matrix x = Matrix::Random(100, 5); // Independent variables
	Vector y = Vector::Random(100); // Dependent variable
	Array w = Array::Constant(100, 1); // Weights

	Matrix std_x = WeightFunction::standardize(x, w);
	Matrix std_y = WeightFunction::standardize(y, w);

	auto qr = decomposition(std_x, w);
	Vector results = kruskal(qr).solve(std_y, threads);

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

Note: The output values of Weighted and Unweighted are the same because ```w``` is an array of a constant value of 1.

---

## Installation

### Quick Installation

For a quick a simple installation, just use the Git command below to clone this repository and all it's dependencies.

```
cd users/local/folder
git clone --recurse https://github.com/tomn2804/Kanalysis.git
```

Then add the following macro to the top of your project header ```.h``` or source ```.cpp``` files.

```c++
#include <users/local/folder/Kanalysis/Kanalysis/include/kanalysis.h>
```

### Manual Installation

The quick installation process above requires navigating through absolute path and is cumbersome.
So, for a relative path approach, this is 2 steps process.

The first step is to install the dependencies and intergrate it into your project.
Then the second step is to install the Kanalysis library itself.

#### Installing Dependencies

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

## Using Kanalysis Within the Stats Namespace

This library was solely written to solve the Kruskal's Relative Importance Analysis.
However, this complex computation had led to many other objects being created as a by-product of refactoring out codes.
These objects can be reused for other general purposes.

Just like how the [Matrix](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html) class is the main workhorse for the [Eigen](http://eigen.tuxfamily.org) library.
The main workhorse for this Kanalysis library are:

- [Decomposition](#The-Decomposition-Class)
- [DecompositionWeight](#The-DecompositionWeight-Class)

These 2 classes are a wrapper class around the Eigen [Matrix](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html) class and it's [QR decomposition](https://eigen.tuxfamily.org/dox/classEigen_1_1HouseholderQR.html).

### The Decomposition Class

This class is used to construct the following solver classes/objects:

- [Coefficient](#Coefficient-and-CoefficientWeight-Classes)
- [FittedValue](#FittedValue-and-FittedValueWeight-Classes)
- [Residual](#Residual-and-ResidualWeight-Classes)
- [Correlation](#Correlation-and-CorrelationWeight-Classes)
- [PartialCorrelation](#PartialCorrelation-and-PartialCorrelationWeight-Classes)
- [Kruskal](#Kruskal-and-KruskalWeight-Classes)

#### Example <a name="example1"></a>

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

	Decomposition<Matrix> qr(x);

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

The output will be:

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

### The DecompositionWeight Class

This class is used to construct the following solver classes/objects:

- [CoefficientWeight](#Coefficient-and-CoefficientWeight-Classes)
- [FittedValueWeight](#FittedValue-and-FittedValueWeight-Classes)
- [ResidualWeight](#Residual-and-ResidualWeight-Classes)
- [CorrelationWeight](#Correlation-and-CorrelationWeight-Classes)
- [PartialCorrelationWeight](#PartialCorrelation-and-PartialCorrelationWeight-Classes)
- [KruskalWeight](#Kruskal-and-KruskalWeight-Classes)

#### Key Important Differences Between the Unweighted Class and the Weighted Class

For the DecompositionWeight class and all it's solver classes, **all input arguments of the dependent variable (y) and the independent variables (x) must be standardized**.

To know for sure, which parameters require the argument data to be standardize, just check if the parameter names contain a ```std_``` prefix to it.

If the parameter names contain a ```std_``` prefix then the input data must be standardize using the ```WeightFunction::standardize``` fuction or the ```standardize``` member function of the decomposition class.

#### Example <a name="example2"></a>

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

### Solver Classes

For this section, the following code will be used as the global variables all examples.

```c++
Matrix x = Matrix::Random(100, 5); // Independent variables
Vector y = Vector::Random(100); // Dependent variable
Array w = Array::Constant(100, 1); // Weights

Matrix std_x = WeightFunction::standardize(x, w);
Matrix std_y = WeightFunction::standardize(y, w);
```

#### Coefficient and CoefficientWeight Classes

These 2 classes are used to find the beta coefficients **b** of a linear model.

y = **b**X + e

```c++
auto b = coefficient(decomposition(x)); // Returns a Coefficient class
Vector results = b.solve(y);

auto b = coefficient(decomposition(std_x, w)); // Returns a CoefficientWeight class
Vector weighted_results = b.solve(std_y);
```

#### FittedValue and FittedValueWeight Classes

These 2 classes are used to find the fitted values **ŷ** of a linear model.

**ŷ** = bX

```c++
auto y_hat = fitted_value(decomposition(x)); // Returns a FittedValue class
Vector results = y_hat.solve(y);

auto weighted_y_hat = fitted_value(decomposition(std_x, w)); // Returns a FittedValueWeight class
Vector weighted_results = weighted_y_hat.solve(std_y);
```

#### Residual and ResidualWeight Classes

These 2 classes are used to find the residuals **e** of a linear model.

**e** = y - ŷ

```c++
auto e = residual(decomposition(x)); // Returns a Residual class
Vector results = e.solve(y);

auto weighted_e = residual(decomposition(std_x, w)); // Returns a ResidualWeight class
Vector weighted_results = weighted_e.solve(std_y);
```

#### Correlation and CorrelationWeight Classes

These 2 classes are used to find the [Pearson correlation coefficient](https://en.wikipedia.org/wiki/Pearson_correlation_coefficient) **r** between 2 variables.

#### PartialCorrelation and PartialCorrelationWeight Classes

#### Kruskal and KruskalWeight Classes

---

### CRTP Designed Class

- All Unweighted and Weighted solver classes are CRTP designed and inherit from the **SolverHolderBase** class.
  - compte
  - solve
- The Decomposition and DecompositionWeight classes are CRTP designed and inherit from the **DecompositionBase** class.
