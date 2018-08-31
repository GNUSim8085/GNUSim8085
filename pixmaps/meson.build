if host_machine.system() == 'windows'
install_data('gnusim8085.ico', install_dir : join_paths(get_option('datadir'), 'pixmaps', 'gnusim8085'))
else
install_data('gnusim8085.svg', install_dir : join_paths(get_option('datadir'), 'icons/hicolor/scalable/apps'))
endif

# Create icns file for OS X bundle
if get_option('build-osx-app') == true
meson.add_install_script('cp', join_paths(meson.source_root(), 'pixmaps', 'gnusim8085.png'), join_paths(prefix, datadir, packagename + '.iconset', 'icon_512x512.png'))
meson.add_install_script('iconutil', '-c', 'icns', '-o', join_paths(prefix, datadir, packagename + '.icns'), join_paths(prefix, datadir, packagename + '.iconset'))
meson.add_install_script('rm', '-rf', join_paths(prefix, datadir, packagename + '.iconset'))
endif
