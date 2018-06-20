#pragma once

namespace sf
{
class RenderWindow;
}

class BaseRender
{
public:
	BaseRender(sf::RenderWindow* window) : window_(window) { font_.loadFromFile("resources/sansation.ttf"); }
	virtual ~BaseRender() = default;

	virtual void update() = 0;

protected:
	sf::Font font_;
	sf::RenderWindow* window_ = nullptr;

private:
	BaseRender(const BaseRender& ) = default;
	BaseRender(const BaseRender&& ) noexcept {};
	const BaseRender& operator = (const BaseRender& ) const { return *this; }
	const BaseRender& operator = (const BaseRender&& ) const { return *this; }
};



