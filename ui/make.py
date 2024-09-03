import pyke as p

region_lib = p.CompileAndArchivePhase({
    'name': 'region_lib',
    'archive_basename': 'region',
    'group': 'region',
    'sources': [
        'box.cpp',
        'region.cpp'
    ],
})

box_test = p.CompileAndLinkToExePhase({
    'name': 'box_test',
    'group': 'test',
    'sources': ['../test/box-helpers-test.cpp',
                '../test/region-test.cpp',
                '../test/box-test.cpp',
                '../test/test.cpp'],
}, region_lib)

p.get_main_phase().depend_on([box_test])
