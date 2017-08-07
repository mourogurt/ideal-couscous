import qbs

Project {
    minimumQbsVersion: "1.6.0"
    CppApplication {
        type: ["application", "autotest"]
        consoleApplication: true
        files: ["*.cpp","*.hpp"]
        cpp.cxxLanguageVersion: "c++1z"
        cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL']
        Depends { name: "Basic Reflection" }
        Depends { name: "Qt.test" }
        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }
    }
}
