# Use dylibbundler to bundle dynamic libraries and modify their lookup path
meson.add_install_script('dylibbundler','-x',join_paths(prefix, bindir, packagename),'-b','-d',join_paths(prefix, 'Contents','libs'),'-cd')

# Copy necessary glib schemas and compile them
meson.add_install_script('mkdir','-p',join_paths(prefix,datadir,'glib-2.0','schemas'))
meson.add_install_script('ls','-R','/usr/local/share/')
meson.add_install_script('ls','-R','/usr/local/opt/glib/')
#meson.add_install_script('cp','/usr/local/opt/glib/share/glib-2.0/schemas/org.gtk.Settings.FileChooser.gschema.xml',join_paths(prefix,datadir,'glib-2.0','schemas'))
#meson.add_install_script('glib-compile-schemas',join_paths(prefix,datadir,'glib-2.0','schemas'))

# Create app bundle for distribution
meson.add_install_script('zip','-r',packagename +'_osx' + '-' + meson.project_version() + '.app.zip', 'GNUSim8085.app')
