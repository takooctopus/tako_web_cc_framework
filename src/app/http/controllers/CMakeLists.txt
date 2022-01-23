# 此为模块app/http/controllers所使用的CMakelists.txt

message("           -----------------------------------------------------------------")
message("--         ++[进入app/http/controllers模块编译]")

set(MODULE_CONTROLLERS    controllers)
aux_source_directory(. CONTROLLERS_SOURCE)

add_library(${MODULE_CONTROLLERS} SHARED ${CONTROLLERS_SOURCE})
