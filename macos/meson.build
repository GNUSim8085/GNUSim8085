# This script heavily depends on package installations from brew.
# It will not work when packages are installed from Fink or MacPorts.

# Create Info.plist file
infoplistconf = configuration_data()
infoplistconf.set('PACKAGE', packagename, description : 'Name of package')
infoplistconf.set('VERSION', meson.project_version(), description : 'Version number of package')
infoplistconf.set('COPYRIGHT_STRING', 'Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>', description : 'Copyright information')
configure_file(input : 'Info.plist.in',
               output : 'Info.plist',
               configuration : infoplistconf,
               install_dir : 'Contents')

# Use dylibbundler to bundle dynamic libraries and modify their lookup path
meson.add_install_script('dylibbundler','-x',join_paths(prefix, bindir, packagename),'-b','-d',join_paths(prefix, 'Contents','libs'),'-cd')
# Below lines added to work around a problem in dylibbundler
message('brew prefix ' + run_command('brew', '--prefix').stdout().strip())
dyliblist = ['libcairo-gobject.2.dylib', 'libgio-2.0.0.dylib', 'libgmodule-2.0.0.dylib', 'libgobject-2.0.0.dylib', 'libgthread-2.0.0.dylib', 'libgtk-3.0.dylib', 'libpangocairo-1.0.0.dylib', 'libpangoft2-1.0.0.dylib']
glibdir=run_command('pkg-config', 'glib-2.0', '--variable', 'libdir').stdout().strip()
message('glib path ' + glibdir)
cairodir=run_command('pkg-config', 'cairo', '--variable', 'libdir').stdout().strip()
message('cairo path ' + cairodir)
gtkdir=run_command('pkg-config', 'gtk+-3.0', '--variable', 'libdir').stdout().strip()
message('gtk path ' + gtkdir)
foreach dylib : dyliblist
meson.add_install_script('install_name_tool', '-change', '/usr/local/Cellar/cairo/1.14.12/lib/libcairo.2.dylib', '@executable_path/../libs/libcairo.2.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
meson.add_install_script('install_name_tool', '-change', glibdir + '/libglib-2.0.0.dylib', '@executable_path/../libs/libglib-2.0.0.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
meson.add_install_script('install_name_tool', '-change', glibdir + '/libgmodule-2.0.0.dylib', '@executable_path/../libs/libgmodule-2.0.0.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
meson.add_install_script('install_name_tool', '-change', glibdir + '/libgobject-2.0.0.dylib', '@executable_path/../libs/libgobject-2.0.0.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
meson.add_install_script('install_name_tool', '-change', '/usr/local/Cellar/gtk+3/3.22.30/lib/libgdk-3.0.dylib', '@executable_path/../libs/libgdk-3.0.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
meson.add_install_script('install_name_tool', '-change', '/usr/local/Cellar/pango/1.42.1/lib/libpango-1.0.0.dylib', '@executable_path/../libs/libpango-1.0.0.dylib', join_paths(prefix, 'Contents', 'libs', dylib))
endforeach

gdk_pixbuf_prefix = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'prefix').stdout().strip()
gdk_pixbuf_version = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_binary_version').stdout().strip()
gdk_pixbuf_binarydir = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_binarydir').stdout().strip()
gdk_pixbuf_moduledir = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_moduledir').stdout().strip()
message('gdk_pixbuf_prefix ' + gdk_pixbuf_prefix)
message('gdk_pixbuf_version ' + gdk_pixbuf_version)
message('gdk_pixbuf_binarydir ' + gdk_pixbuf_binarydir)
message('gdk_pixbuf_moduledir ' + gdk_pixbuf_moduledir)
meson.add_install_script('cp', '-r', gdk_pixbuf_binarydir, join_paths(prefix, 'Contents', 'libs', 'gdk-pixbuf-2.0', gdk_pixbuf_version))
install_subdir(gdk_pixbuf_binarydir, install_dir : join_paths('Contents', 'libs', 'gdk-pixbuf-2.0'))
meson.add_install_script('sed', '-i', '-e', 's|'+ gdk_pixbuf_moduledir + '|loaders|g', join_paths(prefix, 'Contents', 'libs', 'gdk-pixbuf-2.0', gdk_pixbuf_version, 'loaders.cache'))

# Copy necessary glib schemas and compile them
meson.add_install_script('mkdir', '-p', join_paths(prefix, datadir, 'glib-2.0', 'schemas'))
#meson.add_install_script('ls','-R','/usr/local/share/')
#meson.add_install_script('ls','-R','/usr/local/opt/')
install_data('/usr/local/share/glib-2.0/schemas/org.gtk.Settings.FileChooser.gschema.xml', install_dir : join_paths(datadir, 'glib-2.0', 'schemas'))
meson.add_install_script('glib-compile-schemas', join_paths(prefix, datadir, 'glib-2.0', 'schemas'))
meson.add_install_script('mkdir', '-p', join_paths(prefix, datadir, 'icons', 'Adwaita'))
meson.add_install_script('cp', '-r', '/usr/local/share/icons/Adwaita/', join_paths(prefix, datadir, 'icons', 'Adwaita'))
#meson.add_install_script('ls', join_paths(prefix, datadir, 'icons', 'Adwaita'))
meson.add_install_script('gtk3-update-icon-cache', '-t', join_paths(prefix, datadir, 'icons', 'Adwaita'))

# Install launcher script
install_data('gnusim8085.sh', install_dir : bindir)

# Create app bundle for distribution
meson.add_install_script('zip','-r',packagename +'_macos' + '-' + meson.project_version() + '.app.zip', 'GNUSim8085.app')
