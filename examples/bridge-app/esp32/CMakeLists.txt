cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/third_party/connectedhomeip/config/esp32/components"
    "${CMAKE_CURRENT_LIST_DIR}/../../common/m5stack-tft/repo/components/tft"
    "${CMAKE_CURRENT_LIST_DIR}/../../common/m5stack-tft/repo/components/spidriver"
    "${CMAKE_CURRENT_LIST_DIR}/../../common/QRCode"
    "${CMAKE_CURRENT_LIST_DIR}/../../common/screen-framework"
)

project(chip-bridge-app)
idf_build_set_property(CXX_COMPILE_OPTIONS "-std=gnu++17;-Os;-DLWIP_IPV6_SCOPES=0;-DCHIP_HAVE_CONFIG_H" APPEND)
idf_build_set_property(C_COMPILE_OPTIONS "-Os;-DLWIP_IPV6_SCOPES=0" APPEND)
