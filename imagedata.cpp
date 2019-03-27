#include "imageData.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>      
#include <string> 

#include<iostream>
#include<exception>
#include "Eigen/Core"



ImageData::ImageData()
{
    imageData = read_std();
}
ImageData::~ImageData()
{

}

void ImageData::print()
{
    using namespace Eigen;
    IOFormat HeavyFmt(FullPrecision, 0, ", ", ";\n", "[", "]", "[", "]");
    printf("----ImageData----\n");
    printf("TrainingData(%d x %d)\n",int(imageData.rows()),int(imageData.cols()));
    std::cout << imageData.format(HeavyFmt)<<std::endl;
}


//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
static char EOS='\0';

static inline int strlen(char *s)
{
    int i;

    i = 0;
    while (*s++) i++;

    return i;
}


//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
static inline char *strdup(char *s)
{
    char *t, *result;

    result = t = new char [strlen(s)+1];
    while(*s) *t++ = *s++;
    *t=EOS;

    return result;
}


//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
static inline bool strisin(const char s, const char *t)
{
    while (*t) {
        if (*t++==s) return true;
    }
    return false;
}
//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
Eigen::MatrixXd ImageData::read_std(std::string expectedType)
{
    using namespace Eigen;
    char magic[3];               // magic number
    const int bufferSize=4096;   // buffer
    char buffer[bufferSize];
    FILE *IN = stdin;   // input file
    int newr, newc, max;               // picture parms

    if (fscanf(IN, "%2s", magic)!=1) {
        printf("ERROR(): Unable to read file magic number for file from stdin.\n");
        exit(1);
    }

    if (! (magic[0]=='P' && strisin(magic[1], expectedType.c_str()))) {
         printf("ERROR(): Wrong magic number for file on stdin.  Expecting P%c or P%c format but had magic number: \"%c%c\".\n",
                expectedType[0],
                expectedType[1],
                magic[0],
                magic[1]);
        exit(1);
    }

    // read comment lines (Warning: assumes comments come right after magic number)
    fscanf(IN, "%s", buffer);
    while (*buffer=='#') {
            fgets(buffer, bufferSize, IN);
//            printf("# %s", buffer);
            fscanf(IN, "%s", buffer);
    }

    // read picture parameters
    newc = atoi(buffer);      // number of cols
    fscanf(IN, "%d", &newr);  // number of rows
    fscanf(IN, "%d", &max);   // maximum value for pixel
    int maxc = newc;
    int maxr = newr;

    // is color?
    isColor = (magic[1]=='3') || (magic[1]=='6');
    if (isColor) newc *= 3;

    // reallocate myself
    MatrixXd m(newr, newc);

    // is ascii numbers?
    if (magic[1]=='2' || magic[1]=='3') {
        for (int r=0; r<maxr; r++) {
            for (int c=0; c<maxc; c++) {
                int tmp;
                if (fscanf(IN, "%d", &tmp)!=1) {
                    printf("ERROR(): Trying to read ascii pixel value at position (%d, %d) from file stdin but failed.\n",
                           r, c);
                    exit(1);
                }
                m(r,c) = tmp;
            }
        }
    }

    // is binary numbers?
    if (magic[1]=='5' || magic[1]=='6') {
        getc(IN);
        for (int r=0; r<maxr; r++) {
            for (int c=0; c<maxc; c++) {
                m(r,c) = getc(IN);
            }
        }
    }

    rows = newr;
    cols = newc;
    return m;

    //stores the number of rows/colloms and 
    //number of values read in
    int r, c;
    //figure out the dimenision of the array to read
    try
    {
        std::cin>>r>>c;
    }
    catch (std::ios_base::failure e)
    {
    std::cout << "Standard exception: " << e.what() << std::endl;
    }
    Eigen::MatrixXd input(r,c);
    try
    {
        for(int row = 0; row < r; row++)
        {
            for (int col = 0; col < c; col++)
            {
            double item = 0.0;
            std::cin >> item;
            input(row, col) = item;
            }
        }

    }
    catch (std::ios::failure e)
    {
        std::cout << "Standard exception: " << e.what() << std::endl;
    }
    return input;
}


//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
int byteValue(double x)
{
    int z;

    z = int(x);
    if (z<0) z = 0;
    if (z>255) z = 255;

    return z;
}

//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
void ImageData::writeImagePpm(std::string filename, const Eigen::MatrixXd& input)
{
    using namespace Eigen;
    FILE *OUT;
    int maxc = int(input.cols());
    int maxr = int(input.rows());

    if (maxc%3 != 0) {
        printf("ERROR(writeImagePpm): Number of columns %d not divisible by three but supposed to be a matrix of RGB values.\n", maxc);
        exit(1);
    }

    if (filename.length()>0) {
        OUT = fopen(filename.c_str(), "w");
        if (OUT==NULL) {
            printf("ERROR(writeImagePpm): Trying to open file \"%s\" but failed.\n", filename.c_str());
            exit(1);
        }
    }
    else {
        OUT = stdout;
    }

    fprintf(OUT, "P3\n");
    fprintf(OUT, "# Name: x\n");
    fprintf(OUT, "# 8 bit color\n");
    fprintf(OUT, "%d %d\n", maxc/3, maxr);      // NOTE: columns then rows!
    fprintf(OUT, "255\n");                      // maximum level of color channels
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc-1; c++) {
            fprintf(OUT, "%d ", byteValue(input(r,c)));
        }
        fprintf(OUT, "%d\n", byteValue(input(r,maxc-1)));
    }

    fclose(OUT);
}


//copied from class matrix library, 
// Author: Dr. Robert B. Heckendorn, University of Idaho
// Version: 3.6W
// Date: Mar 19, 2019
void ImageData::writeImagePgm(std::string filename, const Eigen::MatrixXd& input)
{
    using namespace Eigen;
    FILE *OUT;
    int maxc = int(input.cols());
    int maxr = int(input.rows());


    if (filename.length()>0) {
        OUT = fopen(filename.c_str(), "w");
        if (OUT==NULL) {
            printf("ERROR(writeImagePgm): Trying to open file \"%s\" but failed.\n", filename.c_str());
            exit(1);
        }
    }
    else {
        OUT = stdout;
    }

    fprintf(OUT, "P2\n");
    fprintf(OUT, "# Name: x\n");
    fprintf(OUT, "# 8 bit gray scale\n");
    fprintf(OUT, "%d %d\n", maxc, maxr);        // NOTE: columns then rows!
    fprintf(OUT, "255\n");                      // maximum level of gray
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc-1; c++) {
            fprintf(OUT, "%d ", byteValue(input(r,c)));
        }
        fprintf(OUT, "%d\n", byteValue(input(r,maxc-1)));
    }
    fclose(OUT);
}