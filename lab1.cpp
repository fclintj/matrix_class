#include <iostream>
#include <fstream>
#include <sstream> 
#include <armadillo>

arma::mat read_pgm_p5(std::string fname){
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

    
    arma::mat fimage(M,N);

    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            d = infile.get();
            fimage(i,j)= (int)d;
        }
    }

    return fimage;
}

void write_pgm_p5(arma::mat image, std::string name){
    std::ofstream file(name,std::ios_base::out |std::ios_base::binary |std::ios_base::trunc);
    int maxColorValue = 255;

    file << "P5\n" << image.n_cols << " " << image.n_rows<< "\n" << maxColorValue << "\n";

    unsigned char *buff = new unsigned char[image.n_rows*image.n_cols*sizeof(unsigned char)];


    for(int i=0; i < int(image.n_rows);++i){
        for(int j=0; j < int(image.n_cols);++j){
            buff[(i*image.n_rows) + j] = image(i,j);
        }
    }

    file.write((char *)buff, image.n_cols*image.n_rows*sizeof(unsigned char)); 
    file.close();
    std::cout << "Image written to " << name << "." << std::endl;
}

void write_pgm_p2(arma::mat image, std::string name) {
    std::ofstream file;
    file.open(name);

    file << "P2" << std::endl;
    file << "# Created by Clint Ferrin" << std::endl;
    file << image.n_cols << " " << image.n_rows << std::endl;
    file << "255" << std::endl;
    
    for (unsigned long long i = 0; i < image.n_rows; i++) {
        for (unsigned long long j = 0; j < image.n_cols; j++) {
            file << int(image(i,j)) << std::endl; 
        }
    }
    file.close(); 

    std::stringstream ss;
    std::string input_line = "";
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

arma::mat conv2d(arma::mat X, arma::mat H) {
    arma::mat Y(X.n_rows+H.n_rows-1,X.n_cols+H.n_cols-1);
    X = add_row_border(X,H.n_rows-1);
    X = add_col_border(X,H.n_cols-1);
    double sum = 0;
    for (unsigned long long i = 0; i < Y.n_rows; i++) {	
        for (unsigned long long j = 0; j < Y.n_cols; j++) {
            sum = 0;
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
    int remove_rows = (X.n_rows - rows);
    int remove_cols= (X.n_cols- cols);

    X.shed_cols(0,remove_cols/2-1);
    remove_cols= (X.n_cols- cols);
    X.shed_cols(X.n_cols-remove_cols,X.n_cols-1);

    X.shed_rows(0,remove_rows/2-1);
    remove_rows= (X.n_rows- rows);
    X.shed_rows(X.n_rows-remove_rows,X.n_rows-1);
    return X;
}

arma::mat conv2d_aspects(arma::mat X, arma::mat H){
    arma::mat Y = conv2d(X,H); 
    Y = reduce_dimension(Y,X.n_rows,X.n_cols);
    return Y;
}

arma::mat make_S_1(){
    arma::mat mat(3,3);

    double data[9] = 
    { 1,0,-1,
      2,0,-2,
      1,0,-1 };

    for (unsigned long long i = 0; i < mat.n_rows; i++) {
        for (unsigned long long j = 0; j < mat.n_cols; j++) {
            mat(i,j) = data[i*mat.n_rows+j];
        }
    }

    return mat;
}

arma::mat make_S_2(){
    arma::mat mat(3,3);

    double data[9] = 
    { -1,-2,-1,
       0, 0, 0,
       1, 2, 1  };

    for (unsigned long long i = 0; i < mat.n_rows; i++) {
        for (unsigned long long j = 0; j < mat.n_cols; j++) {
            mat(i,j) = data[i*mat.n_rows+j];
        }
    }

    return mat;
}

void problem2(){
    arma::mat F, H_1,Y;
    F = read_pgm_p5("../source/image.pgm");

    H_1 = make_H_1();

    Y = conv2d_aspects(F, H_1);
    Y = threshold_mat(Y,0,255);
    write_pgm_p5(Y,"../report/media/problem2.pgm");
}

void problem3(){
    arma::mat F, S_1, S_2, G_1, G_2, Y, Y_v, Y_h;
    F = read_pgm_p5("../source/image.pgm");

    S_1 = make_S_1();
    S_2 = make_S_2();

    // vertical convolution
    Y_v = conv2d_aspects(F,S_1);
    Y_v = threshold_mat(Y_v,0,255);
    // horizontal convolution
    Y_v = conv2d_aspects(F,S_1);
    Y_h = conv2d_aspects(F,S_2);
    Y_h = threshold_mat(Y_h,0,255);


    G_1 = conv2d_aspects(F,S_1);
    G_2 = conv2d_aspects(F,S_2);
    Y = arma::abs(G_1) + arma::abs(G_2);
    Y = threshold_mat(Y,0,255);

    write_pgm_p5(Y,"../report/media/problem3.pgm");
    write_pgm_p5(Y_v,"../report/media/problem3_ver.pgm");
    write_pgm_p5(Y_h,"../report/media/problem3_hor.pgm");
}

void problem4(){
    arma::mat F, H, Y;
    long double min, max;
    
    F = read_pgm_p5("../source/image.pgm");
    H = read_pgm_p5("../report/media/filter_final.pgm");

    min = arma::min(arma::min(H));
    max = arma::max(arma::max(H));
    std::cout << min << std::endl;
    std::cout << max << std::endl;

    std::cout << H.min()  << std::endl;
    std::cout << H.max() << std::endl;

    H = H-min;

    Y = conv2d_aspects(F,H);

    max = arma::max(arma::max(Y));
    std::cout << max << std::endl;

    Y = floor(Y*255.0/max); 
    Y = threshold_mat(Y,0,255);

    write_pgm_p5(Y,"../report/media/problem4.pgm");
}

int main() {
    problem2();
    // problem3();
    // problem4();
}
    
