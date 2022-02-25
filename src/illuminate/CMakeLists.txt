# 此为模块app/http/controllers所使用的CMakelists.txt

message("           -----------------------------------------------------------------")
message("--         ++[进入illuminate模块编译]")


set(MODULE_ILLUMINATES    illuminate)

# aux_source_directory(./Kernel ILLUMINATE_SOURCE)
# aux_source_directory(. ILLUMINATE_SOURCE)

set (ILLUMINATE_SOURCE 
    # Kernel 相关cc，暂时没什么问题
    ./Kernel/noncopyable.h
    ./Kernel/FileUtil.cc
    ./Kernel/MutexLock.h
    ./Kernel/CurrentThread.h
    ./Kernel/Thread.cc
    ./Kernel/Condition.h
    ./Kernel/CountDownLatch.cc
    ./Kernel/LogStream.cc
    ./Kernel/AsyncLogging.cc
    ./Kernel/LogFile.cc
)

message("--    测试ILLUMINATE[父目录] : MyVAL : ${ILLUMINATE_SOURCE}")

# aux_source_directory(. ILLUMINATE_SOURCE)

# add_subdirectory(Kernel)

add_library(${MODULE_ILLUMINATES} SHARED ${ILLUMINATE_SOURCE})
