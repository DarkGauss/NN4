
#include "Eigen/Dense"
#include <string>
#ifndef IMAGEDATA_H
#define IMAGEDATA_H

/**
 * class ImageData
 * -------------
 * Represents a standard image dataset from assignment 3
 *
 */
class ImageData
{
//values
public:
    int rows;
    int cols;
    Eigen::MatrixXd imageData;
    bool isColor;
//constructors
public:
    ImageData();
    ~ImageData();
    void print();
    //for color
    void writeImagePpm(std::string filenamem, const Eigen::MatrixXd& input);
    //for non color
    void writeImagePgm(std::string filename, const Eigen::MatrixXd& input);
public:
//helper functions for reading in data
private:
    Eigen::MatrixXd read_std(std::string expectedType = "2356"); //reads the standard data structure for class in
};

#endif
