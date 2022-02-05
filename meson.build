project(
	'ez_paint',
	'cpp',
	default_options: ['cpp_std=c++20', 'build.cpp_std=c++20']
)

deps = [
	dependency('Threads'),
	dependency('imgui'),
	dependency('implot'),
	dependency('ImGui-SFML'),
	dependency('PNG'),
	dependency('fmt'),
	dependency('DataFrame'),
	dependency('RapidJSON'),
	dependency('Backward'),
	dependency('Catch2'),
	dependency('libheif')
]

includes = include_directories('include')
src = [
	'src/app.cpp',
	'src/brush.cpp',
	'src/canvas.cpp',
	'src/collision.cpp',
	'src/exporter.cpp',
	'src/image.cpp',
	'src/tree.cpp',
	'src/utils.cpp',
	'src/main.cpp'
]

cpp_args = [
	'-0fast',
	'-fpermissive',
	'-Wall',
	'-Wno-switch',
	'-Wno-unused-variable',
	'-Wno-sign-conversion',
	'-Wno-unused-function',
	'-pedantic-errors',
]

lib = shared_library(
	meson.project_name(),
	src,
	dependencies: deps,
	version: '1.0',
	soversion: '0',
	include_directories: includes,
	cpp_args: cpp_args
)

exe = executable(
	meson.project_name(),
	'src/main.cpp',
	dependencies: deps,
	include_directories: includes,
	link_with: lib,
	cpp_args: cpp_args
)
test('unittests', exe)