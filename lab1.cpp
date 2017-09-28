#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <algorithm>
using matrix = std::vector<std::vector<double>>;

class Mat {
    public:
        matrix loc;
        int m;
        int n; 

        Mat()
        {
            m = 1;
            n = 1;
            loc.resize( m, std::vector<double>( n, 0) );
        }

        Mat(int r, int c)
        {
            m = r;
            n = c;
            loc.resize( m, std::vector<double>( n, 0) );
        };

        Mat(matrix mat)
        {
            m = mat.size();
            n = mat[0].size();
            loc.resize( m, std::vector<double>( n, 0) );
            loc = mat;
        };

        void print()
        {
            if (n < 15 && m < 200) 
            {
                // to change the width, change in printf statement, and make int 3 lines down +1
                printf("┌");
                for (int i = 0; i < 6*n; i++) {
                    printf(" ");
                }
                printf("┐\n");

                for (int i = 0; i < m; i++){
                    printf("│");
                    for (int j = 0; j < n; j++){ 
                        printf("%5.1f ",loc[i][j]);
                    } 
                    printf("│\n");
                }
                printf("└");
                for (int i = 0; i < 6*n; i++) {
                    printf(" ");
                }
                printf("┘\n");
            }

            else 
            {
                int length = 10;
                int height = 8;
                if (n < length*2) { length = ceil(double(n)/2); }
                if (m < height) { height = m; }

                printf("┌");
                for (int i = 0; i < 6*length; i++) {
                    printf(" ");
                }
                printf("\n│");
                // print first line ...
                for (int j = 0; j < length; j++){ 
                    printf("%5.1f ",loc[0][j]);
                }
                printf("...\n");

                for (int i = 1; i < height; i++){
                    printf("│");
                    for (int j = 0; j < length; j++){ 
                        printf("%5.1f ",loc[i][j]);
                    } 
                    printf("\n");
                }

                printf("\n");

                // print the last block of the matrix
                for (int i = m-height; i < m-1; i++){
                    printf("    ");
                    for (int j = n-length; j < n; j++){ 
                        printf("%5.f ",loc[i][j]);
                    } 
                    printf("│\n");
                }

                printf(" ...");
                for (int j = n-length; j < n; j++){ 
                    printf("%5.f ",loc[m-1][j]);
                } 
                printf("│\n");


                for (int i = 0; i < 6*length; i++) {
                    printf(" ");
                }
                printf("    ┘\n");
                printf("%d X %d\n",m,n);
            }
        }

        // Overload * operator to multiply two Box objects.
        Mat operator*(double x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = x*this->loc[i][j];
                }
            }
            return mat;
        }
        friend Mat operator*(double x,const Mat& arr) {
            Mat mat(arr.m,arr.n);
            for (int i = 0; i < arr.m; i++) {
                for (int j = 0; j < arr.n; j++) {
                    mat.loc[i][j] = x*arr.loc[i][j];
                }
            }
            return mat;
        }
};

matrix read_pgm(std::string name) {
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

    matrix image;
    image.resize( numrows, std::vector<double>( numcols, 3) );


    // Following lines : data
    for(row = 0; row < numrows; ++row)
        for (col = 0; col < numcols; ++col) ss >> image[row][col];

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

void write_pgm(Mat image, std::string name) {
    std::ofstream file;
    file.open(name);
    file << "P2" << std::endl;
    file << "# Created by Clint Ferrin" << std::endl;
    file << image.n << " " << image.m << std::endl;
    
    for (int i = 0; i < image.m; i++) {
        for (int j = 0; j < image.n; j++) {
            file << int(image.loc[i][j]) << std::endl; 
        }
    }
    file.close(); 

    std::stringstream ss;
    std::string inputLine = "";
    std::cout << "Image written to " << name << "." << std::endl;
}

Mat make_H_1() {
    Mat mat(5,5);

    double data[25] = 
    {  1,2,3,2,1, 2,4,6,4,2, 3,6,9,6,3, 2,4,6,4,2, 1,2,3,2,1  };

    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < mat.n; j++) {
            mat.loc[i][j] = data[i*mat.n+j];
        }
    }
    return 1/double(81)*mat;
}

void fill_mat(Mat& mat) {
    int count = 0;
    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < mat.n; j++) {
            mat.loc[i][j] = ++count;
        }
    }
}

Mat add_border(Mat orig) {
    Mat new_image;
    new_image.m=orig.m+2;;
    new_image.n=orig.n+2;
    new_image.loc.resize( new_image.m, std::vector<double>( new_image.n, 0) );

    for (int i = 1; i < new_image.m-1; i++) {
        for (int j = 1; j < new_image.n-1; j++) {
            new_image.loc[i][j] = orig.loc[i-1][j-1]; 
        }
    }
    return new_image;
}

Mat remove_border(Mat orig) {
    Mat new_image;
    new_image.m=orig.m-2;;
    new_image.n=orig.n-2;
    new_image.loc.resize( new_image.m, std::vector<double>( new_image.n, 0) );

    for (int i = 0; i < new_image.m; i++) {
        for (int j = 0; j < new_image.n; j++) {
            new_image.loc[i][j] = orig.loc[i+1][j+1]; 
        }
    }
    return new_image;
}

Mat conv2d(Mat X, Mat H) {
    Mat Y(X.m,X.n);
    for (int i = 0; i < X.m; i++) {	
        for (int j = 0; j < X.n; j++) {
            int sum1 = 0;
            int sum2 = 0;
            
            // calculate convolution for each pixel
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    if ((i+k-1 > 0 && j+l-1 > 0) && (i+k-1 < X.m && j+l-1 < X.n)) {
                        sum1 += X.loc[i+k-1][j+l] * H.loc[k][l];
                        sum2 += X.loc[i+k-1][j+l] * H.loc[k][l];
                    }
                }
            }
            Y.loc[i][j] = sqrt(sum1*sum1 + sum2*sum2);
        }
    }  
    return Y;
}

Mat conv2d_comp(Mat X, Mat H) {
    Mat Y(X.m+H.m-1,X.n+H.n-1);
    X = add_border(X);

    for (int i = 0; i < X.m-1; i++) {	
        for (int j = 0; j < X.n-1; j++) {
            int sum = 0;
            
            // calculate convolution for each pixel
            for (int k = 0; k < H.m; k++) {
                for (int l = 0; l < H.n; l++) {
                        sum += H.loc[k][l] * X.loc[i+H.m-k-1][j+H.n-l-1];
                    }
                }
            Y.loc[i][j] = sum;
            }
        }
    Y.print();
    return Y;
}
Mat threshold_mat(Mat mat,int min,int max){
     for (int i = 0; i < mat.m; i++) {
         for (int j = 0; j < mat.n; j++){
            if(mat.loc[i][j] > max) {mat.loc[i][j] = max;}
            if(mat.loc[i][j] < min) {mat.loc[i][j] = min;}
         }
     }
     return mat;
}

int main() {
    Mat image(read_pgm("../source/image2.pgm"));
    Mat H(2,2);
    Mat X(3,3);
    fill_mat(X);
    fill_mat(H);
    // Mat H_1 = make_H_1();
    Mat Y = conv2d_comp(X,H);
    // Y = threshold_mat(Y,0,255);
    // Y.print();
    // write_pgm(Y,"output.pgm");
}
