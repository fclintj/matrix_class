#include <iostream>
#include <armadillo>

using namespace std;

arma::mat add_row_border(arma::mat X, int n) {
    for (int i = 0; i < n; i++) {
        arma::mat Z_row(1,X.n_cols); Z_row.fill(0); 
        X = arma::join_cols(X,Z_row);
        X = arma::join_cols(Z_row,X);
    }
    return X;
}

arma::mat add_col_border(arma::mat X, int n) {
    for (int i = 0; i < n; i++) {
        arma::mat Z_col(X.n_rows,1); Z_col.fill(0);
        X = arma::join_rows(X,Z_col);
        X = arma::join_rows(Z_col,X);
    }
    return X;
}

arma::mat conv2d_comp(arma::mat X, arma::mat H) {
    arma::mat Y(X.n_rows+H.n_rows-1,X.n_cols+H.n_cols-1);
    X = add_row_border(X,H.n_rows-1);
    X = add_col_border(X,H.n_cols-1);
    for (unsigned long long i = 0; i < Y.n_rows; i++) {	
        for (unsigned long long j = 0; j < Y.n_cols; j++) {
            int sum = 0;
            // calculate convolution directly. Flipping matrix in process 
            for (unsigned long long k = 0; k < H.n_rows; k++) {
                for (unsigned long long l = 0; l < H.n_cols; l++) {
                    sum += H(k,l) * X(i+H.n_rows-k-1,j+H.n_cols-l-1);
                }
            }
            Y(i,j) = sum;
        }
    }
    return Y;
}

int main() {
    arma::mat A(6,3);   // directly specify the matrix size (elements are uninitialized)
    A.fill(1);
    // A   << 1  << 3 << 3 << arma::endr
    //     << 4  << 6 << 6 << arma::endr;

    arma::mat X = conv2d_comp(A,A);
    X.print();
    X.shed_col(X.n_cols-1);
    std::cout << std::endl;
    X.print();
    // // determinant
    // cout << "det(A): " << det(A) << endl;
    //
    // // inverse
    // cout << "inv(A): " << endl << inv(A) << endl;
    //
    // // save matrix as a text file
    // A.save("A.txt", raw_ascii);
    //
    // // load from file
    // mat B;
    // B.load("A.txt");
    //
    // // submatrices
    // cout << "B( span(0,2), span(3,4) ):" << endl << B( span(0,2), span(3,4) ) << endl;
    //
    // cout << "B( 0,3, size(3,2) ):" << endl << B( 0,3, size(3,2) ) << endl;
    //
    // cout << "B.row(0): " << endl << B.row(0) << endl;
    //
    // cout << "B.col(1): " << endl << B.col(1) << endl;
    //
    // // transpose
    // cout << "B.t(): " << endl << B.t() << endl;
    //
    // // maximum from each column (traverse along rows)
    // cout << "max(B): " << endl << max(B) << endl;
    //
    // // maximum from each row (traverse along columns)
    // cout << "max(B,1): " << endl << max(B,1) << endl;
    //
    // // maximum value in B
    // cout << "max(max(B)) = " << max(max(B)) << endl;
    //
    // // sum of each column (traverse along rows)
    // cout << "sum(B): " << endl << sum(B) << endl;
    //
    // // sum of each row (traverse along columns)
    // cout << "sum(B,1) =" << endl << sum(B,1) << endl;
    //
    // // sum of all elements
    // cout << "accu(B): " << accu(B) << endl;
    //
    // // trace = sum along diagonal
    // cout << "trace(B): " << trace(B) << endl;
    //
    // // generate the identity matrix
    // mat C = eye<mat>(4,4);
    //
    // // random matrix with values uniformly distributed in the [0,1] interval
    // mat D = randu<mat>(4,4);
    // D.print("D:");
    //
    // // row vectors are treated like a matrix with one row
    // rowvec r;
    // r << 0.59119 << 0.77321 << 0.60275 << 0.35887 << 0.51683;
    // r.print("r:");
    //
    // // column vectors are treated like a matrix with one column
    // vec q;
    // q << 0.14333 << 0.59478 << 0.14481 << 0.58558 << 0.60809;
    // q.print("q:");
    //
    // // convert matrix to vector; data in matrices is stored column-by-column
    // vec v = vectorise(A);
    // v.print("v:");
    //
    // // dot or inner product
    // cout << "as_scalar(r*q): " << as_scalar(r*q) << endl;
    //
    // // outer product
    // cout << "q*r: " << endl << q*r << endl;
    //
    // // multiply-and-accumulate operation (no temporary matrices are created)
    // cout << "accu(A % B) = " << accu(A % B) << endl;
    //
    // // example of a compound operation
    // B += 2.0 * A.t();
    // B.print("B:");
    //
    // // imat specifies an integer matrix
    // imat AA;
    // imat BB;
    //
    // AA << 1 << 2 << 3 << endr << 4 << 5 << 6 << endr << 7 << 8 << 9;
    // BB << 3 << 2 << 1 << endr << 6 << 5 << 4 << endr << 9 << 8 << 7;
    //
    // // comparison of matrices (element-wise); output of a relational operator is a umat
    // umat ZZ = (AA >= BB);
    // ZZ.print("ZZ:");
    //
    // // cubes ("3D matrices")
    // cube Q( B.n_rows, B.n_cols, 2 );
    //
    // Q.slice(0) = B;
    // Q.slice(1) = 2.0 * B;
    //
    // Q.print("Q:");
    //
    // // 2D field of matrices; 3D fields are also supported
    // field<mat> F(4,3); 
    //
    // for(uword col=0; col < F.n_cols; ++col)
    // for(uword row=0; row < F.n_rows; ++row)
    //   {
    //   F(row,col) = randu<mat>(2,3);  // each element in field<mat> is a matrix
    //   }
    //
    // F.print("F:");

    return 0;
}

