#include "common.hpp"
#include <format>
#include <filesystem>
#include "utility/luau.hpp"

static auto namecall(state_t L) -> int {
    auto& self = path_builder_t::self(L);
    const auto [atom, name] = luau::namecall_atom<method_name>(L);
    using mn = method_name;
    switch (atom) {
        case mn::string:
            return luau::push(L, self);
        case mn::extension:
            return luau::push(L, self.extension());
        case mn::has_extension:
            return luau::push(L, self.has_extension());
        case mn::replace_extension:
            return path_builder_t::push(L, self.replace_extension(luaL_checkstring(L, 2)));
        case mn::parent_path:
            return path_builder_t::push(L, self.parent_path());
        case mn::is_absolute:
            return luau::push(L, self.is_absolute());
        case mn::is_relative:
            return luau::push(L, self.is_relative());
        case mn::filename:
            return luau::push(L, self.filename());
        case mn::has_filename:
            return luau::push(L, self.has_filename());
        case mn::replace_filename:
            return path_builder_t::push(L, self.replace_filename(luaL_checkstring(L, 2)));
        case mn::remove_filename:
            return path_builder_t::push(L, self.remove_filename());
        case mn::generic_string:
            return luau::push(L, self.generic_string());
        default:
            break;
    }
    luaL_errorL(L, "invalid namecall '%s'", name);
}
static auto tostring(state_t L) -> int {
    auto fmt = std::format("\"{}\"", path_builder_t::self(L).string());
    lua_pushstring(L, fmt.c_str());
    return 1;
}
static auto div(state_t L) -> int {
    return path_builder_t::push(L, to_path(L, 1) / to_path(L, 2));
}

void register_path(state_t L) {
    const luaL_Reg metatable[] = {
        {"__namecall", namecall},
        {"__tostring", tostring},
        {"__div", div},
        {nullptr, nullptr}
    };
    path_builder_t::register_type(L, metatable);
}
auto to_path(state_t L, int idx) -> path_t {
    return path_builder_t::get_or(L, idx, [&]{return luaL_checkstring(L, idx);});
}
