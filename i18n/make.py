import pyke as p

humon_repo = p.ExternalRepoPhase({
    'name': 'humon_repo',
    'package_name': 'humon',
    'repo_name': 'spacemeat/humon',
    'repo_version': 'v0.2.3',
    'using_pyke_makefile': 'project',
})

humon = p.PykeRepoPhase({
    'name': 'humon',
    'use_deps': ['static_lib.humon_archive'],
}, humon_repo)


fmt_repo = p.ExternalRepoPhase({
    'name': 'fmt_repo',
    'package_name': 'fmt',
    'repo_name': 'fmtlib/fmt',
    'repo_version': '11.0.1',
    'using_cmake_makefile': 'project',
})

fmt = p.CMakeRepoPhase({
    'name': 'fmt',
    'lib_kind': 'archive',   # shared_object
    'archive_arg': '',
    'shared_object_arg': ' -DBUILD_SHARED_LIBS=TRUE',
    'cmake_args': '{{lib_kind}_arg} -DFMT_TEST=FALSE',
    'makes': {'fmt': '{lib_kind}'},
}, fmt_repo)

i18n_lib = p.CompileAndArchivePhase({
    'name': 'static_lib',
    'group': 'i18n',
    'archive_basename': 'i18n',
    'include_dirs': ['include', 'external/humon/include'],
    'sources': [
        'i18n.cpp',
    ],
})

test = p.CompileAndLinkToExePhase({
    'name': 'test_demo',
    'group': 'i18n',
    'src_dir': 'test-demo',
    'include_dirs': ['include',
                     'external/humon/include',
                     'external/fmt/include'],
    'sources': ['i18n_test.cpp'],
}, [humon, fmt, i18n_lib])


p.get_main_phase().depend_on([test])
