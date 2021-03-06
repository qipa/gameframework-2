#pragma once
#ifndef __GAME_EXCEPTION_H
#define __GAME_EXCEPTION_H

// /////////////////////////////////////////////////////////////////
// @file GameException.h
// @author PJ O Halloran
// @date 21/11/2009
//
// File contains the header for the GameException class.
//
// /////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

namespace GameHalloran {
    // /////////////////////////////////////////////////////////////////
    // @class GameException
    // @author PJ O Halloran
    //
    // This class represents exceptions that may occur during a game.
    // It is the base class from which all GameExceptions must inherit
    // from.
    //
    // /////////////////////////////////////////////////////////////////
    class GameException : public std::exception {
    private:

        std::string m_msg;                      ///< Exception message.

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        GameException();

        // /////////////////////////////////////////////////////////////////
        // Custom constructor.
        //
        // @param errorMessageRef The error message associated with the
        //                          exception.
        //
        // /////////////////////////////////////////////////////////////////
        GameException(const std::string &errorMessageRef);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~GameException() throw();

        // /////////////////////////////////////////////////////////////////
        // Description of exception.
        //
        // /////////////////////////////////////////////////////////////////
        const char *what() const throw();
    };
}

#endif
