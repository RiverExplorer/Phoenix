/**
 * Project: Phoenix
 * Time-stamp: <2025-03-01 13:42:52 doug>
 *
 * @file TestClient1.hpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#ifndef _RIVEREXPLORER_PHOENIX_TESTS_TEST_CLIENT1_HPP_
#define _RIVEREXPLORER_PHOENIX_TESTS_TEST_CLIENT1_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>

// Test class definition
class ClientTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ClientTest);
	CPPUNIT_TEST(GetServerListTest);
	CPPUNIT_TEST(DeleteServerTest1);
	CPPUNIT_TEST(AddServerTest);
	CPPUNIT_TEST(DeleteServerTest2);
	CPPUNIT_TEST(AddServerTest);
	CPPUNIT_TEST(ClientConstructorTest);
	CPPUNIT_TEST(CreateCapabilityVendorIDTest);
	CPPUNIT_TEST(CreateCapabilityPreTest);
	CPPUNIT_TEST(CreateCapabilityVendorExtension1Test);
	CPPUNIT_TEST(ConnectToServerTest);
	CPPUNIT_TEST_SUITE_END();

private:
	std::string TestHost;
	uint16_t		TestPort;
	
public:
    // Setup method - called before each test
	void setUp();

	// Teardown method - called after each test
	void tearDown();

	// Test methods
	//
	void GetServerListTest();
	void DeleteServerTest1();
	void DeleteServerTest2();
	void DeleteServerTest(size_t Number);
	void AddServerTest();
	void ClientConstructorTest();
	void CreateCapabilityVendorIDTest();
	void CreateCapabilityPreTest();
	void CreateCapabilityVendorExtension1Test();
	
	void ConnectToServerTest();
};
// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ClientTest);

#endif // _RIVEREXPLORER_PHOENIX_TESTS_TEST_CLIENT1_HPP_
