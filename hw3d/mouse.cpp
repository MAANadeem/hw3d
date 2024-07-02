#include "mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept { return { x,y }; }
int Mouse::GetPosX() const noexcept { return x; }
int Mouse::GetPosY() const noexcept { return y; }
bool Mouse::isLDown() const noexcept { return lPressed; }
bool Mouse::isRDown() const noexcept { return rPressed; }
bool Mouse::IsEmpty() const noexcept { return buffer.empty(); }
bool Mouse::IsInWindow() const noexcept { return inWindow; }
void Mouse::Clear() noexcept { buffer = std::queue<Event>(); }

Mouse::Event Mouse::Read() noexcept {
	if (!IsEmpty()) {
		Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return Mouse::Event();
	}
}

void Mouse::Trim() noexcept {
	while (buffer.size() > bufferSize) buffer.pop();
}

void Mouse::OnMove(int x, int y) noexcept {
	this->x = x; this->y = y;
	buffer.emplace(Event(Event::Type::MOVE, *this));
	Trim();
}
void Mouse::OnLDown() noexcept {
	lPressed = true;
	buffer.emplace(Event(Event::Type::LDOWN, *this));
	Trim();
}
void Mouse::OnLUp() noexcept {
	lPressed = false;
	buffer.emplace(Event(Event::Type::LUP, *this));
	Trim();
}
void Mouse::OnRDown() noexcept {
	rPressed = true;
	buffer.emplace(Event(Event::Type::RDOWN, *this));
	Trim();
}
void Mouse::OnRUp() noexcept {
	rPressed = false;
	buffer.emplace(Event(Event::Type::RUP, *this));
	Trim();
}
void Mouse::OnWDown() noexcept {
	buffer.emplace(Event(Event::Type::WDOWN, *this));
	Trim();
}
void Mouse::OnWUp() noexcept {
	buffer.emplace(Event(Event::Type::WUP, *this));
	Trim();
}
void Mouse::OnLeave() noexcept {
	inWindow = false;
	buffer.emplace(Event(Event::Type::LEAVE, *this));
	Trim();
}
void Mouse::OnEnter() noexcept {
	inWindow = true;
	buffer.emplace(Event(Event::Type::ENTER, *this));
	Trim();
}
void Mouse::OnWDelta(int delta) noexcept {
	wheelDelta += delta;
	const int stdDelta = 120;
	// generate events for every 120 
	while (wheelDelta >= stdDelta)
	{
		wheelDelta -= stdDelta;
		OnWUp();
	}
	while (wheelDelta <= -stdDelta)
	{
		wheelDelta += stdDelta;
		OnWDown();
	}
}