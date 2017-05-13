#include "UI.h"

void UIBar::draw()
{
	int width = w * (value / value_max) - 2;
	auto renderer = Game::Instance().getRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), r, g, b, 255);
	rect = { x + 1, y + 1, w - 2, h - 2 };
	SDL_RenderFillRect(renderer, &rect);
}

UIBar::UIBar(int x, int y, int w, int h, int value_max)
	:UIEntity(x, y, w, h, value_max), r(255), g(255), b(255) {
}