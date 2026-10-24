#ifndef TOUCHGFX_SCALABLEIMAGE_HPP
#define TOUCHGFX_SCALABLEIMAGE_HPP

#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{

class ScalableImage : public Image
{
public:
    enum ScalingAlgorithm
    {
        NEAREST_NEIGHBOR,
        BILINEAR_INTERPOLATION
    };

    ScalableImage(const Bitmap& bmp = Bitmap(BITMAP_INVALID))
        : Image(bmp), scalingAlgorithm(BILINEAR_INTERPOLATION)
    {}

    virtual ~ScalableImage() {}

    virtual void setBitmap(const Bitmap& bmp) override
    {
        bitmap = bmp;
        if (rect.width == 0 && rect.height == 0 && bmp.getId() != BITMAP_INVALID)
        {
            setWidthHeight(bmp.getWidth(), bmp.getHeight());
        }
    }

    void setScalingAlgorithm(ScalingAlgorithm algorithm)
    {
        scalingAlgorithm = algorithm;
    }

    ScalingAlgorithm getScalingAlgorithm() const
    {
        return scalingAlgorithm;
    }

protected:
    ScalingAlgorithm scalingAlgorithm;
};

} // namespace touchgfx

#endif // TOUCHGFX_SCALABLEIMAGE_HPP
