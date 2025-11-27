#ifndef MP2_LAB2_MATRIX_CSR_H
#define MP2_LAB2_MATRIX_CSR_H
#include "tmatrix.h"
template <typename T>
class CSR_M {
private:
    size_t sz_m;
    TDynamicVector<T> pMem;
    TDynamicVector<size_t> rows_Ind;
    TDynamicVector<size_t> cols_Ind;
public:
    CSR_M(size_t size_Matrix, size_t size_CSR_M=1):sz_m(size_Matrix){
        pMem=TDynamicVector<T>(size_CSR_M);
        cols_Ind=TDynamicVector<size_t>(size_CSR_M);
        rows_Ind=TDynamicVector<size_t>(size_Matrix+1);
    }
    CSR_M(const TDynamicMatrix<T>& Matrix):sz_m(Matrix.size()) {
        size_t non_zero_elem = 0;
        for (size_t i = 0; i < sz_m; i++) {
            for (size_t j = 0; j < sz_m; j++) {
                if (Matrix[i][j] != T()) non_zero_elem++;
            }
        }
        pMem=TDynamicVector<T>(non_zero_elem);
        rows_Ind=TDynamicVector<size_t>(sz_m+1);
        cols_Ind=TDynamicVector<size_t>(non_zero_elem);
        size_t ind = 0;
        for (size_t i = 0; i < sz_m; i++) {
            rows_Ind[i] = ind;
            for (size_t j = 0; j < sz_m; j++) {
                if (Matrix[i][j] != T()) {
                    pMem[ind] = Matrix[i][j];
                    cols_Ind[ind] = j;
                    ind++;
                }
            }
        }
        rows_Ind[sz_m] = ind;
    }

    // Индексация когда необходимо вычисления с 0 (Только для чтения)
    T at(size_t i, size_t j){
        if (i>=sz_m || j>=sz_m){
            throw std::out_of_range("Выход за пределы");
        }
        for (size_t k = rows_Ind[i]; k < rows_Ind[i+1]; k++) {
            if (cols_Ind[k] == j)
                return pMem[k];
        }
        return T();
    }
    T at(size_t i, size_t j)const{
        if (i>=sz_m || j>=sz_m){
            throw std::out_of_range("Выход за пределы");
        }
        for (size_t k = rows_Ind[i]; k < rows_Ind[i+1]; k++) {
            if (cols_Ind[k] == j)
                return pMem[k];
        }
        return T();
    }
    TDynamicMatrix<T> Convert_Matrix()const{
        TDynamicMatrix<T> temp(sz_m);
        for (size_t row = 0; row < sz_m; row++) {
            size_t start_ind = rows_Ind[row];
            size_t end_ind = rows_Ind[row + 1];

            for (size_t k = start_ind; k < end_ind; k++) {
                size_t col = cols_Ind[k];
                temp[row][col] = pMem[k];
            }
        }
        return temp;
    }

    CSR_M operator*(const CSR_M& C_M)const{
        if (this->sz_m!=C_M.sz_m) {
            throw std::invalid_argument("Нельзя умножать матрицы разного размера");
        }
        TDynamicMatrix<T>temp1=Convert_Matrix();
        TDynamicMatrix<T> temp2 = C_M.Convert_Matrix();
        TDynamicMatrix<T> result = temp1*temp2;
        return CSR_M(result);
    }
    friend ostream& operator<<(ostream& ostr, const CSR_M& M){
        for (size_t i = 0; i < M.sz_m; i++) {
            for (size_t j = 0; j <M.sz_m; j++ ) {
                ostr << M.at(i,j) << " ";
            }
            ostr << endl;
        }
        return ostr;
    }
};


#endif //MP2_LAB2_MATRIX_CSR_H