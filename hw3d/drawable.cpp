#include "drawable.h"
#include "bindable.h"

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept {
	binds.push_back(std::move(bind));
}

void Drawable::Draw(Graphics& gfx) const noexcept {
	for (auto& b : binds) {
		b->Bind(gfx);
	}
	gfx.DrawIndexed(indexCount);
}