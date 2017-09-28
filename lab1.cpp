#include <iostream>
#include <fstream>
#include <sstream> 
#include <armadillo>

arma::mat read_pgm(std::string name) {
    int row = 0, col = 0, numrows = 0, numcols = 0;
    std::ifstream infile(name);
    std::stringstream ss;
    std::string inputLine = "";

    // First line : version
    getline(infile,inputLine);
    if(inputLine.compare("P2") != 0) std::cerr << "Version error" << std::endl;
    // else std::cout << "Version : " << inputLine << std::endl;

    // Second line : comment
    getline(infile,inputLine);
    // std::cout << "Comment : " << inputLine << std::endl;

    // Continue with a stringstream
    ss << infile.rdbuf();
    // Third line : size
    ss >> numcols >> numrows;
    // std::cout << numrows << " rows and " << numcols << " cols" << std::endl;

    arma::mat image(numrows,numcols);

    // Following lines : data
    for(row = 0; row < numrows; ++row)
        for (col = 0; col < numcols; ++col) ss >> image(row,col);

    // // Now print the array to see the result
    // for(row = 0; row < numrows; ++row) {
    //     for(col = 0; col < numcols; ++col) {
    //         // cout << array[row][col] << " ";
    //     }
    //     // cout << endl;
    // }
    infile.close();
    return image;    
}

void write_pgm(arma::mat image, std::string name) {
    std::ofstream file;
    file.open(name);
    file << "P2" << std::endl;
    file << "# Created by Clint Ferrin" << std::endl;
    file << image.n_cols << " " << image.n_rows << std::endl;
    
    for (unsigned long long i = 0; i < image.n_rows; i++) {
        for (unsigned long long j = 0; j < image.n_cols; j++) {
            file << int(image(i,j)) << std::endl; 
        }
    }
    file.close(); 

    std::stringstream ss;
    std::string inputLine = "";
    std::cout << "Image written to " << name << "." << std::endl;
}

arma::mat make_H_1() {
    arma::mat mat(5,5);

    double data[25] = 
    {  1,2,3,2,1, 2,4,6,4,2, 3,6,9,6,3, 2,4,6,4,2, 1,2,3,2,1  };

    for (unsigned long long i = 0; i < mat.n_rows; i++) {
        for (unsigned long long j = 0; j < mat.n_cols; j++) {
            mat(i,j) = data[i*mat.n_rows+j];
        }
    }
    return 1/double(81)*mat;
}

arma::mat conv2d(arma::mat X, arma::mat H) {
    arma::mat Y(X.n_rows,X.n_cols);
    for (unsigned long long i = 0; i < X.n_rows; i++) {	
        for (unsigned long long j = 0; j < X.n_cols; j++) {
            int sum1 = 0;
            int sum2 = 0;
            
            // calculate convolution for each pixel
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    if ((i+k-1 > 0 && j+l-1 > 0) && (i+k-1 < X.n_rows && j+l-1 < X.n_cols)) {
                        sum1 += X(i+k-1,j+l) * H(k,l);
                        sum2 += X(i+k-1,j+l) * H(k,l);
                    }
                }
            }
            Y(i,j) = sqrt(sum1*sum1 + sum2*sum2);
        }
    }  
    return Y;
}

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

arma::mat threshold_mat(arma::mat mat, int min, int max){
     for (unsigned long long i = 0; i < mat.n_rows; i++) {
         for (unsigned long long j = 0; j < mat.n_cols; j++){
            if(mat(i,j) > max) {mat(i,j) = max;}
            if(mat(i,j) < min) {mat(i,j) = min;}
         }
     }
     return mat;
}

arma::mat reduce_dimension(arma::mat X, int rows, int cols){
    int remove_rows = (X.n_rows - rows)/2;
    int remove_cols= (X.n_cols- cols)/2;
    std::cout << remove_cols << std::endl;
    std::cout << remove_rows << std::endl;

    for (int i = 0; i < remove_rows; i++) {
        X.shed_col(0);
    }
    for (unsigned long long i = 0; i < X.n_rows-rows; i++) {
       X.shed_col(X.n_rows) ;
    }

    for (int i = 0; i < remove_cols; i++) {
        X.shed_row(0);
    }
    for (unsigned long long i = 0; i < X.n_cols-cols; i++) {
       X.shed_row(X.n_cols) ;
    }
    return X;
}

int main() {
    arma::mat image(read_pgm("../source/image.pgm"));
    arma::mat H(2,2);
    H = make_H_1();
    arma::mat Y = conv2d_comp(image,H);
    Y = threshold_mat(Y,0,255);
    // Y = reduce_dimension(Y,image.n_rows,image.n_cols);
    write_pgm(Y,"output.pgm");
}
