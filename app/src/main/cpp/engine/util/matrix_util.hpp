#ifndef SONAR_MATRIX_UTIL_HPP
#define SONAR_MATRIX_UTIL_HPP

#include "Eigen/Core"
#include "unsupported/Eigen/FFT"
#include "unsupported/Eigen/MatrixFunctions"
#include "unsupported/Eigen/Splines"
#include <bits/stdc++.h>

using namespace std;
using namespace Eigen;

class MatrixUtil {
public:
    template<typename T>
    static void VectorToMatrix(MatrixX<T> &m, const vector<vector<T>> &v) {
        int rows = v.size();
        assert(m.rows() == rows);

        for (int i = 0; i < rows; i++) {
            int cols = v[i].size();
            assert(m.cols() == cols);
            for (int j = 0; j < cols; j++) {
                m(i, j) = v[i][j];
            }
        }
    }

    template<typename T>
    static void MatrixToVector(vector<vector<T>> &v, const MatrixX<T> &m) {
        int rows = m.rows(), cols = m.cols();
        assert(v.size() == rows);

        for (int i = 0; i < rows; i++) {
            assert(v[i].size() == cols);
            for (int j = 0; j < cols; j++) {
                v[i][j] = m(i, j);
            }
        }
    }

    template<typename T>
    static void RealToComplex(MatrixX<complex<T>> &dst, const MatrixX<T> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(i, j) = complex<T>(src(i, j), 0);
            }
        }
    }

    template<typename T>
    static void ComplexToReal(MatrixX<T> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(i, j) = src(i, j).real();
            }
        }
    }

    template<typename T>
    static void ComplexToAbs(MatrixX<T> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(i, j) = abs(src(i, j));
            }
        }
    }

    template<typename T>
    static void Copy(MatrixX<T> &dst, const MatrixX<T> &src) {
        int rows = src.rows(), cols = src.cols();
        if (dst.rows() != rows || dst.cols() != cols) {
            dst.resize(rows, cols);
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(i, j) = src(i, j);
            }
        }
    }

    template<typename T>
    static void ComplexToMultiChannel(MatrixX<T> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows * 2 && dst.cols() == cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(2 * i, j) = real(src(i, j));
                dst(2 * i + 1, j) = imag(src(i, j));
            }
        }
    }

    template<typename T>
    static vector<T> GetRow(const MatrixX<T> &m, int row) {
        int rows = m.rows(), cols = m.cols();
        assert(row >= 0 && row < rows);

        vector<T> res(cols);
        for (int j = 0; j < cols; j++) {
            res[j] = m(row, j);
        }

        return res;
    }

    template<typename T>
    static void SetRow(MatrixX<T> &m, int row, const vector<T> &v) {
        int rows = m.rows(), cols = m.cols();
        assert(row >= 0 && row < rows && v.size() == cols);

        for (int j = 0; j < cols; j++) {
            m(row, j) = v[j];
        }
    }

    template<typename T>
    static vector<T> GetCol(const MatrixX<T> &m, int col) {
        int rows = m.rows(), cols = m.cols();
        assert(col >= 0 && col < cols);

        vector<T> res(rows);
        for (int i = 0; i < rows; i++) {
            res[i] = m(i, col);
        }

        return res;
    }

    template<typename T>
    static void SetCol(MatrixX<T> &m, int col, const vector<T> &v) {
        int rows = m.rows(), cols = m.cols();
        assert(col >= 0 && col < cols && v.size() == rows);

        for (int i = 0; i < rows; i++) {
            m(i, col) = v[i];
        }
    }

    template<typename T>
    static void CyclicShift(MatrixX<T> &m, int r, int c) {
        int rows = m.rows(), cols = m.cols();

        int row_offset = r % rows;
        if (row_offset < 0) {
            row_offset += rows;
        }
        ColReverse(m, 0, row_offset - 1);
        ColReverse(m, row_offset, m.rows() - 1);
        ColReverse(m, 0, m.rows() - 1);

        int col_offset = c % cols;
        if (col_offset < 0) {
            col_offset += cols;
        }
        RowReverse(m, 0, col_offset - 1);
        RowReverse(m, col_offset, m.cols() - 1);
        RowReverse(m, 0, m.cols() - 1);
    }

    template<typename T>
    static void RowReverse(MatrixX<T> &m, int col_start, int col_end) {
        for (int i = 0; i < m.rows(); i++) {
            for (int j = col_start, k = col_end; j < k; j++, k--) {
                T value = m(i, j);
                m(i, j) = m(i, k);
                m(i, k) = value;
            }
        }
    }

    template<typename T>
    static void ColReverse(MatrixX<T> &m, int row_start, int row_end) {
        for (int j = 0; j < m.cols(); j++) {
            for (int i = row_start, k = row_end; i < k; i++, k--) {
                T value = m(i, j);
                m(i, j) = m(k, j);
                m(k, j) = value;
            }
        }
    }

    template<typename T>
    static MatrixX<T> Dot(const MatrixX<T> &m1, const MatrixX<T> &m2) {
        int rows = m1.rows(), cols = m1.cols();
        assert(m2.rows() == rows && m2.cols() == cols);
        MatrixX<T> res(rows, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res(i, j) = m1(i, j) * m2(i, j);
            }
        }

        return res;
    }

    static complex<double> DotSum(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2) {
        int rows = (int) m1.rows(), cols = (int) m1.cols();
        assert(m2.rows() == rows && m2.cols() == cols);
        complex<double> res(0, 0);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res += m1(i, j) * m2(i, j);
            }
        }

        return res;
    }

    static double DotSum(const MatrixX<double> &m1, const MatrixX<double> &m2) {
        int rows = (int) m1.rows(), cols = (int) m1.cols();
        double res = 0.0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res += m1(i, j) * m2(i, j);
            }
        }

        return res;
    }

    template<typename T>
    static void RowFFT(MatrixX<complex<T>> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        FFT<T> fft_calculator;
        for (int i = 0; i < rows; i++) {
            vector<complex<T>> vec_freq, vec_time = GetRow(src, i);
            fft_calculator.fwd(vec_freq, vec_time);
            SetRow(dst, i, vec_freq);
        }
    }

    template<typename T>
    static void RowIFFT(MatrixX<complex<T>> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        FFT<double> fft_calculator;
        for (int i = 0; i < rows; i++) {
            vector<complex<T>> vec_time, vec_freq = GetRow(src, i);
            fft_calculator.inv(vec_time, vec_freq);
            SetRow(dst, i, vec_time);
        }
    }

    template<typename T>
    static void ColFFT(MatrixX<complex<T>> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        FFT<T> fft_calculator;
        for (int j = 0; j < cols; j++) {
            vector<complex<T>> vec_freq, vec_time = GetCol(src, j);
            fft_calculator.fwd(vec_freq, vec_time);
            SetCol(dst, j, vec_freq);
        }
    }

    template<typename T>
    static void ColIFFT(MatrixX<complex<T>> &dst, const MatrixX<complex<T>> &src) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        FFT<double> fft_calculator;
        for (int j = 0; j < cols; j++) {
            vector<complex<T>> vec_time, vec_freq = GetCol(src, j);
            fft_calculator.inv(vec_time, vec_freq);
            SetCol(dst, j, vec_time);
        }
    }

    template<typename T>
    static void MatrixPadding(MatrixX<T> &dst, const MatrixX<T> &src, int r, int c, T value) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows + r && dst.cols() == cols + c);
        dst.setConstant(value);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dst(i, j) = src(i, j);
            }
        }
    }

    template<typename T>
    static void FreqPadding(MatrixX<complex<T>> &dst, const MatrixX<complex<T>> &src, int len) {
        int rows = src.rows(), cols = src.cols();
        assert(dst.rows() == rows && dst.cols() == cols + len);
        dst.setConstant(complex<T>(0, 0));

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j < (cols + 1) / 2) {
                    dst(i, j) = src(i, j);
                } else {
                    dst(i, len + j) = src(i, j);
                }
            }
        }
    }

    static MatrixX<double> SplineInterpolation(const MatrixX<double> &raw_data, int raw_freq, int new_freq) {
        int rows = (int) raw_data.rows(), raw_cols = (int) raw_data.cols(), new_cols = raw_cols * new_freq / raw_freq;

        RowVectorX<double> raw_t(raw_cols);
        for (int j = 0; j < raw_cols; j++) {
            raw_t(j) = j / (double) raw_freq;
        }

        MatrixX<double> new_data(rows, new_cols);

        for (int i = 0; i < rows; i++) {
            RowVectorX<double> raw_vec(raw_cols);
            for (int j = 0; j < raw_cols; j++) {
                raw_vec(j) = raw_data(i, j);
            }

            const auto fit = SplineFitting<Spline<double, 1, 3>>::Interpolate(raw_data.row(i).matrix(), 3, raw_t);
            const Spline<double, 1, 3> &spline(fit);

            for (int j = 0; j < new_cols; j++) {
                new_data(i, j) = spline((double) j / (double) new_freq).coeff(0);
            }
        }

        return new_data;
    }

    template<typename T>
    static vector<int> RowArgmax(MatrixX<T> &m) {
        vector<int> res(m.rows());

        for (int i = 0; i < m.rows(); i++) {
            int argmax = 0;
            for (int j = 1; j < m.cols(); j++) {
                if (m(i, j) > m(i, argmax)) {
                    argmax = j;
                }
            }
            res[i] = argmax;
        }

        return res;
    }

    template<typename T>
    static vector<int> ColArgMax(MatrixX<T> &m) {
        vector<int> res(m.cols());

        for (int j = 0; j < m.cols(); j++) {
            int argmax = 0;
            for (int i = 0; i < m.rows(); i++) {
                if (m(i, j) > m(argmax, j)) {
                    argmax = i;
                }
            }
            res[j] = argmax;
        }

        return res;
    }

    template<typename T>
    static void Save(const char *filename, vector<T> data) {
        ofstream output_file(filename);

        if (output_file.is_open()) {
            for (T sample: data) {
                output_file << sample << endl;
            }

            output_file.close();
            cout << "write data to file: " << filename << " successfully" << endl;

            return;
        }

        cout << "Unable to open file: " << filename << endl;
    }

    static double CalculateRMSE(const vector<double> &x, const vector<double> &y) {
        int n = (int) x.size();
        assert(y.size() == n);

        double res = 0.0;
        for (int i = 0; i < n; i++) {
            res += (x[i] - y[i]) * (x[i] - y[i]);
        }

        return sqrt(res / n);
    }
};

#endif
