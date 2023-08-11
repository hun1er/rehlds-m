#-------------------------------------------------------------------------------
# Compile Options
#-------------------------------------------------------------------------------

add_compile_options(
  /MP${PROCESSOR_CORES}         # Build with multiple processes
  /arch:SSE2                    # Minimum CPU architecture requirements
  /external:anglebrackets       # Treat all headers included via <> as external
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
)

#-------------------------------------------------------------------------------
# Link Options
#-------------------------------------------------------------------------------

add_link_options(
  /CGTHREADS:${PROCESSOR_CORES} # Code generation threads
)
