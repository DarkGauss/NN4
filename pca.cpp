

#include <cstdlib> 
#include <iostream>
#include <cmath>
#include "Eigen/Dense"
#include "imagedata.h"
#include "utility.h"


using namespace Eigen;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage: enter the number of eigen values to be kept. k < 0 means transpose will happen\n" << endl;
        exit(1);
    
    }
    
    //stores the number of eigenvalues to be kept
    int k = stoi(argv[1]);
    //stores wheather to transpose the matrix
    bool transpose = k < 0;
    k = abs(k);

    //load image data
    ImageData data;
    MatrixXd x = data.imageData;
    printf("(size of Image: %i x %i)\n", int(x.rows()), int(x.cols()));
    if(transpose) x.transposeInPlace();

    // Mean centering data.
    VectorXd xMeans = x.rowwise().mean();
    printf("(size of Mean: 1 x %i)\n", int(xMeans.size()));

    
    MatrixXd centered = x.colwise() - xMeans;

    MatrixXd cov = centered.transpose() * centered;
    cov = cov.array()/int(x.rows()); 

    SelfAdjointEigenSolver<MatrixXd> es(cov);
    VectorXd e_values = es.eigenvalues();
    MatrixXd e_vectors = es.eigenvectors();

    e_values.reverseInPlace();
    MatrixXd row_e_vectors =e_vectors.rowwise().reverse().transpose();
    VectorXd row_e_values = e_values.head(k);
    printf("(size of EigenVectors: %i x %i)\n", int(row_e_vectors.rows()), int(row_e_vectors.cols()));
    printf("(size of EigenValues: 1 x %i)\n", int(row_e_vectors.rows()));

    MatrixXd V;
    V = row_e_vectors.topRows(k);
 
    MatrixXd encoded = centered * V.transpose();
    printf("(size of Encoded: %i x %i)\n", int(encoded.rows()), int(encoded.cols()));

    MatrixXd decoded_partial = encoded * V;

    MatrixXd decoded = decoded_partial.colwise() + xMeans;

    if(transpose) decoded.transposeInPlace();
    if(transpose) x.transposeInPlace();

    printf("(size of Decoded: %i x %i)\n", int(decoded.rows()), int(decoded.cols()));

    //calculate the error
    MatrixXd Error = x.array() - decoded.array(); 
    Error = Error.cwiseProduct(Error).eval();
    double error = Error.sum() / (double(x.cols())*double(x.rows()));
    printf("Per Pixel Dist^2: %f",error);

    if(data.isColor) {
        data.writeImagePpm("z.ppm", decoded);
    } else {
        data.writeImagePgm("z.pgm", decoded);
    }

   return 0;
}

/* EXAMPlE OUTPUT
 pca 160 < girlWithPearlEarringJohansson.ppm
 (size of Pic: 540 X 1464)
 (size of Mean: 1 X 1464)
 (size of EigenVectors: 1464 X 1464)
 (size of EigenValues: 1 X 1464)
 (size of Encoded: 540 X 160)
 (size of Decoded: 540 X 1464)
 Per Pixel Dist^2: 1.05186
 */