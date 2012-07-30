#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include <exception>
#include <iostream>
#include <cstdlib>

#include "SampleTest.h"

using std::string;
using std::cout;
using std::endl;

SampleTest::SampleTest() {}

void SampleTest::setUp() {
    m_instance = new string;
}

void SampleTest::tearDown() {
    delete m_instance;
}

void SampleTest::testConcat() {
    *m_instance = "";
    CPPUNIT_ASSERT(*m_instance + "test" == "test");
}

void SampleTest::testException() {
    throw std::exception();
}

void SampleTest::testFailure() {
    cout << "Entering failure!" << endl;
    CPPUNIT_ASSERT(1 == 2);
}

SampleTest::~SampleTest() {}

CPPUNIT_TEST_SUITE_REGISTRATION(SampleTest);

int main() {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
//   CppUnit::TestResult result;
    runner.addTest(registry.makeTest());

    return runner.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
