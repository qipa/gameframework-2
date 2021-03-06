#pragma once
#ifndef __STACK_ALLOCATER_TEST_SUITE_H
#define __STACK_ALLOCATER_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file StackAllocaterTestSuite.h
// @author PJ O Halloran
// @date 01/07/2010
//
// File contains the header for the StackAllocater Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "StackAllocater.h"

// /////////////////////////////////////////////////////////////////
// @class StackAllocaterTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the StackAllocater
// class.
//
// /////////////////////////////////////////////////////////////////
class StackAllocaterTestSuite : public CxxTest::TestSuite {

private:

    static const GameHalloran::U32 m_defaultSize = (1024 * 4);  ///< Initial size of the stack (4 MB).
    GameHalloran::StackAllocater m_testObj;                     ///< Pointer to the object under test.
    GameHalloran::U32 m_padding;

    // /////////////////////////////////////////////////////////////////
    // Check if the test data is created and valid.
    //
    // /////////////////////////////////////////////////////////////////
    bool IsTestDataReady() {
        //return (m_testObjPtr != NULL && m_testObjPtr->IsValid());
        return (true);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    StackAllocaterTestSuite() : m_testObj(m_defaultSize) {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~StackAllocaterTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testObj.Clear();
#ifdef _DEBUG
        m_padding = 8;
#else
        m_padding = 0;
#endif
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        GameHalloran::U32 s(100000);
        GameHalloran::StackAllocater obj(s);
        TS_ASSERT(obj.IsValid());
        TS_ASSERT_EQUALS(obj.GetAvailableMemory(), s);
        TS_ASSERT_EQUALS(obj.GetEndMarker(), s);
        TS_ASSERT_EQUALS(obj.GetMarker(), 0);

        void *blockA = obj.Alloc(10);
        TS_ASSERT(blockA != NULL);
        TS_ASSERT_EQUALS(obj.GetMarker(), 10 + m_padding);
        TS_ASSERT_EQUALS(obj.GetEndMarker(), s);
        TS_ASSERT_EQUALS(obj.GetAvailableMemory(), s - 10 - m_padding);

        void *blockB = obj.AllocEnd(25);
        TS_ASSERT(blockB != NULL);
        TS_ASSERT_EQUALS(obj.GetMarker(), 10 + m_padding);
        TS_ASSERT_EQUALS(obj.GetEndMarker(), s - 25 - m_padding);
        TS_ASSERT_EQUALS(obj.GetAvailableMemory(), s - 10 - 25 - (m_padding * 2));
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testIsValid(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetSize(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testAlloc(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetMarker(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testAllocEnd(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetEndMarker(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testFreeToMarker(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testClear(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetAvailableMemory(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

    };

//#ifdef _DEBUG
//
//  // ////////////////////////////////////////////////////////////
//  //
//  // ////////////////////////////////////////////////////////////
//  void testVerifyBlock(void)
//  {
//      if(!IsTestDataReady())
//      {
//          TS_FAIL("Test data not created.");
//          return;
//      }
//
//  };
//
//  // ////////////////////////////////////////////////////////////
//  //
//  // ////////////////////////////////////////////////////////////
//  void testVerifyStack(void)
//  {
//      if(!IsTestDataReady())
//      {
//          TS_FAIL("Test data not created.");
//          return;
//      }
//
//  };
//
//  // ////////////////////////////////////////////////////////////
//  //
//  // ////////////////////////////////////////////////////////////
//  void testGetNumberBlocksAllocated(void)
//  {
//      if(!IsTestDataReady())
//      {
//          TS_FAIL("Test data not created.");
//          return;
//      }
//
//  };
//
//#endif

};

#endif
