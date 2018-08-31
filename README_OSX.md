GNUSim8085 depends on other open source applications/libraries which are not
available on app store. You can install these dependencies from repositories
Fink, MacPorts or Brew.

## Dependencies

Build dependencies (Mandatory)
- GTK3 (>= 3.10.0) development package
- GtkSourceView3 (>= 3.10.0) development package
- meson OR autoconf and automake

Build dependencies (Optional)
- gettext (>= 0.18)
- markdown (discount or similar package that provides markdown binary)

## Install from source using meson build system (default)
To install from source, run the following commands

```sh
$ meson builddir
$ cd builddir
$ ninja
$ sudo ninja install
```

Note: If you are using brew to install dependencies then simply run command
'brew bundle' from the source root. This will fetch all the necessary packages.

## Install from source using autotools build system
To install from source, run the following commands

```sh
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

Note: If you are using brew to install dependencies then run command
'brew bundle' followed by 'brew install autoconf' and 'brew install automake'
This will fetch all the necessary packages.
