import qbs
import qbs.Probes
import qbs.FileInfo
import qbs.Environment

Project {
    property path ide_source_tree: path
    name: "Basic reflection"
    //property bool testsEnabled: Environment.getEnv("TEST")
    qbsSearchPaths: "qbs"
    references: [
        "src/src.qbs",
        "tests/tests.qbs"
    ]
    AutotestRunner {
        Depends { name: "Qt.core" }
    }
}
 
