remove_definitions(-DRDKIX_RDBOOST_BUILD)
rdkix_python_extension(rdBase
                       RDBase.cpp
                       LINK_LIBRARIES
                       RDGeneral RDBoost)

