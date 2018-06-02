packagename = meson.project_name()
binaryxsltproc = find_program('xsltproc', required : false)
xslfile = get_option('with-xslpath')
inputxml = packagename + '.xml'
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
inputfiles = [inputxml, imagefiles]
outputhtm = packagename + '.htm'

xslfileexists = run_command('ls', xslfile).returncode() == 0
xslprogexists = binaryxsltproc.found()
canbuildhelp = xslfileexists and xslprogexists
message ('can build help : ' + canbuildhelp.to_string())
if canbuildhelp
  custom_target (
  'buildhelp',
  input: inputfiles,
  output : outputhtm,
  command : [binaryxsltproc, '-o', '@OUTPUT0@', xslfile, '@INPUT0@'],
  install : true,
  install_dir : packagehelpdir
  )
  install_data (imagefiles, install_dir : packagehelpdir)
endif
