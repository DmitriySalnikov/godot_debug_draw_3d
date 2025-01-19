
// `likely` and `unlikely` are redefined in tracy
#pragma push_macro("likely")
#pragma push_macro("unlikely")
#undef likely
#undef unlikely

#include "../thirdparty/tracy/public/TracyClient.cpp"

#pragma pop_macro("likely")
#pragma pop_macro("unlikely")

// Undefine `poll` as it may override godot::HTTPClient::poll
#undef poll