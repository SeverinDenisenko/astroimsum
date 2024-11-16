#include "source_extractor.hpp"

#include <boost/process.hpp>
#include <boost/process/pipe.hpp>

#include <fstream>
#include <sstream>

namespace astro {
    array_t<point_t> source_extractor::extract(string_t file)
    {
        namespace bp = boost::process;

        std::stringstream cmd;
        cmd << "sex " << file << " -c "
            << "config.sex";

        bp::ipstream stdout;
        bp::ipstream stderr;
        bp::child child(cmd.str(), bp::std_out > stdout, bp::std_err > stderr);
        child.join();

        array_t<point_t> result;

        std::ifstream list("list.cat");
        string_t str;
        while (std::getline(list, str)) {
            if (str.front() == '#') {
                continue;
            }

            std::stringstream line(str);
            index_t index;
            point_t point(2);

            line >> index;
            line >> point[0];
            line >> point[1];

            result.push_back(point);
        }

        return result;
    }
}
