#ifndef IMAGE__IMAGE_PROCESSOR__H
#define IMAGE__IMAGE_PROCESSOR__H

#include <Magick++.h>

namespace img {
    class ImageProcessor {
    public:
        ImageProcessor(const std::string &pathToOriginalImage);

        void setTileSize(std::size_t size);

        // scale to make original image longer edge == maxSize
        void setScale(std::size_t maxSize);

        // generates tiles, saves them in files, returns list of filenames
        // in case of crash removes created files and throws std::runtime_error
        std::vector<std::vector<std::string>> generateTiles(const std::string &filepathPrefix);

    private:
        const std::string originalImgPath;
        Magick::Image scaledImg;
        std::size_t maxSizeForScale;
        std::size_t tileSize;

        void createScaled();
        Magick::Image cutFromScaledAt(std::size_t x, std::size_t y) const;
    };
}

#endif