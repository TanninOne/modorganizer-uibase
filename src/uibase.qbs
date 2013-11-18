import qbs.base 1.0

DynamicLibrary {
    name: 'UIBase'

    Depends { name: "Qt"; submodules: ["core", "gui", "declarative"] }
    Depends { name: 'cpp' }

    cpp.defines: [ '_WINDLL' ]
    cpp.staticLibraries: [ 'user32', 'shell32', 'ole32' ]
    cpp.includePaths: [ qbs.getenv("BOOSTPATH") ]

    files: [
        '*.cpp',
        '*.h',
        '*.ui'
    ]
}
