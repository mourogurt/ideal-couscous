import qbs
import qbs.Probes
import qbs.FileInfo
import qbs.Environment

Project {
    name: "Ideal couscous"
    qbsSearchPaths: "qbs"
    references: [
        "src/src.qbs",
        "tests/tests.qbs"
    ]
    AutotestRunner {
        Depends { name: "Qt.core" }
    }
}
 
