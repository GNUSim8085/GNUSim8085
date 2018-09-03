extra_args = [
 '--directory=' + meson.source_root(),
 '--copyright-holder=Sridhar Ratnakumar',
 '--package-version=' + meson.project_version(),
 '--msgid-bugs-address=https://github.com/GNUSim8085/GNUSim8085/issues'
]

i18n.gettext(meson.project_name(),
    args: extra_args,
    preset: 'glib'
)

# Following block assumes that packages are installed from brew.
# It will not work when packages are installed from Fink or MacPorts.
if get_option('build-macos-app') == true
  # Bundle GTK translations based on the languages we have
  read_languages = run_command ('tail', '-1', 'LINGUAS')
  if read_languages.returncode() == 0
    languages = read_languages.stdout().strip().split()
  endif
  foreach l : languages
    gtk_translation = join_paths('/usr', 'local', 'share', 'locale', l, 'LC_MESSAGES', 'gtk30.mo')
    if run_command('ls', gtk_translation).returncode() == 0
      install_data(gtk_translation, install_dir : join_paths(localedir, l, 'LC_MESSAGES'))
    endif
  endforeach
endif
