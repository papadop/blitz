add_custom_target(blitz-doc ALL DEPENDS stencils doc-examples)

add_subdirectory(examples)
add_subdirectory(stencils)
add_subdirectory(doxygen)

set(STENCILS
    backward11.texi backward12.texi backward21.texi backward22.texi backward31.texi backward32.texi backward41.texi backward42.texi
    central12.texi central14.texi central22.texi central24.texi central32.texi central34.texi central42.texi central44.texi
    forward11.texi forward12.texi forward21.texi forward22.texi forward31.texi forward32.texi forward41.texi forward42.texi
    Laplacian2D4.texi Laplacian2D.texi)

foreach(i ${STENCILS})
    set(TEXINFOS ${TEXINFOS} stencil/${i})
endforeach()

set(EXAMPLES
    cast.texi debug.texi dump.texi fixed-point.texi fixed.texi io.texi outer.texi output.texi range.texi simple.texi slicing.texi
    storage.texi strideslice.texi xor.texi)

set(OUTPUTS
    cast.out debug.out dump.out fixed.out io.out outer.out output.out
    range.out simple.out slicing.out storage.out strideslice.out xor.out)

foreach(i ${EXAMPLES} ${OUTPUTS})
    set(TEXINFOS ${TEXINFOS} examples/${i})
endforeach()

set(TEXINFOS
    about.texi arrays-ctors.texi arrays-debug.texi arrays-expr.texi arrays-globals.texi arrays-indirect.texi arrays-intro.texi
    arrays-io.texi arrays-members.texi arrays-multi.texi arrays-slicing.texi arrays-stencils.texi arrays-storage.texi arrays-types.texi
    arrays-usertype.texi compiling.texi constants.texi download.texi faq.texi help.texi install.texi legal.texi numinquire.texi
    parallel.texi platforms.texi random.texi tau.texi tinymatrix.texi tinyvector.texi tuning.texi copyright.texi)

set(TEXI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/blitz.texi)
set(TEXI_PROG_ARGS -I ${CMAKE_CURRENT_SOURCE_DIR} -I ${CMAKE_CURRENT_BINARY_DIR})

find_program(MAKEINFO makeinfo)
if (MAKEINFO)
    add_custom_command(COMMAND ${MAKEINFO} --no-split ${TEXI_PROG_ARGS}  ${TEXI_SRC}
                       TARGET  blitz-doc
                       OUTPUTS blitz.info)
endif()
mark_as_advanced(MAKEINFO)

find_program(TEXI2HTML texi2html
             ${CYGWIN_INSTALL_PATH}/bin /bin /usr/bin /usr/local/bin /sbin)
mark_as_advanced(TEXI2HTML)

if (TEXI2HTML)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html)
    add_custom_command(COMMAND ${TEXI2HTML} ${TEXI_PROG_ARGS} --split=chapter ${TEXI_SRC}
                       TARGET  blitz-doc
                       OUTPUTS html)
    install(DIRECTORY html DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

find_program(TEXI2PDF texi2pdf
             ${CYGWIN_INSTALL_PATH}/bin /bin /usr/bin /usr/local/bin /sbin)
mark_as_advanced(TEXI2PDF)

if (TEXI2PDF)
    find_path(TEXINFO_TEX_DIR "texinfo.tex" HINTS "/usr/share/texmf/tex/texinfo/")
    add_custom_command(COMMAND TEXINPUTS=:${TEXINFO_TEX_DIR}:; ${TEXI2PDF} ${TEXI_PROG_ARGS} ${TEXI_SRC}
                       TARGET  blitz-doc
                       OUTPUTS blitz.pdf)
    install(FILES blitz.pdf DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "blitz.html;blitz.pdf;blitz.aux;blitz.cp;blitz.cps;blitz.fn;blitz.fns;blitz.ky;blitz.log;blitz.pg;blitz.toc;blitz.tp;blitz.vr")

foreach(i AUTHORS COPYING COPYING.LESSER COPYRIGHT LEGAL LICENSE NEWS README)
    set(BLITZ_INFORMATION ${BLITZ_INFORMATION} ${CMAKE_SOURCE_DIR}/${i})
endforeach()

# Install files

install(FILES blitz.info DESTINATION ${CMAKE_INSTALL_INFODIR})
install(FILES ${BLITZ_INFORMATION} DESTINATION ${CMAKE_INSTALL_DOCDIR})
