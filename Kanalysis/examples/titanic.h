#pragma once

#include "include/config.h"

#include <armadillo> // To read ".csv" files

#define TITANIC_FILE_PATH "../../../../examples/data/titanic.csv"

// First 5 (excluding headers) rows of the "titanic.csv" dataset:
//
//| Survived | Pclass | Sex | Age | SibSp | Parch |   Fare  |
//|:--------:|:------:|:---:|:---:|:-----:|:-----:|:-------:|
//| 0        | 3      | 2   | 22  | 1     | 0     | 7.25    |
//| 1        | 1      | 1   | 38  | 1     | 0     | 71.2833 |
//| 1        | 3      | 1   | 26  | 0     | 0     | 7.925   |
//| 1        | 1      | 1   | 35  | 1     | 0     | 53.1    |
//| 0        | 3      | 2   | 35  | 0     | 0     | 8.05    |
// ... 709 out of 714 (excluding headers) rows omitted.
//
// Original data sets can be found [here](https://www.kaggle.com/c/titanic/data).

namespace kanalysis
{
	KANALYSIS_INLINE Matrix read_csv(const std::string& path)
	{
		using namespace arma;

		// Import the data set
		Mat<Scalar> data;
		data.load(path, csv_ascii);

		// Remove the first header row
		Col<uword> row_indices(data.n_rows - 1);
		std::iota(row_indices.begin(), row_indices.end(), 1);
		data = data.rows(row_indices);

		return Map<Matrix>(data.memptr(), data.n_rows, data.n_cols);
	}
}
