#ifndef GF_I_CLEANABLE_OBSERVER_H
#define GF_I_CLEANABLE_OBSERVER_H

// /////////////////////////////////////////////////////////////////
// @file ICleanableObserver.h
// @author PJ O Halloran
// @date 26/11/2011
//
// Contains the ICleanableObserver interface.
//
// /////////////////////////////////////////////////////////////////


#include "ICleanable.h"


namespace GameHalloran
{
    
    // /////////////////////////////////////////////////////////////////
    // @class ICleanableObserver
    // @author PJ O Halloran.
    //
    // Cleanable Observer base interface.
    //
    // /////////////////////////////////////////////////////////////////
    class ICleanableObserver
    {
    public:
        
        virtual ~ICleanableObserver() {};
        
        virtual void VNotifyDirty(ICleanable *ptr) = 0;
    };
}


#endif
