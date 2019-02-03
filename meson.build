project('gnusim8085', 'c',
  version : '1.4.1.20180912',
  license : 'GPL-2.0+',
  meson_version : '>=0.41.0'
)
packagename = meson.project_name()
prefix = get_option('prefix')
bindir = get_option('bindir')
datadir = get_option('datadir')
localedir = get_option('localedir')
packagedocdir = join_paths(datadir, 'doc', packagename)
packagepixmapsdir = join_paths(datadir, 'icons', 'hicolor', 'scalable', 'apps')
if get_option('build-macos-app') == true
  packagedocdir = join_paths('Contents', 'SharedSupport')
else
endif
packagehelpdir = join_paths(packagedocdir, 'help')
packagemenudir= join_paths(prefix, datadir, 'gnome', 'apps')
conf_data = configuration_data()
conf_data.set_quoted('PACKAGE', packagename, description : 'Name of package')
conf_data.set_quoted('PACKAGE_NAME', packagename)
conf_data.set_quoted('VERSION', meson.project_version(), description : 'Version number of package')
if get_option('build-macos-app') == true
  conf_data.set_quoted('PACKAGE_DATA_DIR', '../../' + datadir + '/' + packagename)
  conf_data.set_quoted('PACKAGE_DOC_DIR', '../../' + packagedocdir)
  conf_data.set_quoted('PACKAGE_HELP_DIR', '../../' + packagehelpdir, description : 'Directory where help files are installed')
  conf_data.set_quoted('PACKAGE_PIXMAPS_DIR', '../../' + packagepixmapsdir, description : 'Directory where pixmaps files are installed')
  conf_data.set_quoted('LOCALEDIR', '../../' + localedir)
else
  conf_data.set_quoted('PACKAGE_DATA_DIR', prefix + '/' + datadir + '/' + packagename)
  conf_data.set_quoted('PACKAGE_DOC_DIR', join_paths(prefix, packagedocdir))
  conf_data.set_quoted('PACKAGE_HELP_DIR', join_paths(prefix, packagehelpdir), description : 'Directory where help files are installed')
  conf_data.set_quoted('PACKAGE_PIXMAPS_DIR', join_paths(prefix, packagepixmapsdir), description : 'Directory where pixmaps files are installed')
  conf_data.set_quoted('LOCALEDIR', join_paths(prefix, localedir))
endif
conf_data.set_quoted('PACKAGE_MENU_DIR', packagemenudir, description : 'Directory where menu files are installed')
conf_data.set_quoted('PACKAGE_URL', 'http://www.gnusim8085.org/', description : 'Website URL')

i18n = import('i18n')
conf_data.set_quoted('GETTEXT_PACKAGE', packagename)
configure_file(output : 'config.h',
               configuration : conf_data)


if get_option('build-macos-app') == false
desktopconf = configuration_data()
desktopconf.set('PACKAGE_PIXMAPS_DIR', packagepixmapsdir)
configure_file(input : 'GNUSim8085.desktop.in',
               output : 'GNUSim8085.desktop',
               configuration : desktopconf,
               install_dir : join_paths(get_option('datadir'), 'applications'))
endif

gtk_api='gtk+-3.0'
gtksourceview_api='gtksourceview-3.0'
gtk_required='3.10.0'
gtksourceview_required='3.10.0'

if host_machine.system() == 'windows'
installerconf = configuration_data()
installerconf.set('PACKAGE', packagename, description : 'Name of package')
installerconf.set('VERSION', meson.project_version(), description : 'Version number of package')
installerconf.set('GTK_VERSION_REQ', gtk_required, description : 'GTK version required')
installerconf.set('GTKSOURCEVIEW_API', gtksourceview_api, description : 'GtkSourceView API version')
configure_file(input : 'installer.nsi.in',
               output : 'installer.nsi',
               configuration : installerconf)
endif

subdir('po')
subdir('src')
subdir('pixmaps')
subdir('doc')
subdir('data')
if get_option('build-macos-app') == true
  subdir('macos')
endif
