
#include "../thirdparty/tracy/public/TracyClient.cpp"

// Undefine `poll` as it may override godot::HTTPClient::poll
#undef poll