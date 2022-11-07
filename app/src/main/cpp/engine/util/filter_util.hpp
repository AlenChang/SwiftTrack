#ifndef SONAR_FILTER_UTIL_HPP
#define SONAR_FILTER_UTIL_HPP

#include "matrix_util.hpp"

class FilterUtil {
public:
    enum filter_type {
        LOW_PASS = 1,
        HIGH_PASS = 2,
        BAND_PASS = 3
    };

    enum filter_method {
        MATLAB_DIY = 1,
        BUTTERWORTH = 2,
        CHEBYSHEV_I = 3,
        CHEBYSHEV_II = 4,
        RBJ = 5
    };

    static void Filter(MatrixX<complex<double>> &dst, const MatrixX<complex<double>> &src, double fs = 48000,
                       const vector<double> &fc = {3000},
                       filter_type type = LOW_PASS, filter_method method = MATLAB_DIY) {
        int rows = (int) src.rows(), cols = (int) src.cols();
        assert(dst.rows() == rows && dst.cols() == cols);

        if (type == LOW_PASS && method == MATLAB_DIY) {
            for (int i = 0; i < rows; i++) {
                vector<complex<double>> src_row = MatrixUtil::GetRow(src, i);
                vector<complex<double>> dst_row(cols);
                MatlabDIYFilter(dst_row, src_row);
                MatrixUtil::SetRow(dst, i, dst_row);
            }

            return;
        }

        cout << "Unsupported filter type = " << type << ", method = " << method << endl;
        assert(false);
    }

    static void Filter(vector<complex<double>> &dst, const vector<complex<double>> &src, double fs = 48000,
                       const vector<double> &fc = {3000},
                       filter_type type = LOW_PASS, filter_method method = MATLAB_DIY
    ) {
        assert(dst.size() == src.size());

        if (type == LOW_PASS && method == MATLAB_DIY) {
            MatlabDIYFilter(dst, src);
            return;
        }

        cout << "Unsupported filter type = " << type << ", method = " << method << endl;
        assert(false);
    }

private:
    /**
     * https://ww2.mathworks.cn/matlabcentral/answers/9900-use-filter-constants-to-hard-code-filter#answer_13623
     *
     * Matlab code (my_filter.m):
     *
     * function y = my_filter(b, a, x)
     *
     * n = length(a);
     * z(n) = 0;
     * b = b / a(1);
     * a = a / a(1);
     * y = zeros(size(x));
     *
     * for m = 1 : length(y)
     *     y(m) = b(1) * x(m) + z(1);
     *     for i = 2 : n
     *         z(i - 1) = b(i) * x(m) + z(i) - a(i) * y(m);
     *     end
     * end
     */
    static void MatlabDIYFilter(vector<complex<double>> &dst, const vector<complex<double>> &src) {
        const int NUM_COEFFICIENTS = 7;
        const double COEFFICIENT_A[NUM_COEFFICIENTS] = {
                1.0,
                -4.484563008434193,
                8.529005084031876,
                -8.779107970620451,
                5.147642681385829,
                -1.627714784894886,
                0.216582855616241
        };
        const double COEFFICIENT_B[NUM_COEFFICIENTS] = {
                0.0288258919440031e-3,
                0.1729553516640186e-3,
                0.4323883791600464e-3,
                0.5765178388800619e-3,
                0.4323883791600464e-3,
                0.1729553516640186e-3,
                0.0288258919440031e-3
        };

        int n = (int) src.size();
        for (int i = 0; i < n; i++) {
            dst[i] = complex<double>(0, 0);
        }
        vector<complex<double>> buffer(NUM_COEFFICIENTS, complex<double>(0, 0));

        for (int i = 0; i < n; i++) {
            dst[i] = COEFFICIENT_B[0] * src[i] + buffer[0];
            for (int j = 1; j < NUM_COEFFICIENTS; j++) {
                buffer[j - 1] = COEFFICIENT_B[j] * src[i] + buffer[j] - COEFFICIENT_A[j] * dst[i];
            }
        }
    }
};

#endif
