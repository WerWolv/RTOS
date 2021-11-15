project('kernel', [ 'c', 'cpp' ],
  version : '0.1',
  default_options: ['c_std=c17', 'cpp_std=c++20'])

cc = meson.get_compiler('cpp')
assert(cc.get_id() == 'clang', 'This project only support the Clang compiler.')
assert(meson.is_cross_build(), 'This project is supposed to be cross compiled.')

kernel_sources = [
    'source/init/vtor.s',
    'source/init/startup.s',

    'source/main.cpp',

    'source/hw/stm32f7/uart.cpp'
]

includes = include_directories('include')

c_flags = [ '-target', 'arm-none-linux', '-Wall', '-g', '-Og', '-ffreestanding', '-nostdlib', '-mcpu=cortex-m7', '-mthumb', '-mfloat-abi=hard', '-Wno-unused-command-line-argument', '-fno-stack-protector', '-no-pie']
cpp_flags = c_flags + ['-fno-rtti' , '-fomit-frame-pointer',  '-fno-exceptions', '-fno-asynchronous-unwind-tables', '-fno-unwind-tables']
link_args = ['-fuse-ld=lld', '-Wl,-T', meson.current_source_dir() + '/linker.ld', '-target', 'arm-none-linux', '-mfloat-abi=hard', '-ffreestanding', '-nostdlib', '-nodefaultlibs', '-no-pie']

message(meson.current_source_dir())

kernel_elf = executable('kernel', kernel_sources,
  include_directories: includes,
  link_args: link_args,
  c_args: c_flags,
  cpp_args: cpp_flags,
  pie: false
  )

custom_target('kernel.bin',
  build_by_default: true,
  input: kernel_elf,
  output: 'kernel.bin',
  command: [ 'llvm-objcopy', '--input-target=aarch64-none-linux', '--strip-all', '-O', 'binary', kernel_elf, 'kernel.bin' ])