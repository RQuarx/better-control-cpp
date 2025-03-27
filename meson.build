project(
    'better-control',
    'cpp',
    version: 'i',
    license: 'GPLv3',
    default_options: [
        'cpp_std=c++23'
    ]
)

compile_flags = [
    '-Iinc',
    '-Wall',
    '-Wextra',
    '-O3'
]

source = files(
    'src/main.cpp',
    'src/arg_parse.cpp',
    'src/window.cpp',
    'src/logger.cpp',
    'src/utils.cpp',
    'src/tools.cpp',
)

deps = [
    dependency('gtkmm-3.0')
]

executable(
    'control',
    source,
    include_directories: include_directories('inc'),
    cpp_args: compile_flags,
    dependencies: deps
)