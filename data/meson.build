install_data('8085asm.lang', install_dir : join_paths(get_option('datadir'), meson.project_name()))
if get_option('build-macos-app') == false
install_data('gnusim8085.appdata.xml', install_dir : join_paths(get_option('datadir'), 'metainfo'))
endif
if get_option('build-macos-app') == true
install_data('/usr/local/share/gtksourceview-3.0/language-specs/language2.rng', install_dir : join_paths(get_option('datadir'), meson.project_name()))
install_data('/usr/local/share/gtksourceview-3.0/language-specs/def.lang', install_dir : join_paths(get_option('datadir'), meson.project_name()))
install_data('/usr/local/share/gtksourceview-3.0/styles/styles.rng', install_dir : join_paths(get_option('datadir'), meson.project_name()))
install_data('/usr/local/share/gtksourceview-3.0/styles/classic.xml', install_dir : join_paths(get_option('datadir'), meson.project_name()))
endif
