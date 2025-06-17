#pragma once
#include "util.h"
#include "pathfinder.h"

class Pathfinder {
public:
    void loadMap(const std::vector<std::vector<int>>& map) {
        m_height = map.size();
        if (m_height == 0) return;
        m_width = map[0].size();
        m_grid.clear();
        
        for (const auto& row : map) {
            m_grid.insert(m_grid.end(), row.begin(), row.end());
        }
    }

    std::vector<Point> findPath(Point start, Point target) {
        if (m_grid.empty()) return {};
        
        auto cmp = [](const std::pair<int, PathNode>& left, 
                     const std::pair<int, PathNode>& right) {
            return left.first > right.first;
        };
        std::priority_queue<std::pair<int, PathNode>, 
                          std::vector<std::pair<int, PathNode>>,
                          decltype(cmp)> open(cmp);
        
        std::vector<bool> closed(m_grid.size(), false);
        std::vector<Point> cameFrom(m_grid.size(), Point(-1,-1));

        open.emplace(0, PathNode(start, Point(-1,-1), 0));
        cameFrom[start.y * m_width + start.x] = start;

        const Point directions[] = {{-1,0}, {1,0}, {0,1}, {0,-1}};

        while (!open.empty()) {
            PathNode current = open.top().second;
            open.pop();

            if (current.pos == target) break;
            if (closed[current.pos.y * m_width + current.pos.x]) continue;
            closed[current.pos.y * m_width + current.pos.x] = true;

            for (const Point& dir : directions) {
                Point neighbor = current.pos + dir;
                if (!isValid(neighbor)) continue;
                
                int idx = neighbor.y * m_width + neighbor.x;
                if (m_grid[idx] == 0 || closed[idx]) continue;

                int new_g = current.g + 1;
                int h = (int)norm(target - neighbor);
                int f = new_g + h;

                if (cameFrom[idx] == Point(-1,-1) || new_g < cameFrom[idx].x) {
                    open.emplace(f, PathNode(neighbor, current.pos, new_g));
                    cameFrom[idx] = current.pos;
                }
            }
        }

        return buildPath(target, cameFrom);
    }

    bool has3x3Space(Point p) const {
        for(int dx = -1; dx <= 1; ++dx) {
            for(int dy = -1; dy <= 1; ++dy) {
                Point neighbor(p.x + dx, p.y + dy);
                if(!isValid(neighbor) || m_grid[neighbor.y * m_width + neighbor.x] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    std::vector<int> m_grid;
    int m_width = 0;
    int m_height = 0;

    bool isValid(Point p) const {
        return p.x >= 0 && p.x < m_width && p.y >= 0 && p.y < m_height;
    }

    std::vector<Point> buildPath(Point target, const std::vector<Point>& cameFrom) const {
        std::vector<Point> path;
        Point current = target;
        
        while (current != Point(-1,-1) && cameFrom[current.y * m_width + current.x] != current) {
            path.push_back(current);
            current = cameFrom[current.y * m_width + current.x];
            if (current == Point(-1,-1)) return {};
        }
        
        std::reverse(path.begin(), path.end());
        return path;
    }
};