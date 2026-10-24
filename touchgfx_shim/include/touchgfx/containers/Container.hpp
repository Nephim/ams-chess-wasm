#ifndef TOUCHGFX_CONTAINER_HPP
#define TOUCHGFX_CONTAINER_HPP

#include <touchgfx/Drawable.hpp>
#include <vector>
#include <algorithm>

namespace touchgfx
{

class Container : public Drawable
{
public:
    Container() : Drawable() {}

    virtual ~Container()
    {
        for (Drawable* child : children)
        {
            if (child && child->getParent() == this)
            {
                child->setParent(nullptr);
            }
        }
        children.clear();
    }

    virtual void add(Drawable& d)
    {
        if (d.getParent() && d.getParent() != this)
        {
            d.getParent()->remove(d);
        }
        auto it = std::find(children.begin(), children.end(), &d);
        if (it == children.end())
        {
            children.push_back(&d);
        }
        d.setParent(this);
    }

    virtual void remove(Drawable& d)
    {
        auto it = std::find(children.begin(), children.end(), &d);
        if (it != children.end())
        {
            children.erase(it);
        }
        if (d.getParent() == this)
        {
            d.setParent(nullptr);
        }
    }

    virtual void removeAll()
    {
        for (Drawable* child : children)
        {
            if (child && child->getParent() == this)
            {
                child->setParent(nullptr);
            }
        }
        children.clear();
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible) return;
        int16_t absX = parentAbsX + rect.x;
        int16_t absY = parentAbsY + rect.y;
        for (const Drawable* child : children)
        {
            if (child && child->isVisible())
            {
                child->draw(absX, absY, parentAlpha);
            }
        }
    }

    virtual Drawable* findTouchable(int16_t localX, int16_t localY, int16_t& outRelX, int16_t& outRelY) override
    {
        if (!visible) return nullptr;

        for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
        {
            Drawable* child = children[i];
            if (!child || !child->isVisible()) continue;

            int16_t childX = localX - child->getX();
            int16_t childY = localY - child->getY();
            if (childX >= 0 && childX < child->getWidth() &&
                childY >= 0 && childY < child->getHeight())
            {
                Drawable* hit = child->findTouchable(childX, childY, outRelX, outRelY);
                if (hit) return hit;
            }
        }

        if (touchable && localX >= 0 && localX < rect.width && localY >= 0 && localY < rect.height)
        {
            outRelX = localX;
            outRelY = localY;
            return this;
        }
        return nullptr;
    }

    const std::vector<Drawable*>& getChildren() const { return children; }

protected:
    std::vector<Drawable*> children;
};

} // namespace touchgfx

#endif // TOUCHGFX_CONTAINER_HPP
