#include "keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return keyStates[keycode];
}
bool Keyboard::IsKeyEmpty() const noexcept { return eventBuffer.empty(); }
Keyboard::Event Keyboard::ReadKey() noexcept {
	if (!IsKeyEmpty()) {
		Event e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
	else {
		return Keyboard::Event(); //default event is invalid
	}
}
bool Keyboard::IsCharEmpty() const noexcept { return charBuffer.empty(); }
char Keyboard::ReadChar() noexcept {
	if (!IsCharEmpty()) {
		char c = charBuffer.front();
		charBuffer.pop();
		return c;
	}
	else {
		return 0; // null character
	}
}
void Keyboard::TurnOnAuto() noexcept { isAutoRepeat = true; }
void Keyboard::TurnOffAuto() noexcept { isAutoRepeat = false; }
bool Keyboard::isAuto() const noexcept { return isAutoRepeat; }

void Keyboard::FlushKeys() noexcept { eventBuffer = std::queue<Event>(); }
void Keyboard::FlushChars() noexcept { charBuffer = std::queue<char>(); }

void Keyboard::OnKeyDown(unsigned char keycode) noexcept {
	keyStates[keycode] = true;
	eventBuffer.emplace(Event(Event::Type::DOWN, keycode));
	Trim(eventBuffer);
}
void Keyboard::OnKeyUp(unsigned char keycode) noexcept {
	keyStates[keycode] = false;
	eventBuffer.emplace(Event(Event::Type::UP, keycode));
	Trim(eventBuffer);
}
void Keyboard::OnChar(char c) noexcept {
	charBuffer.emplace(c);
	Trim(charBuffer);
}
void Keyboard::ClearState() noexcept {
	keyStates.reset();
}

void Keyboard::Trim(std::queue<char>& charBuffer) noexcept {
	while (charBuffer.size() > bufferSize) charBuffer.pop();
}
void Keyboard::Trim(std::queue<Event>& eventBuffer) noexcept {
	while (eventBuffer.size() > bufferSize) eventBuffer.pop();
}