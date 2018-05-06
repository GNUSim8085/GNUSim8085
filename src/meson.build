gnusim8085_sources = [
'8085-asm.c',
'8085.c',
'8085-instructions.c',
'8085-link.c',
'8085-memblock.c',
'asm-ds-symtable.c',
'asm-err-comm.c',
'asm-genobj.c',
'asm-gensym.c',
'asm-id.c',
'asm-id-info.c',
'asm-listing.c',
'asm-source.c',
'asm-token.c',
'bridge.c',
'callbacks.c',
'gui-app.c',
'gui-editor.c',
'gui-input-symbol.c',
'gui-keypad.c',
'gui-list-data.c',
'gui-list-io.c',
'gui-list-memory.c',
'gui-list-message.c',
'gui-list-stack.c',
'gui-view.c',
'interface.c',
'main.c',
'support.c'
]


gtkdep = dependency(gtk_api, version : '>=' + gtk_required)
gtksourceviewdep = dependency(gtksourceview_api, version : '>=' + gtksourceview_required)

gnusim8085_dependencies = [gtkdep, gtksourceviewdep]

if get_option('enable-gio')
  giodep = dependency('gio-2.0')
  gnusim8085_dependencies += [giodep]
  gnusim8085_sources += ['file-op-gio.c']
else
  gnusim8085_sources += ['file-op.c']
endif

executable(
  'gnusim8085',
  gnusim8085_sources,
  include_directories: include_directories('..'),
  dependencies : gnusim8085_dependencies,
  install : true
)
