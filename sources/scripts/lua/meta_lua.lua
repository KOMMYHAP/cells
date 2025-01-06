---@meta

---@class lua_native
lua_native = {}


---@return integer bytes memory usage in bytes
function lua_native:get_memory_used() end

---@return boolean value is garbage collector on?
function lua_native:is_gc_on() end