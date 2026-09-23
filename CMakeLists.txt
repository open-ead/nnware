cmake_minimum_required(VERSION 3.15)

project(NintendoWare C CXX)
set(NINTENDOWARE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Downstream projects should set something like:
# set(NN_WARE 1.2.3)
function(target_nn_ver LIB_TARGET SCOPE ID)
  set(VER ${${ID}})
  string(REPLACE "." ";" VER "${VER}")

  list(LENGTH VER SEGS)
  if(SEGS LESS 3)
    set(DEF_VER 1 0 0)
    list(SUBLIST DEF_VER ${SEGS} -1 DEF_VER)
    list(APPEND VER ${DEF_VER})

    message(AUTHOR_WARNING "${ID} semver is not complete (%d.%d.%d): ${${ID}}")
  endif()

  list(POP_FRONT VER MAJOR MINOR PATCH)
  
  foreach(VAR IN ITEMS MAJOR MINOR PATCH)
      target_compile_definitions(${LIB_TARGET} ${SCOPE} ${ID}_${VAR}=${${VAR}})
  endforeach()
endfunction()


# add_subdirectory(..) TODO
