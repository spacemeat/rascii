import pyke as p

renderer_lib = p.CompileAndArchivePhase({
    'name': 'terminal',
    'sources': [
        'renderer.cpp',
        'color_convert.cpp',
        #'raster.cpp'
    ],
})

test_renderer_app = p.CompileAndLinkToExePhase({
    'name': 'test_renderer',
    'sources': ['renderer_test.cpp'],
    'additional_flags': ['-fext-numeric-literals']
}, renderer_lib)

input_lib = p.CompileAndArchivePhase({
    'name': 'input',
    'sources': [
        'input.cpp',
    ],
})

test_input_app = p.CompileAndLinkToExePhase({
    'name': 'test_input',
    'sources': ['input_test.cpp']
}, input_lib)

terminal_lib = p.CompileAndArchivePhase({
    'name': 'terminal',
    'sources': [
        'terminal.cpp',
    ],
})

test_terminal_app = p.CompileAndLinkToExePhase({
    'name': 'test_terminal',
    'sources': ['terminal_test.cpp']
}, [terminal_lib, renderer_lib, input_lib])

p.get_main_phase().depend_on([test_renderer_app, test_input_app, test_terminal_app])
