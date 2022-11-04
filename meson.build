project('twang',
    'cpp',
    version: '0.0.1',
    license: 'GPLv2+',
    default_options: [
        'cpp_std=c++17'
    ]
)

cc = meson.get_compiler('cpp')

flex = find_program('flex', required: true)
bison = find_program('bison', required: true)

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())

configure_file(
    output: 'config.h',
    configuration: conf
)

lexer_gen = generator(flex,
    output: '@BASENAME@.yy.cc',
    arguments: ['-o', '@OUTPUT@', '@INPUT@']
)
parser_gen = generator(bison,
    output: ['@BASENAME@.tab.cc', '@BASENAME@.tab.hh'],
    arguments: ['--output=@OUTPUT0@', '--defines=@OUTPUT1@', '@INPUT@']
)

lexer = lexer_gen.process('twang.l')
parser = parser_gen.process('twang.y')

readline_dep = dependency('readline')

bin = executable('twang',
    'main.cpp',
    'reader.cpp',
    lexer, parser,
    dependencies: [readline_dep]
)
