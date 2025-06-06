r_io_sources = [
  'io.c',
  'io_fd.c',
  'io_bank.c',
  'io_map.c',
  'io_memory.c',
  'io_cache.c',
  'io_desc.c',
  'io_plugin.c',
  'io_stream.c',
  'io_bank.c',
  'io_submap.c',
  'ioutils.c',
  'undo.c',
  'p_cache.c',
  'p/io_ar.c',
  'p/io_reg.c',
  'p/io_fd.c',
  'p/io_bfdbg.c',
  'p/io_bochs.c',
  'p/io_socket.c',
  'p/io_debug.c',
  'p/io_default.c',
  'p/io_sysgdb.c',
  'p/io_dsc.c',
  'p/io_gdb.c',
  'p/io_gprobe.c',
  'p/io_gzip.c',
  'p/io_http.c',
  'p/io_ihex.c',
  'p/io_mach.c',
  'p/io_malloc.c',
  'p/io_cyclic.c',
  'p/io_xattr.c',
  'p/io_xalz.c',
  'p/io_mmap.c',
  'p/io_null.c',
  'p/io_procpid.c',
  'p/io_ptrace.c',
  'p/io_qnx.c',
  'p/io_r2k.c',
  'p/io_r2pipe.c',
  'p/io_r2web.c',
  'p/io_rap.c',
  'p/io_rbuf.c',
  'p/io_self.c',
  'p/io_shm.c',
  'p/io_sparse.c',
  'p/io_tcpslurp.c',
  'p/io_uf2.c',
  'p/io_winkd.c',
  'p/io_winedbg.c',
  'p/io_zip.c',
]

if host_machine.system() == 'windows'
  r_io_sources += [
    'p/io_r2k_windows.c',
    'p/io_windbg.c',
    'p/io_w32.c',
    'p/io_w32dbg.c',
  ]
endif

r_io_deps = [
  r_util_dep,
  r_socket_dep,
  r_cons_dep,
  r_muta_dep,
  bochs_dep,
  gdb_dep,
  winkd_dep,
  qnx_dep,
  zip_dep,
  ar_dep,
  pth
]

if host_machine.system() == 'haiku'
  r_io_deps += [
    cc.find_library('network')
  ]
endif

if host_machine.system() == 'linux' or host_machine.system() == 'android'
  r_io_sources += [
    'p/io_r2k_linux.c',
    'p/io_isotp.c',
  ]
  ## only for shm_open
  # r_io_deps += meson.get_compiler('c').find_library('rt')
endif

if use_ptrace_wrap
  r_io_sources += 'ptrace_wrap.c'
endif

r_io = library('r_io', r_io_sources,
  include_directories: platform_inc,
  link_with: [r_util, r_cons, r_muta, r_socket],
  dependencies: r_io_deps,
  c_args: library_cflags,
  install: true,
  implicit_include_directories: false,
  install_rpath: rpath_lib,
  soversion: r2_libversion
)

r_io_dep = declare_dependency(link_with: [r_io, r_muta],
  include_directories: platform_inc)

if get_option('blob')
r_io_static_deps = [
  r_util_static_dep,
  r_cons_static_dep,
  r_muta_static_dep,
  r_socket_static_dep,
  winkd_static_dep,
  bochs_static_dep,
  gdb_static_dep,
  qnx_static_dep,
  ar_static_dep,
  zip_dep,
  pth
]

r_io_static = static_library('r_io_static', r_io_sources,
  include_directories: platform_inc,
  link_with: [r_util_static, r_muta_static, r_cons_static, r_socket_static],
  dependencies: r_io_static_deps,
  c_args: library_cflags,
  install: true,
  implicit_include_directories: false,
)

r_io_static_dep = declare_dependency(link_with: r_io_static,
  include_directories: platform_inc)
endif

pkgconfig_mod.generate(r_io,
  subdirs: 'libr',
  version: r2_version,
  name: 'r_io',
  filebase: 'r_io',
  libraries: pkgcfg_sanitize_libs,
  requires: [
    'r_util',
    'r_cons',
    'r_muta',
    'r_socket'
  ],
  description: 'radare foundation libraries'
)
