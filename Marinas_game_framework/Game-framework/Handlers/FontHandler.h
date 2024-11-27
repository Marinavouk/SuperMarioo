#pragma once

class CApplication;

class CFontHandler
{
public:

	 CFontHandler(void) {}
	 CFontHandler(CApplication* application) : m_pApplication(application) {}
	~CFontHandler(void) {}

	TTF_Font*	CreateFont(const std::string& fileName, const int32_t textSize);
	void		DestroyFont(TTF_Font* font);

	bool		RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_FPoint& position, const SDL_Color& color);

	/**
	* Use this function to retrieve the size of a string (in pixels)
	* Imagine a rectangle completely surrounding the text and the size of this rectangle is what is returned
	* Returns an SDL_FPoint where the x-field of the point is the width of the text and the y-field of the point is the height of the text
	*/
	SDL_FPoint	GetTextSize(TTF_Font* font, const std::string& text);

private:

	CApplication* m_pApplication = nullptr;

};