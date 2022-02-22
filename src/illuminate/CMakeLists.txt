# 此为模块app/http/controllers所使用的CMakelists.txt

message("           -----------------------------------------------------------------")
message("--         ++[进入illuminate模块编译]")

set(MODULE_ILLUMINATES    illuminate)
aux_source_directory(. ILLUMINATE_SOURCE)

add_library(${MODULE_ILLUMINATES} SHARED ${ILLUMINATE_SOURCE})
