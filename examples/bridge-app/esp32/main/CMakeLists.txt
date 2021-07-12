idf_component_register(PRIV_INCLUDE_DIRS
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/examples/bridge-app/bridge-common"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/examples/platform/esp32"
                      "${CMAKE_CURRENT_LIST_DIR}/include"
                      SRC_DIRS
                      "${CMAKE_CURRENT_LIST_DIR}"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/examples/bridge-app/bridge-common/gen"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/util"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/reporting"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/basic"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/level-control"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/diagnostic-logs-server"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/descriptor"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/network-commissioning"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/on-off-server"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/operational-credentials-server"
                      "${CMAKE_SOURCE_DIR}/third_party/connectedhomeip/src/app/clusters/general-commissioning-server"
                      PRIV_REQUIRES chip QRCode tft spidriver bt screen-framework)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17) 
target_compile_options(${COMPONENT_LIB} PRIVATE "-DLWIP_IPV6_SCOPES=0" "-DCHIP_HAVE_CONFIG_H")

