#ifndef LOCATION_H
#define LOCATION_H

#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "util.h"
#include <tuple>

using namespace std;
using namespace cv;

class Location {
public:
    vector<Mat> maps;
    vector<vector<vector<int>>> paths;
    
    Location() {}
    
    void load() {
        Mat img;

        for (int i = 0; i <= 15; i++) {
            img = imread("/opt/afk/maps/" + to_string(i) + "-map.png");
            maps.push_back(img);
            img = imread("/opt/afk/maps/" + to_string(i) + "-path.png");

            vector<vector<int>> tmp;
            for (int x = 0; x < img.rows; x++) {
                vector<int> row;
                for (int y = 0; y < img.cols; y++) {
                    row.push_back(img.at<Vec3b>(x, y) == Vec3b(255, 0, 255) ? 0 : 1);
                }
                tmp.push_back(row);
            }
            paths.push_back(tmp);
        }
    }

    void walkable(vector<Waypoint> walkable) {
        for (Waypoint wp : walkable) paths[wp.z][wp.y][wp.x] = 0;
    }

    tuple<int, int, int, int> update(Mat FloorIndex, Mat Minimap, int x, int y) {
        int z = 0, valid = -1;

        try {
            for (int i = 0; i < FloorIndex.rows; i+=4) {
                if (FloorIndex.at<Vec3b>(i, 0) == Vec3b(38, 38, 37)) break;
                z++;
            }

            if (x == 0 && y == 0) {
                for (int i = 16; i < maps[z].rows - 16; i++) {
                    for (int j = 16; j < maps[z].cols - 16; j++) {
                        if (compare(j, i, z, Minimap)) {
                            x = j, y = i;
                        }
                    }
                }
            } else {
                auto [X, Y] = find(x, y, z, Minimap);
                x = X;
                y = Y;
            }

            for (int i = y - 1; i < y + 2; ++i) {
                for (int j = x - 1; j < x + 2; ++j) {
                    if (paths[z][i][j] == 1) valid++;
                }
            }

        } catch (exception& e) {
            console.log(e.what());
        }
        
        return make_tuple(x, y, z, valid);
    }

    tuple<int, int> find(int x, int y, int z, Mat minimap) {
        int oldX = x, oldY = y; 
        try {
            if (compare(x, y, z, minimap)) return make_tuple(x, y);

            int X, Y, grow = 0;

            while (grow < 150) {
                grow += 2;
                x--;
                y--;

                X = x;
                while (x < maps[z].rows && x < X + grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    x++;
                }

                Y = y;
                while (y < maps[z].cols && y < Y + grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    y++;
                }

                X = x;
                while (x >= 0 && x > X - grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    x--;
                }

                Y = y;
                while (y >= 0 && y > Y - grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    y--;
                }
            }
        
        } catch (exception &e) {
        }

        return make_tuple(oldX, oldY);
    }

    bool compare(int x, int y, int z, Mat minimap) {
        try {
            if (paths[z][y][x] == 0 && x >= 16 && x <= (2559 - 16) && y >= 16 && y <= (2559 - 16)) return false;

            Mat part = Mat(maps[z], Rect(x - 16, y - 16, 34, 34));

            int threshold = 0;

            for (int i = 0; i < minimap.rows - 1; i++) {
                for (int j = 0; j < minimap.cols - 1; j++) {
                    if (minimap.at<Vec3b>(i, j) == Vec3b(255, 255, 255)) continue;
                    if (threshold > 20) return false;
                    if (minimap.at<Vec3b>(i, j) != part.at<Vec3b>(i, j)) threshold++;
                }
            }

            return true;
        } catch (exception &e) {
        }

        return false;
    }
};

#endif