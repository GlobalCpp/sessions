#include <array>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <vector>
#include <cmath>

#include "point.h"

using PathLength = double;
static const int MAX_ELEMS = 32;

template<typename T>
struct QuickStack
{
	std::array<T, MAX_ELEMS> vals_;
	size_t sz_ = 0;
	void clear() { sz_ = 0; }
	void push(T const& v) { vals_[sz_] = v; sz_++; }
	void pop() {sz_--;}
	T const& bottom() const { return vals_[0]; }
	T const& top() const { return vals_[sz_ - 1]; }
	size_t size() const noexcept { return sz_; }
	bool empty() const noexcept { return size() == 0; }
};

inline std::ostream& operator<<(std::ostream& out, const QuickStack<point>& s)
{
	QuickStack<point> tmpStack=s;
	while (tmpStack.size())
	{
		out << tmpStack.top().pointNum << " ";
		tmpStack.pop();
	}

return out;
}

template<typename T>
struct QuickQueue
{
	std::array<T, MAX_ELEMS> vals_;
	size_t front_ = 0;
	size_t back_ = 0;
	void clear() { front_ = back_ = 0; }
	bool empty() const noexcept { return front_ == back_; }
	std::vector<T> toVector() const
	{
		std::vector<T> res;
		if (back_ < front_)
		{
			std::copy(
				vals_.begin() + front_,
				vals_.end(),
				std::back_inserter(res));
			std::copy(
				vals_.begin(),
				vals_.begin() + back_,
				std::back_inserter(res));
		}
		else
		{
			std::copy(
				vals_.begin() + front_,
				vals_.begin() + back_,
				std::back_inserter(res));
		}
		return res;
	}

	void push(T const& v)
	{
		vals_[back_] = v;
		advance_back();
	}
	void advance_back()
	{
		++back_;
		if (back_ >= MAX_ELEMS) back_ = 0;
	}
	void advance_front()
	{
		++front_;
		if (front_ >= MAX_ELEMS) front_ = 0;
	}
	void pop() { advance_front(); }

	T const& front() const { return vals_[front_]; }

	size_t size() const noexcept
	{
		if (back_ < front_)
			return (MAX_ELEMS - front_) + back_;
		return back_ - front_;
	}
};

struct DistanceTable
{
private:
	std::vector<point> cityList_;
	std::array<PathLength, MAX_ELEMS*MAX_ELEMS> distances_;
	std::array<PathLength, MAX_ELEMS> minEdge_;
	PathLength hypoMin_ = 0;

public:
	PathLength getDistance(int firstCity, int secondCity) const noexcept
	{ return distances_[firstCity * MAX_ELEMS + secondCity]; }

	PathLength getMinEdge(int city) const
	{ return minEdge_[city]; }

	PathLength findDistance(point point1, point point2)
	{
		return sqrt((PathLength)((point1.x - point2.x) * (point1.x - point2.x) + 
						(point1.y - point2.y) * (point1.y - point2.y)));
	}

	PathLength hypoMin() const noexcept { return hypoMin_; }

	DistanceTable(std::vector<point> const& cityList)
		: cityList_(cityList)
	{
		std::fill_n(minEdge_.begin(), cityList_.size(), std::numeric_limits<PathLength>::max());
		for(int i=0; i<cityList_.size(); i++)
		{
			 for(int j=0; j<cityList_.size(); j++)
			 {
				auto dist = findDistance(cityList_[i], cityList[j]);
				distances_[i * MAX_ELEMS + j] = dist;
				if (i != j)
					minEdge_[i] = std::min(minEdge_[i], dist);
			 }
		}
		hypoMin_ = std::accumulate(minEdge_.begin(), minEdge_.begin() + cityList_.size(), PathLength{});
	}
};

class FileReader
{
public:
	FileReader(std::string const& fileName)
	{
		std::ifstream fileIn;
		fileIn.open(fileName);
		if(!fileIn.is_open())
		{
			std::println("error opening file {}", fileName); 
			throw std::runtime_error(std::format("error opening file {}", fileName));
		}
		
		// Store first two items in file as length of a side and number of cities
		int length = 0;
		int numCities = 0;
		fileIn >> length;
		fileIn >> numCities;
	
		fileIn >> firstCity_.x;
		fileIn >> firstCity_.y;
		firstCity_.pointNum = 1;
		
		for(int k=numCities; k!=1; --k) 
		{
			// read rest file here, storing each city into queue, instead of array
			// Iteratively move through file storing points in city
			point tmpPnt;
			fileIn >> tmpPnt.x;
			fileIn >> tmpPnt.y;
			tmpPnt.pointNum = numCities - k + 2;		
			
			pointQueue_.push(tmpPnt);
			std::println("{} {}", tmpPnt.x, tmpPnt.y); 
		}
	}
	point firstCity() const noexcept{ return firstCity_; }
	QuickQueue<point> const& pointQueue() const noexcept { return pointQueue_; }

private:
	point firstCity_;
	QuickQueue<point> pointQueue_;
};

struct Path
{
	PathLength length = std::numeric_limits<PathLength>::max();
	QuickStack<point> points;
};

struct Solver
{
public:
	Solver(
		QuickQueue<point> const& pointQueue,
		point firstCity,
		std::vector<point> const& cityList)
		: table_(cityList)
		, firstCity_(firstCity)
		, citiesToVisit_(pointQueue)
	{
		//bestPath_.length = simpleMinPath(cityList);
		std::println("Initial best path: {}", bestPath_.length);
	}

	void solve()
	{
		hypoMinRemaining_ = table_.hypoMin();
		hypoMinRemaining_ -= table_.getMinEdge(0);
		std::println("Hypothetical min: {}", hypoMinRemaining_);

		// start in first city
		currentPath_.push(firstCity_);

		travel();

		std::ostringstream os;
		os << firstCity_.pointNum << " " << bestPath_.points;
		std::println(
			"Traveled {} times along {} paths with {} longer paths. Final path length {}: {}", 
			travelCounter_, pathCount_, longerPaths_, bestPath_.length, os.str());
	}

private:
	template<typename T>
	struct ValueRestorer
	{
		T original_;
		T& val_;
		explicit ValueRestorer(T& val)
			: original_(val)
			, val_(val)
		{}
		~ValueRestorer() { val_ = original_; }
	};

	void travel()
	{
		++travelCounter_;
		// Make sure distance gets restored on stack unwinding
		ValueRestorer distanceProtector(currentDistance_);
		auto returnDistance
			= table_.getDistance(
				currentPath_.top().pointNum - 1,
				currentPath_.bottom().pointNum - 1);

		if (citiesToVisit_.empty())
		{
			++pathCount_;
			// finalize path: add distance to start
			currentDistance_ += returnDistance;
			// check if path is new min
			if (currentDistance_ < bestPath_.length)
			{
				bestPath_.length = currentDistance_;
				bestPath_.points = currentPath_;
			}
			else
			{
				++longerPaths_;
			}
			return;
		}

		// check hypothetical minimum path length left
		if (currentDistance_ + hypoMinRemaining_ + returnDistance >= bestPath_.length)
		{
			return;
		}
		auto cityCount = citiesToVisit_.size();
		for (int i = 0; i < cityCount; ++i)
		{
			// select next city
			point nextCity = citiesToVisit_.front();
			citiesToVisit_.pop();
			
			// go to city
			ValueRestorer distanceProtectorOnRecursion(currentDistance_);
			ValueRestorer hypoMinRemProtectorOnRecursion(hypoMinRemaining_);
			currentDistance_ += table_.getDistance(
				currentPath_.top().pointNum - 1, 
				nextCity.pointNum - 1);
			hypoMinRemaining_ -= table_.getMinEdge(nextCity.pointNum - 1);
			currentPath_.push(nextCity);

			travel();

			// return to previous city
			currentPath_.pop();
			citiesToVisit_.push(nextCity);
		}
	}

	PathLength simpleMinPath(std::vector<point> cityList) const noexcept
	{
		Path simpleMinPath;
		auto currentCity = cityList.back();
		simpleMinPath.points.push(cityList.back());
		simpleMinPath.length = 0;
		while (!cityList.empty())
		{
			int closestCityIndex = 0;
			PathLength closestCityDistance = std::numeric_limits<PathLength>::max();
			for (int i = 0; i < cityList.size(); ++i)
			{
				PathLength thisDist = table_.getDistance(currentCity.pointNum - 1, cityList[i].pointNum - 1);
				if (thisDist < closestCityDistance)
				{
					closestCityIndex = i;
					closestCityDistance = thisDist;
				}
			}
			currentCity = cityList[closestCityIndex];
			simpleMinPath.points.push(currentCity);
			simpleMinPath.length += closestCityDistance;
			cityList.erase(cityList.begin() + closestCityIndex);
		}
		return simpleMinPath.length;
	}

private:
	int travelCounter_ = 0;
	int pathCount_ = 0;
	int longerPaths_ = 0;
	DistanceTable table_;
	point firstCity_;
	QuickQueue<point> citiesToVisit_;
	QuickStack<point> currentPath_;
	PathLength currentDistance_ = 0;
	PathLength hypoMinRemaining_ = 0;
	Path bestPath_;
};

int main(int argc, char** argv)
{
	if (argc != 2) 
	{
		std::println("error: usage: <exe> <infile>");
		exit(1);
	}

	// arg[1] by convention of this program is the name of the file
	FileReader fileReader(argv[1]);
	std::vector<point> cityList = fileReader.pointQueue().toVector();
	cityList.insert(cityList.begin(), fileReader.firstCity());
	Solver solver(fileReader.pointQueue(), fileReader.firstCity(), cityList);

	solver.solve();

return 0;
}

