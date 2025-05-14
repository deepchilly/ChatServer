# 把指定路径下所有源文件名放入SRC_LIST中
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

# 生成可执行文件server
add_executable(chatserver ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

# 目标程序需要链接的动态库
target_link_libraries(chatserver muduo_net muduo_base pthread mysqlclient hiredis)