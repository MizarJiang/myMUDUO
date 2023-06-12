aux_source_directory(. SRC_LIST)
# set(SRC_LIST mprpcapplication.cpp  mprpcconfig.cpp rpcprovider.cpp rpcheader.pb.cc mprpcchannel.cpp)

add_library(m_muduo SHARED ${SRC_LIST})

# add_executable(time ${SRC_LIST})

target_link_libraries(m_muduo muduo_net muduo_base pthread)