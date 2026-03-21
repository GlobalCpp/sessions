#include <algorithm>
#include <cmath>
#include <expected>
#include <fstream>
#include <limits>
#include <print>
#include <ranges>
#include <string>
#include <vector>

struct City {
    int  id;   // 1-based row number from file
    double x, y;
};

// Lower bound proof:
//   Any remaining path  last → u1 → u2 → ... → uk → start  consists of k+1 edges.
//   By definition min_edge[v] ≤ dist(v, any_other_city), so:
//     min_edge[last] ≤ dist(last, u1)
//     min_edge[u_i]  ≤ dist(u_i, u_{i+1})   for i < k
//     min_edge[uk]   ≤ dist(uk, start)
//   Therefore  remaining_cost ≥ min_edge[last] + Σ min_edge[u_i]
class TspSolver {
public:
    explicit TspSolver(std::vector<City> cities)
        : cities_(std::move(cities))
        , n_(static_cast<int>(cities_.size()))
        , dists_(n_ * n_)
        , visited_(n_, false)
        , min_edge_(n_, std::numeric_limits<double>::max())
    {
        build_distances();
        build_min_edges();
        current_path_.reserve(n_);
    }

    void solve() {
        current_path_.push_back(0);
        visited_[0] = true;
        // Sum of min_edge for all cities not yet in the path (all except start city 0).
        remaining_lb_sum_ = 0.0;
        for (int i : std::views::iota(1, n_))
            remaining_lb_sum_ += min_edge_[i];
		std::println("hypothetical min: {}", remaining_lb_sum_);

        branch_and_bound(0.0);
		std::println("Call Count: {} along {} paths with {} longer paths. ", call_count_, pathCount_, longerPaths_);
    }

    double best_length() const { return best_length_; }
    const std::vector<int>&  best_path() const { return best_path_; }
    const std::vector<City>& cities()    const { return cities_; }

private:
    std::vector<City>   cities_;
    int                 n_;
    std::vector<double> dists_;   // flat row-major distance matrix
    std::vector<bool>   visited_;
    std::vector<double> min_edge_;
    std::vector<int>    current_path_;
    std::vector<int>    best_path_;
    double best_length_     = std::numeric_limits<double>::max();
    double remaining_lb_sum_ = 0.0;
	int call_count_ = 0;
	int pathCount_ = 0;
	int longerPaths_ = 0;

    double dist(int i, int j) const {
        return dists_[i * n_ + j];
    }

    void build_distances() {
        for (int i : std::views::iota(0, n_)) {
            for (int j : std::views::iota(0, n_)) {
                double dx = cities_[i].x - cities_[j].x;
                double dy = cities_[i].y - cities_[j].y;
                dists_[i * n_ + j] = std::sqrt(dx * dx + dy * dy);
            }
        }
    }

    void build_min_edges() {
        for (int i : std::views::iota(0, n_))
            for (int j : std::views::iota(0, n_))
                if (i != j)
                    min_edge_[i] = std::min(min_edge_[i], dist(i, j));
    }

    void branch_and_bound(double current_dist) {
		++call_count_;
        if (static_cast<int>(current_path_.size()) == n_) {
			++pathCount_;
            double total = current_dist + dist(current_path_.back(), 0);
            if (total < best_length_) {
                best_length_ = total;
                best_path_   = current_path_;
            }
			else
			{
				++longerPaths_;
			}
            return;
        }

        int last = current_path_.back();

        // Prune: current cost + lower bound on remaining path ≥ best known
        if (current_dist + remaining_lb_sum_ + min_edge_[last] >= best_length_)
            return;

        for (int next : std::views::iota(0, n_)) {
            if (visited_[next]) continue;

            visited_[next] = true;
            current_path_.push_back(next);
            remaining_lb_sum_ -= min_edge_[next];

            branch_and_bound(current_dist + dist(last, next));

            remaining_lb_sum_ += min_edge_[next];
            current_path_.pop_back();
            visited_[next] = false;
        }
    }
};

std::expected<std::vector<City>, std::string>
load_cities(std::string_view filename) {
    std::ifstream file{std::string(filename)};
    if (!file)
        return std::unexpected(std::format("cannot open file: {}", filename));

    int side_length{}, num_cities{};
    if (!(file >> side_length >> num_cities) || num_cities <= 0)
        return std::unexpected("invalid file header");

    std::vector<City> cities;
    cities.reserve(num_cities);

    for (int row = 1; row <= num_cities; ++row) {
        double x{}, y{};
        if (!(file >> x >> y))
            return std::unexpected(
                std::format("failed to read city at row {}", row));
        cities.push_back({.id = row, .x = x, .y = y});
    }
    return cities;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::println(stderr, "usage: {} <city_file>", argv[0]);
        return 1;
    }

    auto result = load_cities(argv[1]);
    if (!result) {
        std::println(stderr, "error: {}", result.error());
        return 1;
    }

    if (result->empty()) {
        std::println(stderr, "error: no cities in file");
        return 1;
    }

    TspSolver solver{std::move(*result)};
    solver.solve();

    std::println("Shortest tour length: {:.6f}", solver.best_length());

    std::print("Path: ");
    const auto& path   = solver.best_path();
    const auto& cities = solver.cities();
    for (int idx : path)
        std::print("{} -> ", cities[idx].id);
    std::println("{}", cities[path.front()].id);

    return 0;
}
