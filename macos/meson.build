# This script heavily depends on package installations from brew.
# It will not work when packages are installed from Fink or MacPorts.

# Create Info.plist file
infoplistconf = configuration_data()
infoplistconf.set('PACKAGE', packagename, description : 'Name of package')
infoplistconf.set('VERSION', meson.project_version(), description : 'Version number of package')
infoplistconf.set('COPYRIGHT_STRING', 'Copyright (C) 2003  Sridhar Ratnakumar &lt;srid@srid.ca&gt;', description : 'Copyright information')
configure_file(input : 'Info.plist.in',
               output : 'Info.plist',
               configuration : infoplistconf,
               install_dir : 'Contents')

# Use dylibbundler to bundle dynamic libraries and modify their lookup path
meson.add_install_script('dylibbundler','-x',join_paths(prefix, bindir, packagename),'-b','-d',join_paths(prefix, 'Contents','libs'),'-cd')
# Below lines added to work around a problem in dylibbundler
message('brew prefix ' + run_command('brew', '--prefix').stdout().strip())
dyliblist = ['libcairo-gobject.2.dylib', 'libcroco-0.6.3.dylib', 'libgio-2.0.0.dylib', 'libgmodule-2.0.0.dylib', 'libgobject-2.0.0.dylib', 'libgthread-2.0.0.dylib', 'libgtk-3.0.dylib', 'libpangocairo-1.0.0.dylib', 'libpangoft2-1.0.0.dylib', 'librsvg-2.2.dylib']
giodir=run_command('pkg-config', 'gio-2.0', '--variable', 'libdir').stdout().strip()
message('gio path ' + giodir)
glibdir=run_command('pkg-config', 'glib-2.0', '--variable', 'libdir').stdout().strip()
message('glib path ' + glibdir)
gobjectdir=run_command('pkg-config', 'gobject-2.0', '--variable', 'libdir').stdout().strip()
message('gobject path ' + gobjectdir)
cairodir=run_command('pkg-config', 'cairo', '--variable', 'libdir').stdout().strip()
message('cairo path ' + cairodir)
cairogobjectdir=run_command('pkg-config', 'cairo-gobject', '--variable', 'libdir').stdout().strip()
message('cairo-gobject path ' + cairogobjectdir)
crocodir=run_command('pkg-config', 'libcroco-0.6', '--variable', 'libdir').stdout().strip()
message('croco path ' + crocodir)
fontconfigdir=run_command('pkg-config', 'fontconfig', '--variable', 'libdir').stdout().strip()
message('fontconfig path ' + fontconfigdir)
freetype2dir=run_command('pkg-config', 'freetype2', '--variable', 'libdir').stdout().strip()
message('freetype2 path ' + freetype2dir)
gtkdir=run_command('pkg-config', 'gtk+-3.0', '--variable', 'libdir').stdout().strip()
message('gtk path ' + gtkdir)
harfbuzzdir=run_command('pkg-config', 'harfbuzz', '--variable', 'libdir').stdout().strip()
message('harfbuzz path ' + harfbuzzdir)
pangocairodir=run_command('pkg-config', 'pangocairo', '--variable', 'libdir').stdout().strip()
message('pangocairo path ' + pangocairodir)
pangodir=run_command('pkg-config', 'pango', '--variable', 'libdir').stdout().strip()
message('pango path ' + pangodir)
pangoft2dir=run_command('pkg-config', 'pangoft2', '--variable', 'libdir').stdout().strip()
message('pangoft2 path ' + pangoft2dir)
pngdir=run_command('pkg-config', 'libpng', '--variable', 'libdir').stdout().strip()
message('png path ' + pngdir)
rsvgdir=run_command('pkg-config', 'librsvg-2.0', '--variable', 'libdir').stdout().strip()
message('rsvg path ' + rsvgdir)
foreach dylib : dyliblist
dylib_path = join_paths(prefix, 'Contents', 'libs', dylib)
if run_command('ls', dylib_path).returncode() == 0
  meson.add_install_script('install_name_tool', '-change', cairodir + '/libcairo.2.dylib', '@executable_path/../libs/libcairo.2.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', crocodir + '/libcroco-0.6.3.dylib', '@executable_path/../libs/libcroco-0.6.3.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', glibdir + '/libglib-2.0.0.dylib', '@executable_path/../libs/libglib-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', glibdir + '/libgmodule-2.0.0.dylib', '@executable_path/../libs/libgmodule-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', glibdir + '/libgobject-2.0.0.dylib', '@executable_path/../libs/libgobject-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', '/usr/local/opt/glib/lib/libglib-2.0.0.dylib', '@executable_path/../libs/libglib-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', '/usr/local/opt/glib/lib/libgmodule-2.0.0.dylib', '@executable_path/../libs/libgmodule-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', '/usr/local/opt/glib/lib/libgobject-2.0.0.dylib', '@executable_path/../libs/libgobject-2.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', gtkdir + '/libgdk-3.0.dylib', '@executable_path/../libs/libgdk-3.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', pangodir + '/libpango-1.0.0.dylib', '@executable_path/../libs/libpango-1.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', pangoft2dir + '/libpangoft2-1.0.0.dylib', '@executable_path/../libs/libpangoft2-1.0.0.dylib', dylib_path)
  meson.add_install_script('install_name_tool', '-change', rsvgdir + '/librsvg-2.2.dylib', '@executable_path/../libs/librsvg-2.2.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', '/usr/local/opt/gettext/lib/libintl.8.dylib', '@executable_path/../libs/libintl.8.dylib', pixbufloader_path)
endif
endforeach

gdk_pixbuf_prefix = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'prefix').stdout().strip()
gdk_pixbuf_version = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_binary_version').stdout().strip()
gdk_pixbuf_binarydir = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_binarydir').stdout().strip()
gdk_pixbuf_moduledir = run_command('pkg-config', 'gdk-pixbuf-2.0', '--variable', 'gdk_pixbuf_moduledir').stdout().strip()
message('gdk_pixbuf_prefix ' + gdk_pixbuf_prefix)
message('gdk_pixbuf_version ' + gdk_pixbuf_version)
message('gdk_pixbuf_binarydir ' + gdk_pixbuf_binarydir)
message('gdk_pixbuf_moduledir ' + gdk_pixbuf_moduledir)
#meson.add_install_script('cp', '-r', join_paths(gdk_pixbuf_prefix, 'lib', 'gdk-pixbuf-2.0', gdk_pixbuf_version), join_paths(prefix, 'Contents', 'libs', 'gdk-pixbuf-2.0', gdk_pixbuf_version))
# Include pixbuf loaders and dependencies
install_subdir(gdk_pixbuf_binarydir, install_dir : join_paths('Contents', 'libs', 'gdk-pixbuf-2.0'))
install_data(join_paths(crocodir, 'libcroco-0.6.3.dylib'), install_dir : join_paths('Contents', 'libs'))
install_data(join_paths(rsvgdir, 'librsvg-2.2.dylib'), install_dir : join_paths('Contents', 'libs'))
#meson.add_install_script('sed', '-i', '-e', 's|'+ gdk_pixbuf_moduledir + '|loaders|g', join_paths(prefix, 'Contents', 'libs', 'gdk-pixbuf-2.0', gdk_pixbuf_version, 'loaders.cache'))
# Modify lookup path on pixbuf loaders
pixbufloaderlist = ['libpixbufloader-svg.so']
foreach pixbufloader : pixbufloaderlist
pixbufloader_path = join_paths(prefix, 'Contents', 'libs', 'gdk-pixbuf-2.0', gdk_pixbuf_version, 'loaders', pixbufloader)
if run_command('ls', pixbufloader_path).returncode() == 0
  meson.add_install_script('install_name_tool', '-change', cairodir + '/libcairo.2.dylib', '@executable_path/../libs/libcairo.2.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', cairogobjectdir + '/libcairo-gobject.2.dylib', '@executable_path/../libs/libcairo-gobject.2.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', crocodir + '/libcroco-0.6.3.dylib', '@executable_path/../libs/libcroco-0.6.3.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', fontconfigdir + '/libfontconfig.1.dylib', '@executable_path/../libs/libfontconfig.1.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', freetype2dir + '/libfreetype.6.dylib', '@executable_path/../libs/libfreetype.6.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', gdkpixbufdir + '/libgdk_pixbuf-2.0.0.dylib', '@executable_path/../libs/libgdk_pixbuf-2.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', giodir + '/libgio-2.0.0.dylib', '@executable_path/../libs/libgio-2.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', glibdir + '/libglib-2.0.0.dylib', '@executable_path/../libs/libglib-2.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', glibdir + '/libgobject-2.0.0.dylib', '@executable_path/../libs/libgobject-2.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', harfbuzzdir + '/libharfbuzz.0.dylib', '@executable_path/../libs/libharfbuzz.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', pangocairodir + '/libpangocairo-1.0.0.dylib', '@executable_path/../libs/libpangocairo-1.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', pangodir + '/libpango-1.0.0.dylib', '@executable_path/../libs/libpango-1.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', pangoft2dir + '/libpangoft2-1.0.0.dylib', '@executable_path/../libs/libpangoft2-1.0.0.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', pngdir + '/libpng16.16.dylib', '@executable_path/../libs/libpng16.16.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', rsvgdir + '/librsvg-2.2.dylib', '@executable_path/../libs/librsvg-2.2.dylib', pixbufloader_path)
  meson.add_install_script('install_name_tool', '-change', '/usr/local/opt/gettext/lib/libintl.8.dylib', '@executable_path/../libs/libintl.8.dylib', pixbufloader_path)
endif
endforeach

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
