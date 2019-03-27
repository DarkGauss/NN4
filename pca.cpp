

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
    VectorXd featureMeans = x.colwise().mean();

    printf("(size of Mean: 1 x %i)\n",int(featureMeans.cols()));
    MatrixXd centered = x.rowwise() - featureMeans;
     /*
    //computer cov
    Eigen::MatrixXf cov = centered.adjoint() * centered;
    cov = cov / (x.rows() - 1);

    //computer evalues
    SelfAdjointEigenSolver<MatrixXd> eig(cov);
    // Normalize eigenvalues to make them represent percentages.
    VectorXf normalizedEigenValues =  eig.eigenvalues() / eig.eigenvalues().sum();
    MatrixXd normalizedEigenVectors = eig.eigenvectors();

    printf("(size of EigenVectors: %d x %d)\n",int(normalizedEigenVectors.rows()),int(normalizedEigenVectors.cols()));
    printf("(size of EigenValues: 1 x %d)\n",int(normalizedEigenValues.cols()));
    debugPrint(normalizedEigenVectors,"normalizedEigenVectors");

    if (data.isColor) {
        data.writeImagePpm("z.ppm", x);
    } else {
        data.writeImagePgm("z.pgm", x);
    }
    */
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