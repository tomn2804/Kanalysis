#pragma once

#include "include/config.h"

#include <armadillo>

#define TRAIN_DATA_PATH "../../../../external/titanic/inst/data-raw/gendermodel.csv"

// Including the header names, this is the first 6 rows of the "train.csv" file
//
//| PassengerId | Survived | Pclass | Name                                                | Sex    | Age | SibSp | Parch | Ticket           | Fare    | Cabin | Embarked |
//|-------------|----------|--------|-----------------------------------------------------|--------|-----|-------|-------|------------------|---------|-------|----------|
//|           1 |        0 |      3 | Braund, Mr. Owen Harris                             | male   |  22 |     1 |     0 | A/5 21171        |    7.25 |       | S        |
//|           2 |        1 |      1 | Cumings, Mrs. John Bradley (Florence Briggs Thayer) | female |  38 |     1 |     0 | PC 17599         | 71.2833 | C85   | C        |
//|           3 |        1 |      3 | Heikkinen, Miss. Laina                              | female |  26 |     0 |     0 | STON/O2. 3101282 |   7.925 |       | S        |
//|           4 |        1 |      1 | Futrelle, Mrs. Jacques Heath (Lily May Peel)        | female |  35 |     1 |     0 |           113803 |    53.1 | C123  | S        |
//|           5 |        0 |      3 | Allen, Mr. William Henry                            | male   |  35 |     0 |     0 |           373450 |    8.05 |       | S        |
//| ...         | ...      | ...    | ...                                                 | ...    | ... | ...   | ...   | ...              | ...     | ...   | ...      |

namespace kanalysis
{
	void read_csv(const std::string& path)
	{
		arma::Mat<Scalar> data;

		arma::field<std::string> headers(1);
		headers(0) = "PassengerId";
		////headers(1) = "Pclass";
		////headers(2) = "Age";
		////headers(3) = "SibSp";
		////headers(4) = "Fare";

		auto names = arma::csv_name(path, headers);
		data.load(names);
		data.print();
		//Map<Matrix> m(data.memptr(), data.n_rows, data.n_cols);
		//std::cout << m << '\n';

		//arma::Mat<Scalar> data;
		//data.load(path);

		//arma::Col<arma::uword> row_indices(data.n_rows - 1);
		//std::iota(row_indices.begin(), row_indices.end(), 1);
		//data = data.rows(row_indices);

		//arma::Mat<Scalar> model_matrix(data.colptr(0), data.n_rows, 4, false, true);
		//model_matrix.print();
	}
}
