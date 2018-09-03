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
meson.add_install_script('zip','-r',packagename +'_osx' + '-' + meson.project_version() + '.app.zip', 'GNUSim8085.app')
