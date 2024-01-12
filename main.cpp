#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point
{
    double x { 0.0 };
    double y { 0.0 };
    double z { 0.0 };

    double total_distance { 0.0 };

    double distance_to(const Point& p2)
    {
        return std::sqrt(std::pow(p2.x - x, 2)
                         + std::pow(p2.y - y, 2)
                         + std::pow(p2.z - z, 2));
    }

    static Point from_total_distance(double total_distance,
                                     const std::vector<Point>& points)
    {
        int begin_index { 0 };
        int end_index { static_cast<int>(points.size() - 2) };

        while (begin_index <= end_index)
        {
            int middle_index { (begin_index + end_index) / 2 };
            const auto& p1 { points[middle_index] };
            const auto& p2 { points[middle_index + 1] };

            if (total_distance < p1.total_distance)
            {
                end_index = middle_index - 1;
            }
            else if (total_distance > p2.total_distance)
            {
                begin_index = middle_index + 1;
            }
            else
            {
                double ratio { (total_distance - p1.total_distance)
                             / (p2.total_distance - p1.total_distance) };

                return Point { p1.x + (p2.x - p1.x) * ratio,
                             p1.y + (p2.y - p1.y) * ratio,
                             p1.z + (p2.z - p1.z) * ratio,
                             total_distance };
            }
        }

        return Point {};
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& p);
};

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    return out << "Point { " << p.x << ' ' << p.y << ' ' << p.z << " | " << p.total_distance << " }";
}

std::vector<Point> load_points(const std::string& file_path)
{
    std::vector<Point> points {};

    std::ifstream file { file_path };

    double x { 0.0 };

    while (file >> x)
    {
        auto& point { points.emplace_back(Point {}) };

        point.x = x;
        file >> point.y;
        file >> point.z;

        if (points.size() > 1)
        {
            const auto& previous_point { points[points.size() - 2] };

            point.total_distance = previous_point.total_distance
                                   + point.distance_to(previous_point);
        }
    }

    return points;
}

int main()
{
    const auto points { load_points("../test.txt") };

    for (int i { 0 }; i <= 50; ++i)
    {
        std::cout << Point::from_total_distance(i, points) << '\n';
    }

    return 0;
}
