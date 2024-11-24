#pragma once

class CApplication;

class CInputHandler
{
public:

	CInputHandler(void) {}
	CInputHandler(CApplication* application) : m_pApplication(application) {}
	~CInputHandler(void) {}

	void		Update(void);

	bool		KeyPressed(const SDL_Scancode keyboardKey);
	bool		KeyHeld(const SDL_Scancode keyboardKey);
	bool		KeyReleased(const SDL_Scancode keyboardKey);

	bool		MouseButtonPressed(const int32_t mouseButton);
	bool		MouseButtonHeld(const int32_t mouseButton);
	bool		MouseButtonReleased(const int32_t mouseButton);

public:

	// Place getters and setters in their own public field

	SDL_FPoint	GetMousePosition(void);

private:

	CApplication* m_pApplication = nullptr;

	SDL_FPoint		m_MousePosition = { 0.0f, 0.0f };

	bool			m_CurrentKeyboardState[SDL_NUM_SCANCODES] = { false };
	bool			m_PreviousKeyboardState[SDL_NUM_SCANCODES] = { false };
	bool			m_CurrentMouseState[8] = { false };
	bool			m_previousMouseState[8] = { false };

};