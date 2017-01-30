import qbs 1.0;
import qbs.Process;
import qbs.FileInfo;

Product {
    name: "qbs-doxygen";
    Group {
        name: "Doyxgen config";
        files: "Doxyfile";
        fileTags: ["dox"];
    }
}
