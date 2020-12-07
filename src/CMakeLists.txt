project(swapfees)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( swapfees swapfees swapfees.cpp transfer.cpp )
target_include_directories( swapfees PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( swapfees ${CMAKE_SOURCE_DIR}/../ricardian )