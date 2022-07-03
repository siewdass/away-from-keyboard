#include "acknow.h"

Pixel Acknow::rgb( Mat img, int x, int y ) {

    Vec3b rgb = img.at < Vec3b > ( y, x );
    Pixel pixel;
    pixel.r = rgb[ 2 ];
    pixel.g = rgb[ 1 ];
    pixel.b = rgb[ 0 ];

    return pixel;
}

vector < Pixel > Acknow::inspect( Mat img ) {
    vector < Pixel > arr; 
    if ( !img.empty( ) ) {
        
        for ( int i = 0; i < img.rows; i++ ) {
            for ( int j = 0; j < img.cols; j++ ) {
                cv::Vec3b rgb = img.at < cv::Vec3b > ( i, j );
                Pixel pixel;
                pixel.r = rgb[ 2 ];
                pixel.g = rgb[ 1 ];
                pixel.b = rgb[ 0 ];

                arr.push_back( pixel );
            }
        }

    }
    return arr;
}

tuple < bool, vector < tuple < Point, bool > > > Acknow::detect( Mat img ) {

    vector < tuple < Point, bool > > arr;
    vector < int > targeting;
    Pixel pixel;
    bool target;
    int X, Y;

    try {

        if ( !img.empty( ) ) {
            
            int battlecount = 0;
            int rb = 255, gb = 0, bb = 0;
            int rmax = 0, gmax = 0, bmax = 0, count = 27;
            int width = img.cols, height = img.rows;
            int secuence = 0;

            for ( int y = 0; y <= height; y++ ) {
                for ( int x = 0; x <= width; x++ ) {

                    int top = 0, bottom = 0, middle = 0, fail = 0, check = 0;
                    int left = 0, right = 0;

                    pixel = Acknow::rgb( img, x, y );

                    if ( pixel.r == rmax & pixel.g == gmax & pixel.b == bmax ) {

                        // top
                        for ( int i = 0; i < count; i++ ) {
                            pixel = Acknow::rgb( img, x + i, y );
                            if ( pixel.r == rmax & pixel.g == gmax & pixel.b == bmax ) {
                                top += 1;
                            } else {
                                i = 100;
                            }
                        }

                        // bottom
                        if ( top == count ) {
                            for ( int i = 0; i < count; i++ ) {
                                pixel = Acknow::rgb( img, x + i, y + 3 );
                                if ( pixel.r == rmax & pixel.g == gmax & pixel.b == bmax ) {
                                    bottom += 1;
                                } else {
                                    i = 100;
                                }
                            }
                        }

                        // left
                        if ( bottom == count ) {
                            for ( int i = 1; i < 3; i++ ) {
                                pixel = Acknow::rgb( img, x, y + i );
                                if ( pixel.r == rmax & pixel.g == gmax & pixel.b == bmax ) {
                                    left += 1;
                                } else {
                                    i = 100;
                                }
                            }
                        }

                        // right
                        if ( left == 2 ) {
                            for ( int i = 1; i < 3; i++ ) {
                                pixel = Acknow::rgb( img, x + 26, y + i );
                                if ( pixel.r == rmax & pixel.g == gmax & pixel.b == bmax ) {
                                    right += 1;
                                } else {
                                    i = 100;
                                }
                            }
                        }

                        if ( top == count & bottom == count & left == 2 & right == 2 ) {

                            Point point;
                            point.x = x;
                            point.y = y;

                            target = false;
                            targeting.clear( );

                            X = x - 7, Y = y + 45;

                            for ( int i = 0; i <= 3; i++ ) {
                                for ( int j = 0; j <= 42; j++ ) {
                                    pixel = Acknow::rgb( img, X + j, Y + i );
                                    if ( pixel.r == 255 & pixel.g == 0 && pixel.b == 0 ||
                                         pixel.r == 255 & pixel.g == 128 & pixel.b == 128 ) {
                                        targeting.push_back( 1 );
                                    }
                                }
                            }

                            X = x + 32, Y = y + 6;

                            for ( int i = 0; i <= 42; i++ ) {
                                for ( int j = 0; j <= 3; j++ ) {
                                    pixel = Acknow::rgb( img, X + j, Y + i );
                                    if ( pixel.r == 255 & pixel.g == 0 && pixel.b == 0 ||
                                         pixel.r == 255 & pixel.g == 128 & pixel.b == 128 ) {
                                        targeting.push_back( 1 );
                                    }
                                }
                            }

                            //std::cout << "target pixels:" << targeting.size( ) << std::endl;
                            
                            if ( targeting.size( ) > 10 ) {
                                target = true;
                                //cv::Rect rect( x - 9, y + 4, 46, 46 );
                                //cv::rectangle( img, rect, cv::Scalar( 0, 255, 255 ) );
                            }

							arr.push_back( make_tuple( point, target ) );
                            x += count;
                        }
                    }

                }
            }
        }

    } catch ( exception &e ) {
    }

    //string url = "/home/siewdass/images/" + to_string( count ) + ".png";
    //imwrite( url, img );
    //count += 1;

    return make_tuple( target, arr );
}

vector < Point > Acknow::flask( Mat img ) {

    vector < Point > arr;
    Pixel pixel;
    vector < tuple < int, int, int > > NORMAL {
        { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
        { 44, 45, 45 }, { 40, 40, 40 }, { 40, 40, 40 }, { 36, 36, 36 },
        { 45, 46, 46 }, { 0, 0, 24 }, { 0, 0, 24 }, { 0, 0, 24 }, { 0, 0, 24 },
        { 39, 40, 40 }, { 39, 39, 40 }, { 44, 44, 44 }, { 45, 46, 46 }, { 32, 32, 32 },
        { 40, 40, 41 }, { 34, 34, 35 }, { 24, 25, 25 }, { 2, 2, 5 }, 
        { 38, 38, 39 }, { 36, 36, 37 }, { 44, 44, 44 }, { 44, 45, 45 },
        { 57, 57, 57 }, { 62, 63, 63 }, { 63, 64, 64 }, { 87, 87, 87 }
    };
    vector < tuple < int, int, int > > STRONG {
        { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
        { 0, 0, 0 }, { 101, 100, 100 }, { 146, 146, 147 }, { 42, 42, 45 },
        { 45, 46, 46 }, { 7, 5, 3 }, { 40, 40, 41 }, { 40, 40, 41 }, { 36, 36, 36 },
        { 39, 40, 40 }, { 39, 39, 40 }, { 7, 5, 3 }, { 45, 46, 46 }, { 32, 32, 32 },
        { 40, 40, 41 }, { 34, 34, 35 }, { 24, 25, 25 }, { 66, 66, 66 },
        { 31, 26, 24 }, { 14, 14, 25 }, { 92, 91, 91 }, { 0, 0, 0 },
        { 57, 57, 57 }, { 62, 63, 63 }, { 63, 64, 64 }, { 87, 87, 87 }
    };
    vector < tuple < int, int, int > > GREAT {
        { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
        { 44, 45, 45 }, { 0, 0, 0 }, { 0, 0, 0 }, { 63, 63, 63 }, { 45, 46, 46 },
        { 36, 36, 36 }, { 40, 40, 41 }, { 40, 40, 41 }, { 36, 36, 36 },
        { 39, 40, 40 }, { 39, 39, 40 }, { 44, 44, 44 }, { 52, 52, 52 },
        { 52, 52, 52 }, { 40, 40, 41 }, { 97, 97, 97 }, { 24, 25, 25 },
        { 30, 30, 31 }, { 0, 0, 0 }, { 154, 52, 0 }, { 191, 91, 0 },
        { 44, 45, 45 }, { 57, 57, 57 }, { 62, 63, 63 }, { 0, 0, 0 }, { 87, 87, 87 }
    };

    try {

        if ( !img.empty( ) ) {

            int width = img.cols, height = img.rows;
            int total = 30;

            for ( int y = 0; y <= height; y++ ) {
                for ( int x = 0; x <= width; x++ ) {

                    int normal = 0, strong = 0, great = 0;

                    pixel = Acknow::rgb( img, x, y );
                    Pixel pixel2 = Acknow::rgb( img, x + 1, y );
                    Pixel pixel3 = Acknow::rgb( img, x + 2, y );

                    if ( pixel.r == 87 & pixel.g == 87 & pixel.b == 87 &&
                         pixel2.r == 81 & pixel2.g == 81 & pixel2.b == 81 &&
                         pixel3.r == 56 & pixel3.g == 57 & pixel3.b == 56 ) {
                        
                        for ( int i = 0; i <= NORMAL.size( ); i++ ) {
                            pixel = Acknow::rgb( img, x + i, y );
                            auto [ r, g, b ] = NORMAL[ i ];
                            if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                normal += 1;
                            } else {
                                i == NORMAL.size( );
                            }
                        }

                        for ( int i = 0; i <= STRONG.size( ); i++ ) {
                            pixel = Acknow::rgb( img, x + i, y );
                            auto [ r, g, b ] = STRONG[ i ];
                            if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                strong += 1;
                            } else {
                                i == STRONG.size( );
                            }
                        }

                        for ( int i = 0; i <= GREAT.size( ); i++ ) {
                            pixel = Acknow::rgb( img, x + i, y );
                            auto [ r, g, b ] = GREAT[ i ];
                            if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                great += 1;
                            } else {
                                i == GREAT.size( );
                            }
                        }

                        if ( normal == total || strong == total || great == total ) {
                        
                            Point point;
                            point.x = x + 15;
                            point.y = y + 15; 

                            arr.push_back( point );

                            x += total;

                        }
                    }                     
                }
            }
        }

    } catch ( exception &e ) {
    }
    
    return arr;
}