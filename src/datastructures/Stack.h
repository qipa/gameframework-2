#pragma once
#ifndef _GF_STACK_H
#define _GF_STACK_H

// ////////////////////////////////////////////////////////////
// @file Stack.h
// @author PJ O Halloran
// @date 30/03/2011
//
// Header for the template Stack container class(es).
//
// ////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "LinkedList.h"

// ////////////////////////////////////////////////////////////
//
//
// ////////////////////////////////////////////////////////////
namespace GameHalloran {

    // ////////////////////////////////////////////////////////////
    // @class Stack
    // @author PJ O Halloran
    //
    // A Stack (LIFO last in first out) data structure.
    //
    // The underlying implementation uses a LinkedList.  Therefore
    // pushing and popping elements from the top of the Stack is
    // an operation of the order O(1).
    //
    // ////////////////////////////////////////////////////////////
    template<typename ElementType>
    class Stack {
    private:

        LinkedList<ElementType> m_list;             ///< Internal underlying data structure.

    public:

        // ////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // ////////////////////////////////////////////////////////////
        explicit Stack() : m_list() { };

        // ////////////////////////////////////////////////////////////
        // Copy constructor.
        //
        // ////////////////////////////////////////////////////////////
        explicit Stack(const Stack &stackObj) : m_list(stackObj.m_list) { };

        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        ~Stack() { };

        // ////////////////////////////////////////////////////////////
        // Assign the contents of a LinkedList to this list.
        //
        // ////////////////////////////////////////////////////////////
        void operator=(const Stack &rhs) {
            m_list = rhs.m_list;
        };

        // ////////////////////////////////////////////////////////////
        // Get the size of the stack.
        //
        // ////////////////////////////////////////////////////////////
        U64 Size() {
            return (m_list.Size());
        };

        // ////////////////////////////////////////////////////////////
        // Check if the stack is empty.
        //
        // ////////////////////////////////////////////////////////////
        bool IsEmpty() const {
            return (m_list.IsEmpty());
        };

        // ////////////////////////////////////////////////////////////
        // Push an object/type onto the top of the stack.
        //
        // @param val The value to push onto the stack.
        //
        // @return bool True|False on success|failure.
        //
        // ////////////////////////////////////////////////////////////
        bool Push(const ElementType &val) {
            return (m_list.PushBack(val));
        };

        // ////////////////////////////////////////////////////////////
        // Push the current top of the stack down by 1 and set the new
        // stack top as a copy of this value.
        //
        // @param val The value to push onto the stack.
        //
        // @return bool False if the stack is empty, not possible to push
        //              and copy the top element in this case.
        //
        // ////////////////////////////////////////////////////////////
        bool Push() {
            if(m_list.IsEmpty()) {
                return (false);
            }

            ElementType top;
            m_list.Back(top);
            return (m_list.PushBack(top));
        };

        // ////////////////////////////////////////////////////////////
        // Pop the top of the stack.
        //
        // @return bool True if the list was not empty and the top was
        //              popped.
        //
        // ////////////////////////////////////////////////////////////
        bool Pop() {
            return (m_list.PopBack());
        };

        // ////////////////////////////////////////////////////////////
        // Get the element at the top of the stack.
        //
        // @param ElementType& A copy of the object at the top of the
        //                      stack (only when result is true).
        //
        // @return True if the stack was not empty.
        //
        // ////////////////////////////////////////////////////////////
        bool Top(ElementType &topElement) {
            return (m_list.Back(topElement));
        };

        // ////////////////////////////////////////////////////////////
        // Replace the element at the top of the stack with a new value.
        //
        // @param newTopVal The new value of the stacks top element.
        //
        // ////////////////////////////////////////////////////////////
        bool SetTop(const ElementType &newTopVal) {
            if(!m_list.IsEmpty()) {
                m_list.PopBack();
            }

            return (m_list.PushBack(newTopVal));
        };

        // ////////////////////////////////////////////////////////////
        // Clear the stack.
        //
        // ////////////////////////////////////////////////////////////
        void Clear() {
            m_list.Clear();
        };

    };
}

#endif