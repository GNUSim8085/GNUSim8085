project('gnusim8085', 'c',
  version : '1.3.8',
  license : 'GPL-2.0+',
  meson_version : '>=0.42'
)
packagename = meson.project_name()
prefix = get_option('prefix')
datadir = get_option('datadir')
packagedocdir = join_paths(prefix, datadir, 'doc', packagename)
packagehelpdir = join_paths(packagedocdir, 'help')
packagepixmapsdir = prefix + '/' + datadir + '/icons/hicolor/scalable/apps'
packagemenudir= join_paths(prefix, datadir, 'gnome', 'apps')
conf_data = configuration_data()
conf_data.set_quoted('PACKAGE', packagename, description : 'Name of package')
conf_data.set_quoted('PACKAGE_NAME', packagename)
conf_data.set_quoted('VERSION', meson.project_version(), description : 'Version number of package')
conf_data.set_quoted('PACKAGE_DATA_DIR', prefix + '/' + datadir + '/' + packagename)
conf_data.set_quoted('PACKAGE_DOC_DIR', packagedocdir)
conf_data.set_quoted('PACKAGE_HELP_DIR', packagehelpdir, description : 'Directory where help files are installed')
conf_data.set_quoted('PACKAGE_MENU_DIR', packagemenudir, description : 'Directory where menu files are installed')
conf_data.set_quoted('PACKAGE_PIXMAPS_DIR', packagepixmapsdir, description : 'Directory where pixmaps files are installed')
conf_data.set_quoted('PACKAGE_URL', 'http://www.gnusim8085.org/', description : 'Website URL')

i18n = import('i18n')
conf_data.set_quoted('GETTEXT_PACKAGE', packagename)
conf_data.set_quoted('LOCALEDIR',
  join_paths(get_option('prefix'), get_option('localedir')))
configure_file(output : 'config.h',
               configuration : conf_data)


desktopconf = configuration_data()
desktopconf.set('PACKAGE_PIXMAPS_DIR', packagepixmapsdir)
configure_file(input : 'GNUSim8085.desktop.in',
               output : 'GNUSim8085.desktop',
               configuration : desktopconf)

installerconf = configuration_data()
installerconf.set('PACKAGE', packagename, description : 'Name of package')
installerconf.set('VERSION', meson.project_version(), description : 'Version number of package')
if get_option('with-gtk2')
gtk_api = 'gtk+-2.0'
gtksourceview_api='gtksourceview-2.0'
gtk_required='2.24.0'
gtksourceview_required='2.10.0'
else
gtk_api='gtk+-3.0'
gtksourceview_api='gtksourceview-3.0'
gtk_required='3.10.0'
gtksourceview_required='3.10.0'
endif
installerconf.set('GTK_VERSION_REQ', gtk_required, description : 'Name of package')
installerconf.set('GTKSOURCEVIEW_API', gtksourceview_api, description : 'Name of package')
configure_file(input : 'installer.nsi.in',
               output : 'installer.nsi',
               configuration : installerconf)

subdir('po')
subdir('src')
subdir('pixmaps')
subdir('doc')
subdir('data')

install_data('GNUSim8085.desktop', install_dir : join_paths(get_option('datadir'), 'applications'))
