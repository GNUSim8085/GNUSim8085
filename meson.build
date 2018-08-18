project('gnusim8085', 'c',
  version : '1.4.2',
  license : 'GPL-2.0+',
  meson_version : '>=0.37'
)
packagename = meson.project_name()
prefix = get_option('prefix')
bindir = get_option('bindir')
datadir = get_option('datadir')
localedir = get_option('localedir')
if get_option('build-osx-app')
  packagedocdir = join_paths(prefix, 'Contents', 'SharedSupport')
else
  packagedocdir = join_paths(prefix, datadir, 'doc', packagename)
endif
packagehelpdir = join_paths(packagedocdir, 'help')
packagepixmapsdir = prefix + '/' + datadir + '/icons/hicolor/scalable/apps'
packagemenudir= join_paths(prefix, datadir, 'gnome', 'apps')
conf_data = configuration_data()
conf_data.set_quoted('PACKAGE', packagename, description : 'Name of package')
conf_data.set_quoted('PACKAGE_NAME', packagename)
conf_data.set_quoted('VERSION', meson.project_version(), description : 'Version number of package')
conf_data.set_quoted('PACKAGE_DATA_DIR', prefix + '/' + datadir + '/' + packagename)
if get_option('build-osx-app')
  conf_data.set_quoted('PACKAGE_DOC_DIR', '../' + 'SharedSupport')
  conf_data.set_quoted('PACKAGE_HELP_DIR', '../' + 'SharedSupport/' + 'help', description : 'Directory where help files are installed')
else
  conf_data.set_quoted('PACKAGE_DOC_DIR', packagedocdir)
  conf_data.set_quoted('PACKAGE_HELP_DIR', packagehelpdir, description : 'Directory where help files are installed')
endif
conf_data.set_quoted('PACKAGE_MENU_DIR', packagemenudir, description : 'Directory where menu files are installed')
conf_data.set_quoted('PACKAGE_PIXMAPS_DIR', packagepixmapsdir, description : 'Directory where pixmaps files are installed')
conf_data.set_quoted('PACKAGE_URL', 'http://www.gnusim8085.org/', description : 'Website URL')

i18n = import('i18n')
conf_data.set_quoted('GETTEXT_PACKAGE', packagename)
conf_data.set_quoted('LOCALEDIR', join_paths(prefix, localedir))
configure_file(output : 'config.h',
               configuration : conf_data)


if get_option('build-osx-app') == false
desktopconf = configuration_data()
desktopconf.set('PACKAGE_PIXMAPS_DIR', packagepixmapsdir)
configure_file(input : 'GNUSim8085.desktop.in',
               output : 'GNUSim8085.desktop',
               configuration : desktopconf,
               install_dir : join_paths(get_option('datadir'), 'applications'))
endif

installerconf = configuration_data()
installerconf.set('PACKAGE', packagename, description : 'Name of package')
installerconf.set('VERSION', meson.project_version(), description : 'Version number of package')
gtk_api='gtk+-3.0'
gtksourceview_api='gtksourceview-3.0'
gtk_required='3.10.0'
gtksourceview_required='3.10.0'
installerconf.set('GTK_VERSION_REQ', gtk_required, description : 'GTK version required')
installerconf.set('GTKSOURCEVIEW_API', gtksourceview_api, description : 'GtkSourceView API version')
configure_file(input : 'installer.nsi.in',
               output : 'installer.nsi',
               configuration : installerconf)

subdir('po')
subdir('src')
subdir('pixmaps')
subdir('doc')
subdir('data')
if get_option('build-osx-app') == true
  subdir('osx')
endif
