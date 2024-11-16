#include "source_extractor.hpp"

#include <boost/process.hpp>
#include <boost/process/pipe.hpp>

#include <fstream>
#include <sstream>

array_t<point_t> source_extractor::extract(string_t file)
{
    using namespace boost::process;

    std::stringstream cmd;
    cmd << "sex " << file << " "
        << "config.sex";

    ipstream stdout;
    ipstream stderr;
    child child(cmd.str(), std_out > stdout, std_err > stderr);

    child.wait();

    array_t<point_t> result;

    std::ifstream list("list.cat");
    string_t str;
    while (std::getline(list, str)) {
        if (str.front() == '#') {
            continue;
        }

        std::stringstream line(str);
        index_t index;
        point_t point;

        line >> index;
        line >> point[0];
        line >> point[1];

        result.push_back(point);
    }

    return result;
}
