project('purrooser', 'cpp', version: '0.1.0', default_options: ['buildtype=debug'])

gtk3_dep = dependency('gtk+-3.0')
webkit2gtk_dep = dependency('webkit2gtk-4.0')

src_files = files('src/main.cc')

executable('purrooser', src_files,
  dependencies: [gtk3_dep, webkit2gtk_dep],
  install: true
)

install_subdir('resources', install_dir: get_option('datadir') / 'purrooser')