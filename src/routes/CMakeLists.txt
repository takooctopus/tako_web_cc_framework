# 此为模块routers所使用的CMakelists.txt

message("   -----------------------------------------------------------------")
message("--  ++[进入routers模块编译]")

set(MODULE_ROUTERS  routers)
aux_source_directory(. ROUTERS_SOURCE)

add_library(${MODULE_ROUTERS} SHARED ${ROUTERS_SOURCE})