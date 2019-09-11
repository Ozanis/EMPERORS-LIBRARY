file(REMOVE_RECURSE
  "libCrypt.pdb"
  "libCrypt.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Crypt.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
