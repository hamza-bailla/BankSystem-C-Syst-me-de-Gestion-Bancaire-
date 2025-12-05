#pragma once
 //-------------------- Permissions --------------------
enum class Permission : uint32_t {
    NONE = 0,
    ADD_CLIENT = 1 << 0,
    REMOVE_CLIENT = 1 << 1,
    MODIFY_CLIENT = 1 << 2,
    DEPOT = 1 << 3,
    RETRAIT = 1 << 4,
    VIEW_CLIENTS = 1 << 5,
    TRANSFERT = 1 << 6,
    // tu peux ajouter d'autres permissions ici
};

inline Permission operator|(Permission a, Permission b) {
    return static_cast<Permission>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline Permission operator&(Permission a, Permission b) {
    return static_cast<Permission>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
inline bool hasPermission(Permission flags, Permission p) {
    return static_cast<uint32_t>(flags & p) != 0;
}
