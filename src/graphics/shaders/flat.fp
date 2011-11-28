// /////////////////////////////////////////////////////////////////
// @file flat.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Sets all fragments to the same color passed in as a uniform.
//
// /////////////////////////////////////////////////////////////////

#version 150

// Make geometry solid
uniform vec4 colorVec;

// Output fragment color
out vec4 fragmentColorVec;

void main(void)
{ 
   fragmentColorVec = colorVec;
}
