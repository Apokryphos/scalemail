cmake_minimum_required (VERSION 3.10)

project(assets NONE)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/scalemail/assets)

configure_file(shaders/fade.frag ${OUTPUT_DIR}/shaders/fade.frag COPYONLY)
configure_file(shaders/fade.vert ${OUTPUT_DIR}/shaders/fade.vert COPYONLY)
configure_file(shaders/flat.frag ${OUTPUT_DIR}/shaders/flat.frag COPYONLY)
configure_file(shaders/flat.vert ${OUTPUT_DIR}/shaders/flat.vert COPYONLY)
configure_file(shaders/sprite.frag ${OUTPUT_DIR}/shaders/sprite.frag COPYONLY)
configure_file(shaders/sprite.vert ${OUTPUT_DIR}/shaders/sprite.vert COPYONLY)

configure_file(textures/actors.png ${OUTPUT_DIR}/textures/actors.png COPYONLY)
configure_file(textures/checker.png ${OUTPUT_DIR}/textures/checker.png COPYONLY)
configure_file(textures/font.png ${OUTPUT_DIR}/textures/font.png COPYONLY)
configure_file(textures/light.png ${OUTPUT_DIR}/textures/light.png COPYONLY)
configure_file(textures/map.png ${OUTPUT_DIR}/textures/map.png COPYONLY)
configure_file(textures/world.png ${OUTPUT_DIR}/textures/world.png COPYONLY)