import qbs

Project {
     condition: project.withAutotests
     references: [
         "variable_test/variable_test.qbs",
         "method_test/method_test.qbs",
         "metadata_test/metadata_test.qbs"
     ]
} 
