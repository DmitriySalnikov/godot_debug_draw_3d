#include "render_instances.h"

TextGroup::TextGroup(String title, int priority, bool showTitle, Color groupColor) {
	Title = title;
	GroupPriority = priority;
	ShowTitle = showTitle;
	GroupColor = groupColor;
}

void TextGroup::CleanTexts(std::function<void()> update) {
	std::vector<String> keysToRemove;
	keysToRemove.reserve(Texts.size() / 2);

	for (std::pair<String, DelayedText> k : Texts) {
		if (k.second.IsExpired())
			keysToRemove.push_back(k.first);
	}

	for (String &k : keysToRemove)
		Texts.erase(k);

	if (keysToRemove.size() > 0 && update)
		update();
}

DelayedText::DelayedText(std::chrono::milliseconds expirationTime, String text, int priority, Color color, DebugDraw3D *owner) {
	ExpirationTime = expirationTime;
	Text = text;
	Priority = priority;
	ValueColor = color;
	owner_debug_draw = owner;
}

bool DelayedText::IsExpired() {
	return (TIME_NOW() - ExpirationTime).time_since_epoch().count() > 0;
}