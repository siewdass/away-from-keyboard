#include "utilities.h"
#include <opencv2/opencv.hpp>

#include <string>

using namespace std;
using namespace cv;

class Acknow {
public:
    int count = 0;
    Pixel rgb( Mat, int, int );
    vector < Pixel > inspect( Mat );
    tuple < bool, vector < tuple < Point, bool > > > detect( Mat );
    vector < Point > flask( Mat );
};