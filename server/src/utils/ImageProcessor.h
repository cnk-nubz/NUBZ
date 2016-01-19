#ifndef UTILS__IMAGE_PROCESSOR__H
#define UTILS__IMAGE_PROCESSOR__H

#include <Magick++.h>
#include <boost/filesystem.hpp>

namespace utils {

class ImageProcessor {
public:
    ImageProcessor(const std::string &pathToOriginalImage);

    std::size_t width() const;
    std::size_t height() const;

    // loads file from given path
    void reset();

    // adds white frame to make image width and height divisible by x
    void addFrameToBeDivisibleBy(std::size_t x);

    // scale to make original image longer edge == maxSize
    void scale(std::size_t maxSize);

    void saveImageTo(const std::string &path);

    void setTileSize(std::size_t size);

    // generates tiles, saves them, returns list of filenames
    // in case of crash removes created files and throws std::runtime_error
    // directories structure: {dstDir}/{x}/{y}.jpg
    // results form: {x}/{y}.jpg
    std::vector<std::vector<std::string>> generateTiles(const boost::filesystem::path &dstDir);

private:
    const std::string originalImgPath;
    Magick::Image img;
    std::size_t tileSize;

    Magick::Image cutFromScaledAt(std::size_t x, std::size_t y) const;
};
}

#endif