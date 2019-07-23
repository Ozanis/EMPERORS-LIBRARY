#ifndef UNTITLED_DCMPRS_H
#define UNTITLED_DCMPRS_H

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace std;

class Gzip {
public:

    Gzip()=default;
    ~Gzip()=default;
    /*void compress()
    {
        namespace bio = boost::iostreams;

        std::stringstream compressed;
        //std::stringstream origin(data);

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
        out.push(origin);
        bio::copy(out, compressed);

        cout << compressed.str() <<endl;
    }*/

    std::string decompress(const std::string& data)
    {
        namespace bio = boost::iostreams;

        std::stringstream compressed(data);
        std::stringstream decompressed;

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::gzip_decompressor());
        out.push(compressed);
        bio::copy(out, decompressed);

        return decompressed.str();
    }
};




#endif //UNTITLED_DCMPRS_H
