#pragma once

#include <iostream>
#include <stdlib.h>
class Mass {
public:
	enum status {
		BLANK,
		GOAL,
		START,
		WAYPOINT,
		WALL, // 通れない
		WATER,// 進むのが1/3に遅くなる
		ROAD,//進むのが3倍速い
	};
	enum listed {
		NONE,
		OPEN,
		CLOSED,
	};
private:
	status s_ = BLANK;
	listed listed_ = NONE;
	Point pos_;
	Mass *pParnet_ = nullptr;
	int steps_ = 0;
	double estimate_ = 0.0;
	static int getWalkCost(Mass &m) { status s = m.getStatus(); return (s == WATER) ? 3 : (s == ROAD) ? 1 / 3 : 1; }
	void calcCost(const Point target) {
		steps_ = (pParnet_ ? pParnet_->steps_ : 0) + getWalkCost(*this);
		estimate_ = 3.0*Point::distance(pos_, target);
	}

public:
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }

	void setPos(int x, int y) { pos_, set(x, y); }
	const Point &getPos() const { return pos_; };
	int x() { return pos_.x(); };
	int y() { return pos_.y(); };

	void setParnet(Mass *pParnet, const Point&goal) { pParnet_ = pParnet_; calcCost(goal); }
	Mass *getParnet() { return pParnet_; }

	void setlisted(listed t) { listed_ = t; }
	bool isListed(listed t) const { return listed_ == t; }

	double getCost() const { return (double)steps_ + estimate_; }
};

class Point {
	int x_ = -1,
		y_ = -1;
public:
	Point(int x, int y) :x_(x), y_(y) {}
	int x() const { return x_; }
	int y() const { return y_; }
	void setX(int x) { x_ = x; }
	void setY(int y) { y_ = y; }

	bool operator == (const Point& p) const {
		return p.x() == x_ && p.y() == y_;}
	bool operator != (const Point& p) const {
		return !(p == *this);}

	Point getRIght() const { return Point(x_ + 1, y_); }
	Point getLeft() const { return Point(x_ - 1, y_); }
	Point getUp() const { return Point(x_, y_ - 1); }
	Point getDown() const { return Point(x_, y_ + 1); }

	static double distance(const Point p1, const Point p2) {
		double dx = (double)p2.x() - (double)p1.x();
		double dy = (double)p2.y() - (double)p1.y();

		return sqrt(dx*dx + dy * dy);
	}
};

bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}


	class Board {
	private:
		enum {
			BOARD_SIZE = 10,
		};
		Mass mass_[BOARD_SIZE][BOARD_SIZE];
		Mass &getMass(const Point p) { return mass_[p.y()][p.x()]; }

		std:: vector<Mass*>open_list_;
	public:
		Board() {
			for (int y = 0; y < BOARD_SIZE; y++) {
				for (int x = 0; x < BOARD_SIZE; x++) {
					mass_[y][x].setStatus(Mass::BLANK);
				}
			}
			// 壁
			mass_[4][6].setStatus(Mass::WALL);
			mass_[4][5].setStatus(Mass::WALL);
			mass_[5][5].setStatus(Mass::WALL);
			mass_[6][5].setStatus(Mass::WALL);
			// 水
			for (int y = 4; y <= 7; y++) {
				for (int x = 1; x <= 4; x++) {
					mass_[y][x].setStatus(Mass::WATER);
				}
			}
			// 道
			mass_[4][1].setStatus(Mass::ROAD);
			mass_[5][1].setStatus(Mass::ROAD);
			mass_[5][2].setStatus(Mass::ROAD);
			mass_[5][3].setStatus(Mass::ROAD);
			mass_[6][1].setStatus(Mass::ROAD);
			mass_[6][2].setStatus(Mass::ROAD);
			mass_[6][3].setStatus(Mass::ROAD);
		}
		~Board() {}

		bool find(const Point& start, const Point& goal);

		void show() const;
	};
