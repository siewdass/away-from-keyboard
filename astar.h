#pragma once

#include <queue>
#include <vector>
#include <functional>
#include <opencv2/highgui.hpp>

using namespace cv;
#define Array std::vector

namespace pf
{
	class Vec2i {
	public:
		inline Vec2i(int x, int y) : x(x), y(y) {}
		inline Vec2i() = default;

		inline bool operator== (const Vec2i& v) const { return (x == v.x) && (y == v.y); }
		inline const Vec2i operator+ (const Vec2i& v) const { return Vec2i(x + v.x, y + v.y); }

		static Vec2i getDelta(const Vec2i& v1, const Vec2i& v2) { return Vec2i(abs(v1.x - v2.x), abs(v1.y - v2.y)); }

		int x;
		int y;
	};

	using uint = unsigned int;
	using HeuristicFunction = std::function<uint(const Vec2i&, const Vec2i&, int)>;

	struct Node
	{
		Node() : pos(0, 0), parent(-1, -1), f(0), g(0), h(0) {}
		Node(const Vec2i& pos, uint f) : pos(pos), parent(-1, 1), f(f), g(0), h(0) {}
		Node(const Vec2i& pos, const Vec2i& parent, uint f, uint g, uint h) : pos(pos), parent(parent), f(f), g(g), h(h) {}

		Vec2i pos;
		Vec2i parent;
		uint f;
		uint g;
		uint h;
	};

	// Reverse std::priority_queue to get the smallest element on top
	inline bool operator< (const Node& a, const Node& b) { return a.f < b.f; }

	class Astar
	{
	public:
		Astar();

		Array < cv::Point > findPath(cv::Point sP, cv::Point tP );
		void loadMap( Array < Array < int > > );
		void setDiagonalMovement(bool enable);

	private:
		Array < cv::Point > buildPath() const;
		bool isValid(const Vec2i& pos) const;
		bool isBlocked(int index) const;
		int convertTo1D(const Vec2i& pos) const;

		int m_weight;
		int m_size;
		uint m_nrOfDirections;
		Vec2i m_dimensions;
		Vec2i m_startPos;
		Vec2i m_targetPos;
		std::priority_queue<Node> m_openList;
		Array <bool> m_closedList;
		Array <Node> m_cameFrom;
		Array <int> m_grid;
		Array <Vec2i> m_directions;
		HeuristicFunction m_heuristic;
	};

	namespace heuristic
	{
		uint manhattan(const Vec2i& v1, const Vec2i& v2, int weight);
		uint euclidean(const Vec2i& v1, const Vec2i& v2, int weight);
	}
}
