#pragma once
#include <queue>

class Mouse {
	friend class WindowClass;
public:
	class Event {
	public:
		enum class Type {ENTER, LEAVE, LDOWN, LUP, RDOWN, RUP, WDOWN, WUP, WDELTA, MOVE, INVALID};
	private:
		Type type;
		bool lPressed, rPressed;
		int x, y;
	public:
		Event() noexcept : type(Type::INVALID), lPressed(false), rPressed(false), x(0), y(0) {}
		Event(Type type, const Mouse& parent) noexcept : 
			type(type), 
			lPressed(parent.lPressed), 
			rPressed(parent.rPressed), 
			x(parent.x),
			y(parent.y) 
		{}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool isLDown() const noexcept;
	bool isRDown() const noexcept;
	bool IsInWindow() const noexcept;
	Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Clear() noexcept;
private:
	void OnMove(int x, int y) noexcept;
	void OnLeave() noexcept;
	void OnEnter() noexcept;
	void OnLDown() noexcept;
	void OnLUp() noexcept;
	void OnRDown() noexcept;
	void OnRUp() noexcept;
	void OnWDown() noexcept;
	void OnWUp() noexcept;
	void OnWDelta(int delta) noexcept;
	void Trim() noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool lPressed = false;
	bool rPressed = false;
	bool inWindow = false;
	int wheelDelta = 0;
	std::queue<Event> buffer;
};
