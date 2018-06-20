#pragma once

namespace sf
{
class RenderWindow;
}

class BaseRender
{
public:
	BaseRender(sf::RenderWindow* window) : window_(window) {}
	virtual ~BaseRender() = default;

	virtual void update() = 0;

protected:
	sf::RenderWindow* window_;

private:
	BaseRender(const BaseRender& ) = default;
	const BaseRender& operator = (const BaseRender& ) const { return *this; }
};



