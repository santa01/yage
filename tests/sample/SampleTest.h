#ifndef SAMPLETEST_H
#define SAMPLETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include <string>

class SampleTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SampleTest);

    CPPUNIT_TEST(testConcat);
    CPPUNIT_TEST_EXCEPTION(testException, std::exception);
//     CPPUNIT_TEST(testFailure);

    CPPUNIT_TEST_SUITE_END();
private:
    std::string *m_instance;

public:
    explicit SampleTest();

    void setUp();
    void tearDown();

    void testConcat();
    void testException();
    void testFailure();

    virtual ~SampleTest();
};

#endif // SAMPLETEST_H
