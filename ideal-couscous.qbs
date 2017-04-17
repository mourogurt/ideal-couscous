import qbs
import qbs.Probes
import qbs.FileInfo
import qbs.Environment

Project {
    name: "Ideal couscous"
    qbsSearchPaths: "qbs"
    references: [
        "src/src.qbs",
        "tests/tests.qbs",
        "docs/docs.qbs"
    ]
    property bool withAutotests: qbs.buildVariant === "debug"
    property bool testsEnabled: Environment.getEnv("TEST") || qbs.buildVariant === "debug"
    AutotestRunner {}
}
 
