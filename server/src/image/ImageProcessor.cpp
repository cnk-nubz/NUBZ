#include "ImageProcessor.h"
#include "utils/FileHandler.h"

namespace img {
    ImageProcessor::ImageProcessor(const std::string &pathToOriginalImage)
        : originalImgPath(pathToOriginalImage), maxSizeForScale(1024), tileSize(256) {
    }

    void ImageProcessor::setTileSize(std::size_t size) {
        tileSize = size;
    }

    void ImageProcessor::setScale(std::size_t maxSize) {
        maxSizeForScale = maxSize;
    }

    std::vector<std::vector<std::string>> ImageProcessor::generateTiles(
        const std::string &filepathPrefix) {
        createScaled();

        const std::size_t width = scaledImg.columns();
        const std::size_t height = scaledImg.rows();

        std::vector<std::vector<std::string>> result;
        try {
            std::vector<utils::FileHandler> handlers;

            for (std::size_t x = 0; x < width; x += tileSize) {
                result.emplace_back();
                for (std::size_t y = 0; y < height; y += tileSize) {
                    Magick::Image img = cutFromScaledAt(x, y);
                    img.magick("JPEG");

                    std::string filename = filepathPrefix + "_x" + std::to_string(x / tileSize) +
                                           "_y" + std::to_string(y / tileSize) + ".jpg";
                    img.write(filename);
                    handlers.emplace_back(filename);
                    result.back().push_back(filename);
                }
            }

            for (auto &handler : handlers) {
                handler.release();
            }
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }

        scaledImg = Magick::Image();

        return result;
    }

    void ImageProcessor::createScaled() {
        scaledImg = Magick::Image(originalImgPath);
        std::size_t orgWidth = scaledImg.columns();
        std::size_t orgHeight = scaledImg.rows();

        std::size_t maxOrgSize = std::max(orgHeight, orgWidth);
        scaledImg.zoom(Magick::Geometry((double(orgWidth) * maxSizeForScale) / maxOrgSize,
                                        (double(orgHeight) * maxSizeForScale) / maxOrgSize));
    }

    Magick::Image ImageProcessor::cutFromScaledAt(std::size_t x, std::size_t y) const {
        Magick::Image tile = scaledImg;

        std::size_t width = std::min(tileSize, scaledImg.columns() - x);
        std::size_t height = std::min(tileSize, scaledImg.rows() - y);

        tile.chop(Magick::Geometry(x, y));
        tile.crop(Magick::Geometry(width, height));

        return tile;
    }
}