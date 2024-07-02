#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class WindowClass;
public:
	class Event {
	public:
		enum class Type {DOWN, UP, INVALID};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept : type(Type::INVALID), code(0u) {}
		Event(Type t, char c) noexcept : type(t), code(c) {}
		const char GetCode() { return code; }
	};
public:
	Keyboard() = default;

	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	
	char ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;

	void TurnOnAuto() noexcept;
	void TurnOffAuto() noexcept;
	bool isAuto() const noexcept;

	void FlushKeys() noexcept;
	void FlushChars() noexcept;
private:
	void OnKeyDown(unsigned char keycode) noexcept;
	void OnKeyUp(unsigned char keycode) noexcept;
	void OnChar(char c) noexcept;
	void ClearState() noexcept;

	static void Trim(std::queue<char>& buffer) noexcept;
	static void Trim(std::queue<Event>& buffer) noexcept;
private:
	static constexpr unsigned int keyNum = 256u;
	static constexpr unsigned int bufferSize = 16u; //max size till trim is called
	bool isAutoRepeat = false;
	std::bitset<keyNum> keyStates;
	std::queue<char> charBuffer;
	std::queue<Event> eventBuffer;
};
