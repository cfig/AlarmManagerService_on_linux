project('AOSP system core', 'c', 'cpp', default_options : ['cpp_std=c++17'])
fs = import('fs')

#--------------------------------------------------
#                     costants
#--------------------------------------------------
jni_root = 'jni/'
java_headers = [
    fs.expanduser('~/.sdkman/candidates/java/current/include'),
    fs.expanduser('~/.sdkman/candidates/java/current/include/linux'),
]
external_aosp_root = fs.expanduser('~/android/a')

loose_deps_lib_dir = meson.source_root()+ '/system_core/build'
loose_deps_header_dir = external_aosp_root + '/system/core/libutils/include'
liblog_so_dep = meson.get_compiler('cpp').find_library('log', dirs: loose_deps_lib_dir, required: true)
libbase_so_dep = meson.get_compiler('cpp').find_library('base', dirs: loose_deps_lib_dir, required: true)
#--------------------------------------------------
#                     libsystem_clock_so
#--------------------------------------------------
libsystem_clock_sources = [
  jni_root + 'android_os_SystemClock_Companion.cpp',
]

# deps 1
loose_deps = declare_dependency(
    link_args : ['-L' + loose_deps_lib_dir, '-l' + 'utils'],
    include_directories : include_directories(loose_deps_header_dir))

# deps 2
libsystem_clock_deps = meson.get_compiler('cpp').find_library('utils', dirs: loose_deps_lib_dir, required: true)

libsystem_clock_so = library(
  'system_clock',
  libsystem_clock_sources,
  include_directories: [
    jni_root + 'include',
    external_aosp_root + '/system/core/include',
  ] + java_headers,
  #dependencies : [loose_deps],
  dependencies : [libsystem_clock_deps],
)

#--------------------------------------------------
#                    libalarm_manager.so 
#--------------------------------------------------
libalarm_manager_sources = [
  jni_root + 'com_android_server_AlarmManagerService_Companion.cpp',
]

libalarm_manager_so = library(
  'alarm_manager',
  libalarm_manager_sources,
  include_directories: [
    jni_root + 'include',
    external_aosp_root + '/system/core/include',
  ] + java_headers,
  cpp_args : ['-Wno-writable-strings'],
  dependencies : [libsystem_clock_deps, liblog_so_dep],
)


#--------------------------------------------------
#                    libcfig_ahelper.so 
#--------------------------------------------------
libcfig_ahelper_sources = [
  jni_root + 'cfig_AHelper_Companion.cpp',
]

libcfig_ahelper_so = library(
  'cfig_ahelper',
  libcfig_ahelper_sources,
  include_directories: [
    jni_root + 'include',
    external_aosp_root + '/system/core/base/include',
  ] + java_headers,
  cpp_args : ['-Wno-writable-strings'],
  dependencies : [libbase_so_dep],
)

