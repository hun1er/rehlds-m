#-------------------------------------------------------------------------------
# Compile Options
#-------------------------------------------------------------------------------

add_compile_options(
  /MP${PROCESSOR_CORES}         # Build with multiple processes
  /arch:SSE2                    # Minimum CPU architecture requirements
  /external:anglebrackets       # Treat all headers included via < > as external
  /external:templates-          # Evaluate warning level across template instantiation chain
  /Zc:lambda                    # Enable updated lambda processor

  # Whole-program optimization
  $<IF:$<CONFIG:Debug>,/GL-,/GL>

  # Force fast transcendentals
  $<$<NOT:$<CONFIG:Debug>>:/Qfast_transcendentals>

  # Automatic parallelization of loops
  $<$<NOT:$<BOOL:${ENABLE_ASAN}>>:/Qpar>

  # Faster PDB generation
  $<$<CONFIG:Debug>:/Zf>

  # Suppress compiler warnings
  #
  # Note: In recent versions of the MSVC compiler, the STL is compiled with warnings C4996.
  # This flag should be removed in the future, and the /sdl flag should be enabled.
  /wd4996
)

#-------------------------------------------------------------------------------
# Link Options
#-------------------------------------------------------------------------------

add_link_options(
  # Code generation threads
  /CGTHREADS:${PROCESSOR_CORES}

  # Generate debug info
  $<$<CONFIG:RelWithDebInfo>:/DEBUG:FASTLINK>
)
