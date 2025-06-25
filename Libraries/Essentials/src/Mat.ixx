export module Peio.Essentials.Mat;

export import <string>;
import Peio.Essentials.Types;
import Peio.Essentials.Vec;

export namespace Peio {

	namespace Essentials {

		template <typename Type, size_t n, size_t m>
		struct Mat {

			Vec<Vec<Type, n>, m> cols{};

			Mat() {}

			Mat(std::initializer_list<std::initializer_list<Type>> lists) {
				size_t i = 0;
				for (const std::initializer_list<Type>& list : lists) {
					size_t j = 0;
					for (const Type& elem : list)
						cols[j++][i] = elem;
					i++;
				}
			}

			Type& At(size_t i, size_t j) {
				return cols[j][i];
			}
			const Type& At(size_t i, size_t j) const {
				return cols[j][i];
			}

		};

		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator+(Mat<Type, n, m> mat1, Mat<Type, n, m> mat2) {
			Mat<Type, n, m> result;
			for (size_t j = 0; j < m; j++)
				result.cols[j] = mat1.cols[j] + mat2.cols[j];
			return result;
		}
		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator-(Mat<Type, n, m> mat1, Mat<Type, n, m> mat2) {
			Mat<Type, n, m> result;
			for (size_t j = 0; j < m; j++)
				result.cols[j] = mat1.cols[j] - mat2.cols[j];
			return result;
		}
		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator*(Mat<Type, n, m> mat1, Type s) {
			Mat<Type, n, m> result;
			for (size_t j = 0; j < m; j++)
				result.cols[j] = mat1.cols[j] * s;
			return result;
		}
		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator*(Type s, Mat<Type, n, m> mat1) {
			return mat1 * s;
		}
		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator/(Mat<Type, n, m> mat1, Type s) {
			Mat<Type, n, m> result;
			for (size_t j = 0; j < m; j++)
				result.cols[j] = mat1.cols[j] / s;
			return result;
		}
		template <typename Type, size_t n, size_t m>
		Mat<Type, n, m> operator-(Mat<Type, n, m> mat) {
			return (Type)(-1) * mat;
		}

		template <typename Type, size_t n, size_t m>
		Vec<Type, n> operator*(Mat<Type, n, m> mat, Vec<Type, m> v) {
			Vec<Type, n> result{};
			for (size_t j = 0; j < m; j++)
				result = result + (mat.cols[j] * v[j]);
			return result;
		}
		template <typename Type, size_t n1, size_t m1n2, size_t m2>
		Mat<Type, n1, m2> operator*(Mat<Type, n1, m1n2> mat1, Mat<Type, m1n2, m2> mat2) {
			Mat<Type, n1, m2> result;
			for (size_t j = 0; j < m2; j++)
				result.cols[j] = mat1 * mat2.cols[j];
			return result;
		}

		template <typename Type, size_t n, size_t m>
		std::string ToString(Mat<Type, n, m> mat, std::string prefix = "[", std::string elemSep = ", ", std::string rowSep = "\n", std::string suffix = "]") {
			std::string result = prefix;
			for (size_t i = 0; i < n; i++)
				for (size_t j = 0; j < m; j++)
					result += std::to_string(mat.At(i, j)) + ((j == m - 1) ? ((i == n - 1) ? suffix : rowSep) : elemSep);
			return result;
		}

	}
	using namespace Essentials;

}