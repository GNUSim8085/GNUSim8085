GNUSim8085 depends on other open source applications/libraries which are not
available on app store. You can install these dependencies from repositories
Fink, MacPorts or Brew.

## Dependencies

Build dependencies (Mandatory)
- GTK3 (>= 3.10.0) development package
- GtkSourceView3 (>= 3.10.0) development package

Build dependencies (Optional)
- gettext (>= 0.18)
- markdown (discount or similar package that provides markdown binary)

## Install from source
To install from source, run the following commands

```sh
$ ./autogen.sh # only required when configure script does not exist
$ ./configure
$ make
$ sudo make install
```

Note: If you are using brew to install dependencies then simply run command
brew bundle from the source root. This will fetch all the necessary packages.
