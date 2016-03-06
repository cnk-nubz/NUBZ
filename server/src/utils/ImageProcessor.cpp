#include "FileHandler.h"
#include "ImageProcessor.h"

namespace utils {

ImageProcessor::ImageProcessor(const boost::filesystem::path &srcImagePath)
    : srcImagePath(srcImagePath.string()), tileSize(256) {
    reset();
}

std::size_t ImageProcessor::width() const {
    return img.columns();
}

std::size_t ImageProcessor::height() const {
    return img.rows();
}

void ImageProcessor::reset() {
    img = Magick::Image(srcImagePath);
}

void ImageProcessor::addFrameToBeDivisibleBy(std::size_t x) {
    std::size_t dx = (x - (width() % x)) % x;
    std::size_t dy = (x - (height() % x)) % x;

    Magick::Image newImg(Magick::Geometry(width() + dx, height() + dy),
                         Magick::ColorRGB(1.0, 1.0, 1.0));
    newImg.composite(img, dx / 2, dy / 2);
    img = newImg;
}

void ImageProcessor::scale(std::size_t maxSize) {
    std::size_t orgWidth = img.columns();
    std::size_t orgHeight = img.rows();

    std::size_t maxOrgSize = std::max(orgHeight, orgWidth);
    img.zoom(Magick::Geometry((double(orgWidth) * maxSize) / maxOrgSize,
                              (double(orgHeight) * maxSize) / maxOrgSize));
}

void ImageProcessor::saveImageTo(const std::string &path) {
    img.write(path);
}

void ImageProcessor::setTileSize(std::size_t size) {
    tileSize = size;
}

std::vector<std::vector<std::string>> ImageProcessor::generateTiles(
    const boost::filesystem::path &dstDir) {
    static const std::size_t minSize = 16;

    const std::size_t width = this->width();
    const std::size_t height = this->height();

    std::vector<std::vector<std::string>> result;
    try {
        std::vector<utils::FileHandler> handlers;

        for (std::size_t x = 0; x < width; x += tileSize) {
            if (width - x < minSize) {
                // ignore remnants
                break;
            }

            std::string pathSuffix = std::to_string(x / tileSize);
            boost::filesystem::path xPath = dstDir / pathSuffix;
            boost::filesystem::path xPathShort = pathSuffix;
            boost::filesystem::create_directory(xPath);
            handlers.emplace_back(xPath.string());

            result.emplace_back();
            for (std::size_t y = 0; y < height; y += tileSize) {
                if (height - y < minSize) {
                    // ignore remnants
                    break;
                }

                std::string filename = std::to_string(y / tileSize) + ".jpg";
                boost::filesystem::path yPath = xPath / filename;
                boost::filesystem::path yPathShort = xPathShort / filename;

                Magick::Image img = cutFromScaledAt(x, y);
                img.magick("JPEG");

                img.write(yPath.string());
                result.back().push_back(yPathShort.string());
            }
        }

        for (auto &handler : handlers) {
            handler.release();
        }
    } catch (std::exception &e) {
        throw std::runtime_error(e.what());
    }

    return result;
}

Magick::Image ImageProcessor::cutFromScaledAt(std::size_t x, std::size_t y) const {
    Magick::Image tile = img;

    std::size_t tileWidth = std::min(tileSize, width() - x);
    std::size_t tileHeight = std::min(tileSize, height() - y);

    tile.chop(Magick::Geometry(x, y));
    tile.crop(Magick::Geometry(tileWidth, tileHeight));

    return tile;
}
}