packagename = meson.project_name()
binarymarkdown = find_program('markdown', required : false)
inputmd = 'userguide.md'
imagefiles = [
'assembler_listing.png',
'assembler_messages.png',
'editor.png',
'io.png',
'registers_flag.png',
'tab_data.png',
'tab_io.png',
'tab_keypad.png',
'tab_memory.png',
'tab_stack.png',
'toolbar.png'
]
inputfiles = [inputmd, imagefiles]
outputhtm = 'userguide.htm'

canbuildhelp = binarymarkdown.found()
message ('can build help : ' + canbuildhelp.to_string())
if canbuildhelp
  custom_target (
  'buildhelp',
  input: inputfiles,
  output : outputhtm,
  command : [binarymarkdown, '-o', '@OUTPUT0@', '@INPUT0@'],
  install : true,
  install_dir : packagehelpdir
  )
  install_data (imagefiles, install_dir : packagehelpdir)
endif
