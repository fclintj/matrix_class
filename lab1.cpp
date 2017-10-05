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


        double max(){
            double max = 0;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (loc[i][j] > max){
                        max = loc[i][j];
                    }
                }
            }
            return max;
        }

        double min(){
            double min = loc[0][0];

            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (loc[i][j] < min){
                        min = loc[i][j];
                    }
                }
            }
            return min;
        }
        
        // multiply a scalar 
        Mat operator*(double x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = x*this->loc[i][j];
                }
            }
            return mat;
        }
        
        // divide a scalar
        Mat operator/(double x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = this->loc[i][j]/x;
                }
            }
            return mat;
        }

        // multiply a scalar both directions
        friend Mat operator*(double x,const Mat& arr) {
            Mat mat(arr.m,arr.n);
            for (int i = 0; i < arr.m; i++) {
                for (int j = 0; j < arr.n; j++) {
                    mat.loc[i][j] = x*arr.loc[i][j];
                }
            }
            return mat;
        }

        // add a scalar
        Mat operator+(double x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = x+this->loc[i][j];
                }
            }
            return mat;
        }

        // add a scalar in both directions
        friend Mat operator+(double x,const Mat& arr) {
            Mat mat(arr.m,arr.n);
            for (int i = 0; i < arr.m; i++) {
                for (int j = 0; j < arr.n; j++) {
                    mat.loc[i][j] = x+arr.loc[i][j];
                }
            }
            return mat;
        }


        // add two matrices
        Mat operator+(Mat x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = x.loc[i][j]+this->loc[i][j];
                }
            }
            return mat;
        }

        // subtract a scalar
        Mat operator-(double x) const {
            Mat mat(this->m,this->n);
            for (int i = 0; i < this->m; i++) {
                for (int j = 0; j < this->n; j++) {
                    mat.loc[i][j] = this->loc[i][j]-x;
                }
            }
            return mat;
        }

        // subtract a scalar in both directions
        friend Mat operator-(double x,const Mat& arr) {
            Mat mat(arr.m,arr.n);
            for (int i = 0; i < arr.m; i++) {
                for (int j = 0; j < arr.n; j++) {
                    mat.loc[i][j] = x-arr.loc[i][j];
                }
            }
            return mat;
        }

        void print()
        {
            if (n < 15 && m < 200) 
            {
                // to change the width, change in printf statement, and make int 3 lines down +1
                printf("┌");
                for (int i = 0; i < 8*n; i++) {
                    printf(" ");
                }
                printf("┐\n");

                for (int i = 0; i < m; i++){
                    printf("│");
                    for (int j = 0; j < n; j++){ 
                        printf("%7.2f ",loc[i][j]);
                    } 
                    printf("│\n");
                }
                printf("└");
                for (int i = 0; i < 8*n; i++) {
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
                    printf("%7.2f ",loc[0][j]);
                }
                printf("...\n");

                for (int i = 1; i < height; i++){
                    printf("│");
                    for (int j = 0; j < length; j++){ 
                        printf("%7.2f ",loc[i][j]);
                    } 
                    printf("\n");
                }

                printf("\n");

                // print the last block of the matrix
                for (int i = m-height; i < m-1; i++){
                    printf("    ");
                    for (int j = n-length; j < n; j++){ 
                        printf("%7.2f ",loc[i][j]);
                    } 
                    printf("│\n");
                }

                printf(" ...");
                for (int j = n-length; j < n; j++){ 
                    printf("%7.2f ",loc[m-1][j]);
                } 
                printf("│\n");


                for (int i = 0; i < 8*length; i++) {
                    printf(" ");
                }
                printf("    ┘\n");
                printf("%d X %d\n",m,n);
            }
        }
};

Mat read_pgm_p5(std::string fname){
	unsigned char d;
    std::ifstream infile;
    std::stringstream ss;
    std::string input_line;

	infile.open(fname, std::ios::binary);

	if (!infile) {
        std::cout << "Can not open image" << std::endl;;
		exit(1);
	}

    getline(infile,input_line);

    if(input_line.compare("P5") != 0){
        std::cerr << "Image version error" << std::endl; 
        exit(1);
    }

	getline(infile,input_line);
	while(input_line[0]=='#'){
	    getline(infile,input_line);
    }

    int M,N;
    ss.str(input_line);
    ss >> N >> M;
    getline(infile,input_line);

    Mat image(M,N);

    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            d = infile.get();
            image.loc[i][j]= (int)d;
        }
    }

    return image;
}

matrix read_pgm_p2(std::string name) {
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

void write_pgm_p5(Mat image, std::string name){
    std::ofstream file(name,std::ios_base::out |std::ios_base::binary |std::ios_base::trunc);
    int maxColorValue = 255;

    file << "P5\n" << image.n << " " << image.m<< "\n" << maxColorValue << "\n";

    unsigned char *buff = new unsigned char[image.m*image.n*sizeof(unsigned char)];


    for(int i=0; i < int(image.m);++i){
        for(int j=0; j < int(image.n);++j){
            buff[(i*image.m) + j] = image.loc[i][j];
        }
    }

    file.write((char *)buff, image.n*image.m*sizeof(unsigned char)); 
    file.close();
    std::cout << "Image written to " << name << "." << std::endl;
}

void write_pgm_p2(Mat image, std::string name) {
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

void count_mat(Mat& mat) {
    int count = 0;
    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < mat.n; j++) {
            mat.loc[i][j] = ++count;
        }
    }
}
Mat add_row_border(Mat X, int n) {
    Mat new_image(X.m+2*n,X.n);

    for (int i = 0; i < X.m; i++) {
        for (int j = 0; j < X.n; j++) {
            new_image.loc[i+n][j] = X.loc[i][j]; 
        }
    }
    return new_image;
}

Mat add_col_border(Mat X, int n) {
    Mat new_image(X.m,X.n+2*n);

    for (int i = 0; i < X.m; i++) {
        for (int j = 0; j < X.n; j++) {
            new_image.loc[i][j+n] = X.loc[i][j]; 
        }
    }
    return new_image;
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
    Mat Y(X.m+H.m-1,X.n+H.n-1);
    X = add_row_border(X,H.m-1);
    X = add_col_border(X,H.n-1);
    double sum = 0;
    for (int i = 0; i < Y.m; i++) {	
        for (int j = 0; j < Y.n; j++) {
            sum = 0;
            // calculate convolution directly. Flipping matrix in process 
            for (int k = 0; k < H.m; k++) {
                for (int l = 0; l < H.n; l++) {
                    sum += H.loc[k][l] * X.loc[i+H.m-k-1][j+H.n-l-1];
                }
            }
            Y.loc[i][j] = sum;
        }
    }
    return Y;
}

Mat remove_col(Mat X, int start, int stop){
    Mat Y(X.m,X.n-(stop-start));
    for (int i = 0; i < X.m; i++) {
        for (int j = 0; j < X.n; j++) {
            if (j < start) {
                Y.loc[i][j] = X.loc[i][j];
            }         
            else if(j >= start && j < stop){
            }
            else {
                Y.loc[i][j-(stop-start)] = X.loc[i][j];
            }
        }
    }  
    return Y;
}

Mat remove_row(Mat X, int start, int stop){

    Mat Y(X.m-(stop-start),X.n);

    for (int i = 0; i < X.m; i++) {
        for (int j = 0; j < X.n; j++) {
            if (i < start) {
                Y.loc[i][j] = X.loc[i][j];
            }         
            else if(i >= start && i < stop){
            }
            else {
                Y.loc[i-(stop-start)][j] = X.loc[i][j];
            }
        }
    }  
    return Y;
}

Mat reduce_dimension(Mat X, int rows, int cols){
    int remove_rows = (X.m - rows);
    int remove_cols= (X.n- cols);

    X = remove_col(X,0,remove_cols/2);
    remove_cols= (X.n- cols);
    X = remove_col(X,X.n-remove_cols,X.n);

    X = remove_row(X,0,remove_rows/2);
    remove_rows= (X.m- rows);
    X = remove_row(X,X.m-remove_rows,X.m);
    return X;
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

Mat conv2d_aspects(Mat X, Mat H){
    Mat Y = conv2d(X,H); 
    Y = reduce_dimension(Y,X.m,X.n);
    return Y;
}
Mat make_S_1(){
    Mat mat(3,3);

    double data[9] = 
    { 1,0,-1,
      2,0,-2,
      1,0,-1 };

    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < mat.n; j++) {
            mat.loc[i][j] = data[i*mat.m+j];
        }
    }

    return mat;
}

Mat make_S_2(){
    Mat mat(3,3);

    double data[9] = 
    { -1,-2,-1,
       0, 0, 0,
       1, 2, 1  };

    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < mat.n; j++) {
            mat.loc[i][j] = data[i*mat.m+j];
        }
    }

    return mat;
}

void problem2(){
    Mat F, H_1,Y;
    F = read_pgm_p5("../source/image.pgm");

    H_1 = make_H_1();

    Y = conv2d_aspects(F, H_1);
    Y = threshold_mat(Y,0,255);
    write_pgm_p5(Y,"../report/media/problem2.pgm");
}


Mat abs(Mat X){
    Mat Y = X;
    for (int i = 0; i < X.m; i++) {
        for (int j = 0; j < X.n; j++) {
            if(X.loc[i][j] < 0) {
                Y.loc[i][j] = X.loc[i][j]*-1;
            }
        }
    }
    return Y;
}

void problem3(){
    Mat F, S_1, S_2, G_1, G_2, Y, Y_v, Y_h;
    F = read_pgm_p5("../source/image.pgm");

    S_1 = make_S_1();
    S_2 = make_S_2();

    // vertical convolution
    Y_v = conv2d_aspects(F,S_1);
    Y_v = threshold_mat(Y_v,0,255);
    // horizontal convolution
    Y_h = conv2d_aspects(F,S_2);
    Y_h = threshold_mat(Y_h,0,255);


    G_1 = conv2d_aspects(F,S_1);
    G_2 = conv2d_aspects(F,S_2);
    Y = abs(G_1) + abs(G_2);
    Y = threshold_mat(Y,0,255);

    write_pgm_p5(Y_v,"../report/media/problem3_ver.pgm");
    write_pgm_p5(Y_h,"../report/media/problem3_hor.pgm");
    write_pgm_p5(Y,"../report/media/problem3.pgm");
}

void problem4(){
    Mat F, H, Y;
    
    F = read_pgm_p5("../source/image.pgm");
    H = read_pgm_p5("../report/media/filter_final.pgm");

    H = H-H.min();
    Y = conv2d_aspects(F,H);
    
    Y = Y*255.0/Y.max(); 
    Y = threshold_mat(Y,0,255);

    // write_pgm_p5(Y,"../report/media/problem4.pgm");
}

Mat fill_mat(double data[],int m,int n){
    Mat Y(m,n); 
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            Y.loc[i][j] = data[i*n+j];
        }
    }

    return Y;
}



int main() {
    // problem2();
    // problem3();
    problem4();

    // Mat F,Y,H;
    // double data[9] = 
    //     { 1,0,-1,
    //       2,0,-2,
    //       1,0,-1 };
    //
    // H = fill_mat(data,3,3);
    // H.print();
}
