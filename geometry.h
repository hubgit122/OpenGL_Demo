//-----------------------------------------------------------------------------
//           Name: geometry.h
//         Author: Freeglut
//  Last Modified: 01/27/05
//    Description: Data and utility functions for rendering several useful
//                 geometric shapes. This code is a modified version of the
//                 code found in "freeglut_teapot.c" and "freeglut_geometry.c",
//                 which is part of the open source project, Freegut.
//                 http://freeglut.sourceforge.net/
//
//                 See the text below this comment for all the legal licensing
//                 mumbo-jumbo.
//
// The following functions are defined here:
//
// void renderWireTeapot(GLdouble size);
// void renderSolidTeapot(GLdouble size);
// void renderWireCube(GLdouble size);
// void renderSolidCube(GLdouble size);
// void renderWireSphere(GLdouble radius, GLint slices, GLint stacks);
// void renderSolidSphere(GLdouble radius, GLint slices, GLint stacks);
// void renderWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
// void renderSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
// void renderWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
// void renderSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
// void renderWireDodecahedron(void);
// void renderSolidDodecahedron(void);
// void renderWireOctahedron(void);
// void renderSolidOctahedron(void);
// void renderWireTetrahedron(void);
// void renderSolidTetrahedron(void);
// void renderWireIcosahedron(void);
// void renderSolidIcosahedron(void);
// void renderWireSierpinskiSponge(int num_levels, GLdouble offset[3], GLdouble scale);
// void renderSolidSierpinskiSponge(int num_levels, GLdouble offset[3], GLdouble scale);
//-----------------------------------------------------------------------------

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

/*
 * freeglut_geometry.c
 *
 * Freeglut geometry rendering methods.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Fri Dec 3 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>
// Do this to access M_PI, which is not officially part of the C/C++ standard.
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/gl.h>

/* -- INTERFACE FUNCTIONS -------------------------------------------------- */

/*
 * Draws a wireframed cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
 */
void renderWireCube( GLdouble dSize )
{
	double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

	/*
	 * PWO: I dared to convert the code to use macros...
	 */
	glBegin( GL_LINE_LOOP );
	N( 1.0, 0.0, 0.0);
	V(+, -, +);
	V(+, -, -);
	V(+, +, -);
	V(+, +, +);
	glEnd();
	glBegin( GL_LINE_LOOP );
	N( 0.0, 1.0, 0.0);
	V(+, +, +);
	V(+, +, -);
	V(-, +, -);
	V(-, +, +);
	glEnd();
	glBegin( GL_LINE_LOOP );
	N( 0.0, 0.0, 1.0);
	V(+, +, +);
	V(-, +, +);
	V(-, -, +);
	V(+, -, +);
	glEnd();
	glBegin( GL_LINE_LOOP );
	N(-1.0, 0.0, 0.0);
	V(-, -, +);
	V(-, +, +);
	V(-, +, -);
	V(-, -, -);
	glEnd();
	glBegin( GL_LINE_LOOP );
	N( 0.0, -1.0, 0.0);
	V(-, -, +);
	V(-, -, -);
	V(+, -, -);
	V(+, -, +);
	glEnd();
	glBegin( GL_LINE_LOOP );
	N( 0.0, 0.0, -1.0);
	V(-, -, -);
	V(-, +, -);
	V(+, +, -);
	V(+, -, -);
	glEnd();

#   undef V
#   undef N
}

/*
 * Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
 */
void renderSolidCube( GLdouble dSize )
{
	double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

	/*
	 * PWO: Again, I dared to convert the code to use macros...
	 */
	glBegin( GL_QUADS );
	N( 1.0, 0.0, 0.0);
	V(+, -, +);
	V(+, -, -);
	V(+, +, -);
	V(+, +, +);
	N( 0.0, 1.0, 0.0);
	V(+, +, +);
	V(+, +, -);
	V(-, +, -);
	V(-, +, +);
	N( 0.0, 0.0, 1.0);
	V(+, +, +);
	V(-, +, +);
	V(-, -, +);
	V(+, -, +);
	N(-1.0, 0.0, 0.0);
	V(-, -, +);
	V(-, +, +);
	V(-, +, -);
	V(-, -, -);
	N( 0.0, -1.0, 0.0);
	V(-, -, +);
	V(-, -, -);
	V(+, -, -);
	V(+, -, +);
	N( 0.0, 0.0, -1.0);
	V(-, -, -);
	V(-, +, -);
	V(+, +, -);
	V(+, -, -);
	glEnd();

#   undef V
#   undef N
}

/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

static void circleTable(double** sint, double** cost, const int n)
{
	int i;

	/* Table size, the sign of n flips the circle direction */

	const int size = abs(n);

	/* Determine the angle between samples */

	const double angle = 2 * M_PI / (double)n;

	/* Allocate memory for n samples, plus duplicate of first entry at the end */

	*sint = (double*) calloc(sizeof(double), size + 1);
	*cost = (double*) calloc(sizeof(double), size + 1);

	/* Bail out if memory allocation fails, fgError never returns */

	if (!(*sint) || !(*cost))
	{
		free(*sint);
		free(*cost);
		//fgError("Failed to allocate memory in circleTable");
	}

	/* Compute cos and sin around the circle */

	for (i = 0; i < size; i++)
	{
		(*sint)[i] = sin(angle * i);
		(*cost)[i] = cos(angle * i);
	}

	/* Last sample is duplicate of the first */

	(*sint)[size] = (*sint)[0];
	(*cost)[size] = (*cost)[0];
}

/*
 * Draws a solid sphere
 */
void renderSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
	int i, j;

	/* Adjust z and radius as stacks are drawn. */

	double z0, z1;
	double r0, r1;

	/* Pre-computed circle */

	double* sint1, *cost1;
	double* sint2, *cost2;
	circleTable(&sint1, &cost1, -slices);
	circleTable(&sint2, &cost2, stacks * 2);

	/* The top stack is covered with a triangle fan */

	z0 = 1.0;
	z1 = cost2[1];
	r0 = 0.0;
	r1 = sint2[1];

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0, 0, 1);
	glVertex3d(0, 0, radius);

	for (j = slices; j >= 0; j--)
	{
		glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
		glVertex3d(cost1[j]*r1 * radius, sint1[j]*r1 * radius, z1 * radius);
	}

	glEnd();

	/* Cover each stack with a quad strip, except the top and bottom stacks */

	for( i = 1; i < stacks - 1; i++ )
	{
		z0 = z1;
		z1 = cost2[i + 1];
		r0 = r1;
		r1 = sint2[i + 1];

		glBegin(GL_QUAD_STRIP);

		for(j = 0; j <= slices; j++)
		{
			glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
			glVertex3d(cost1[j]*r1 * radius, sint1[j]*r1 * radius, z1 * radius);
			glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
			glVertex3d(cost1[j]*r0 * radius, sint1[j]*r0 * radius, z0 * radius);
		}

		glEnd();
	}

	/* The bottom stack is covered with a triangle fan */

	z0 = z1;
	r0 = r1;

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0, 0, -1);
	glVertex3d(0, 0, -radius);

	for (j = 0; j <= slices; j++)
	{
		glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
		glVertex3d(cost1[j]*r0 * radius, sint1[j]*r0 * radius, z0 * radius);
	}

	glEnd();

	/* Release sin and cos tables */

	free(sint1);
	free(cost1);
	free(sint2);
	free(cost2);
}

/*
 * Draws a solid sphere
 */
void renderWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
	int i, j;

	/* Adjust z and radius as stacks and slices are drawn. */

	double r;
	double x, y, z;

	/* Pre-computed circle */

	double* sint1, *cost1;
	double* sint2, *cost2;
	circleTable(&sint1, &cost1, -slices  );
	circleTable(&sint2, &cost2, stacks * 2);

	/* Draw a line loop for each stack */

	for (i = 1; i < stacks; i++)
	{
		z = cost2[i];
		r = sint2[i];

		glBegin(GL_LINE_LOOP);

		for(j = 0; j <= slices; j++)
		{
			x = cost1[j];
			y = sint1[j];

			glNormal3d(x, y, z);
			glVertex3d(x * r * radius, y * r * radius, z * radius);
		}

		glEnd();
	}

	/* Draw a line loop for each slice */

	for (i = 0; i < slices; i++)
	{
		glBegin(GL_LINE_STRIP);

		for(j = 0; j <= stacks; j++)
		{
			x = cost1[i] * sint2[j];
			y = sint1[i] * sint2[j];
			z = cost2[j];

			glNormal3d(x, y, z);
			glVertex3d(x * radius, y * radius, z * radius);
		}

		glEnd();
	}

	/* Release sin and cos tables */

	free(sint1);
	free(cost1);
	free(sint2);
	free(cost2);
}

/*
 * Draws a solid cone
 */
void renderSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks )
{
	int i, j;

	/* Step in z and radius as stacks are drawn. */

	double z0, z1;
	double r0, r1;

	const double zStep = height / stacks;
	const double rStep = base / stacks;

	/* Scaling factors for vertex normals */

	const double cosn = ( height / sqrt ( height * height + base * base ));
	const double sinn = ( base   / sqrt ( height * height + base * base ));

	/* Pre-computed circle */

	double* sint, *cost;
	circleTable(&sint, &cost, -slices);

	/* Cover the circular base with a triangle fan... */

	z0 = 0.0;
	z1 = zStep;

	r0 = base;
	r1 = r0 - rStep;

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0.0, 0.0, -1.0);
	glVertex3d(0.0, 0.0, z0 );

	for (j = 0; j <= slices; j++)
	{
		glVertex3d(cost[j]*r0, sint[j]*r0, z0);
	}

	glEnd();

	/* Cover each stack with a quad strip, except the top stack */

	for( i = 0; i < stacks - 1; i++ )
	{
		glBegin(GL_QUAD_STRIP);

		for(j = 0; j <= slices; j++)
		{
			glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
			glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
			glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
		}

		z0 = z1;
		z1 += zStep;
		r0 = r1;
		r1 -= rStep;

		glEnd();
	}

	/* The top stack is covered with individual triangles */

	glBegin(GL_TRIANGLES);

	glNormal3d(cost[0]*sinn, sint[0]*sinn, cosn);

	for (j = 0; j < slices; j++)
	{
		glVertex3d(cost[j + 0]*r0,   sint[j + 0]*r0,   z0    );
		glVertex3d(0,              0,              height);
		glNormal3d(cost[j + 1]*sinn, sint[j + 1]*sinn, cosn  );
		glVertex3d(cost[j + 1]*r0,   sint[j + 1]*r0,   z0    );
	}

	glEnd();

	/* Release sin and cos tables */

	free(sint);
	free(cost);
}

/*
 * Draws a wire cone
 */
void renderWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	int i, j;

	/* Step in z and radius as stacks are drawn. */

	double z = 0.0;
	double r = base;

	const double zStep = height / stacks;
	const double rStep = base / stacks;

	/* Scaling factors for vertex normals */

	const double cosn = ( height / sqrt ( height * height + base * base ));
	const double sinn = ( base   / sqrt ( height * height + base * base ));

	/* Pre-computed circle */

	double* sint, *cost;
	circleTable(&sint, &cost, -slices);

	/* Draw the stacks... */

	for (i = 0; i < stacks; i++)
	{
		glBegin(GL_LINE_LOOP);

		for( j = 0; j < slices; j++ )
		{
			glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
			glVertex3d(cost[j]*r,    sint[j]*r,    z   );
		}

		glEnd();

		z += zStep;
		r -= rStep;
	}

	/* Draw the slices */

	r = base;

	glBegin(GL_LINES);

	for (j = 0; j < slices; j++)
	{
		glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn  );
		glVertex3d(cost[j]*r,    sint[j]*r,    0.0   );
		glVertex3d(0.0,          0.0,          height);
	}

	glEnd();

	/* Release sin and cos tables */

	free(sint);
	free(cost);
}


/*
 * Draws a solid cylinder
 */
void renderSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
	int i, j;

	/* Step in z and radius as stacks are drawn. */

	double z0, z1;
	const double zStep = height / stacks;

	/* Pre-computed circle */

	double* sint, *cost;
	circleTable(&sint, &cost, -slices);

	/* Cover the base and top */

	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 0.0, -1.0 );
	glVertex3d(0.0, 0.0,  0.0 );

	for (j = 0; j <= slices; j++)
	{
		glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
	}

	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 0.0, 1.0   );
	glVertex3d(0.0, 0.0, height);

	for (j = slices; j >= 0; j--)
	{
		glVertex3d(cost[j]*radius, sint[j]*radius, height);
	}

	glEnd();

	/* Do the stacks */

	z0 = 0.0;
	z1 = zStep;

	for (i = 1; i <= stacks; i++)
	{
		if (i == stacks)
		{
			z1 = height;
		}

		glBegin(GL_QUAD_STRIP);

		for (j = 0; j <= slices; j++ )
		{
			glNormal3d(cost[j],        sint[j],        0.0 );
			glVertex3d(cost[j]*radius, sint[j]*radius, z0  );
			glVertex3d(cost[j]*radius, sint[j]*radius, z1  );
		}

		glEnd();

		z0 = z1;
		z1 += zStep;
	}

	/* Release sin and cos tables */

	free(sint);
	free(cost);
}

/*
 * Draws a wire cylinder
   */
void renderWireCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
	int i, j;

	/* Step in z and radius as stacks are drawn. */

	double z = 0.0;
	const double zStep = height / stacks;

	/* Pre-computed circle */

	double* sint, *cost;
	circleTable(&sint, &cost, -slices);

	/* Draw the stacks... */

	for (i = 0; i <= stacks; i++)
	{
		if (i == stacks)
		{
			z = height;
		}

		glBegin(GL_LINE_LOOP);

		for( j = 0; j < slices; j++ )
		{
			glNormal3d(cost[j],        sint[j],        0.0);
			glVertex3d(cost[j]*radius, sint[j]*radius, z  );
		}

		glEnd();

		z += zStep;
	}

	/* Draw the slices */

	glBegin(GL_LINES);

	for (j = 0; j < slices; j++)
	{
		glNormal3d(cost[j],        sint[j],        0.0   );
		glVertex3d(cost[j]*radius, sint[j]*radius, 0.0   );
		glVertex3d(cost[j]*radius, sint[j]*radius, height);
	}

	glEnd();

	/* Release sin and cos tables */

	free(sint);
	free(cost);
}

/*
 *
 */
void renderWireTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
	double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
	double* vertex, *normal;
	int    i, j;
	double spsi, cpsi, sphi, cphi ;

	/*
	 * Allocate the vertices array
	 */
	vertex = (double*)calloc( sizeof(double), 3 * nSides * nRings );
	normal = (double*)calloc( sizeof(double), 3 * nSides * nRings );

	glPushMatrix();

	dpsi =  2.0 * M_PI / (double)nRings ;
	dphi = -2.0 * M_PI / (double)nSides ;
	psi  = 0.0;

	for( j = 0; j < nRings; j++ )
	{
		cpsi = cos ( psi ) ;
		spsi = sin ( psi ) ;
		phi = 0.0;

		for( i = 0; i < nSides; i++ )
		{
			int offset = 3 * ( j * nSides + i ) ;
			cphi = cos ( phi ) ;
			sphi = sin ( phi ) ;
			*(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
			*(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
			*(vertex + offset + 2) =                    sphi * iradius  ;
			*(normal + offset + 0) = cpsi * cphi ;
			*(normal + offset + 1) = spsi * cphi ;
			*(normal + offset + 2) =        sphi ;
			phi += dphi;
		}

		psi += dpsi;
	}

	for( i = 0; i < nSides; i++ )
	{
		glBegin( GL_LINE_LOOP );

		for( j = 0; j < nRings; j++ )
		{
			int offset = 3 * ( j * nSides + i ) ;
			glNormal3dv( normal + offset );
			glVertex3dv( vertex + offset );
		}

		glEnd();
	}

	for( j = 0; j < nRings; j++ )
	{
		glBegin(GL_LINE_LOOP);

		for( i = 0; i < nSides; i++ )
		{
			int offset = 3 * ( j * nSides + i ) ;
			glNormal3dv( normal + offset );
			glVertex3dv( vertex + offset );
		}

		glEnd();
	}

	free ( vertex ) ;
	free ( normal ) ;
	glPopMatrix();
}

/*
 *
 */
void renderSolidTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
	double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
	double* vertex, *normal;
	int    i, j;
	double spsi, cpsi, sphi, cphi ;

	/*
	 * Increment the number of sides and rings to allow for one more point than surface
	 */
	nSides ++ ;
	nRings ++ ;

	/*
	 * Allocate the vertices array
	 */
	vertex = (double*)calloc( sizeof(double), 3 * nSides * nRings );
	normal = (double*)calloc( sizeof(double), 3 * nSides * nRings );

	glPushMatrix();

	dpsi =  2.0 * M_PI / (double)(nRings - 1) ;
	dphi = -2.0 * M_PI / (double)(nSides - 1) ;
	psi  = 0.0;

	for( j = 0; j < nRings; j++ )
	{
		cpsi = cos ( psi ) ;
		spsi = sin ( psi ) ;
		phi = 0.0;

		for( i = 0; i < nSides; i++ )
		{
			int offset = 3 * ( j * nSides + i ) ;
			cphi = cos ( phi ) ;
			sphi = sin ( phi ) ;
			*(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
			*(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
			*(vertex + offset + 2) =                    sphi * iradius  ;
			*(normal + offset + 0) = cpsi * cphi ;
			*(normal + offset + 1) = spsi * cphi ;
			*(normal + offset + 2) =        sphi ;
			phi += dphi;
		}

		psi += dpsi;
	}

	glBegin( GL_QUADS );

	for( i = 0; i < nSides - 1; i++ )
	{
		for( j = 0; j < nRings - 1; j++ )
		{
			int offset = 3 * ( j * nSides + i ) ;
			glNormal3dv( normal + offset );
			glVertex3dv( vertex + offset );
			glNormal3dv( normal + offset + 3 );
			glVertex3dv( vertex + offset + 3 );
			glNormal3dv( normal + offset + 3 * nSides + 3 );
			glVertex3dv( vertex + offset + 3 * nSides + 3 );
			glNormal3dv( normal + offset + 3 * nSides );
			glVertex3dv( vertex + offset + 3 * nSides );
		}
	}

	glEnd();

	free ( vertex ) ;
	free ( normal ) ;
	glPopMatrix();
}

/*
 *
 */
void renderWireDodecahedron( void )
{
	/* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
	 * of a cube.  The coordinates of the points are:
	 *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
	 * where x = 0.61803398875 and z = 1.61803398875.
	 */
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ;
	glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ;
	glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ;
	glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ;
	glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;

	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ;
	glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ;
	glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ;
	glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ;
	glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;

	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_LINE_LOOP ) ;
	glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
}

/*
 *
 */
void renderSolidDodecahedron( void )
{
	/* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
	 * of a cube.  The coordinates of the points are:
	 *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
	 * where x = 0.61803398875 and z = 1.61803398875.
	 */
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ;
	glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ;
	glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ;
	glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ;
	glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;

	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ;
	glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ;
	glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ;
	glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ;
	glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;

	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ;
	glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ;
	glVertex3d (  1.0,  1.0, -1.0 ) ;
	glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ;
	glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ;
	glVertex3d (  1.0, -1.0,  1.0 ) ;
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ;
	glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ;
	glVertex3d ( -1.0,  1.0,  1.0 ) ;
	glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ;
	glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ;
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
	glBegin ( GL_POLYGON ) ;
	glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ;
	glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ;
	glVertex3d ( -1.0, -1.0, -1.0 ) ;
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
}

/*
 *
 */
void renderWireOctahedron( void )
{
#define RADIUS    1.0f
	glBegin( GL_LINE_LOOP );
	glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d( 0.577350269189, 0.577350269189, -0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d( 0.577350269189, -0.577350269189, 0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d( 0.577350269189, -0.577350269189, -0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glEnd();
#undef RADIUS
}

/*
 *
 */
void renderSolidOctahedron( void )
{
#define RADIUS    1.0f
	glBegin( GL_TRIANGLES );
	glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d( 0.577350269189, 0.577350269189, -0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d( 0.577350269189, -0.577350269189, 0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d( 0.577350269189, -0.577350269189, -0.577350269189);
	glVertex3d( RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, RADIUS );
	glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189);
	glVertex3d(-RADIUS, 0.0, 0.0 );
	glVertex3d( 0.0, -RADIUS, 0.0 );
	glVertex3d( 0.0, 0.0, -RADIUS );
	glEnd();
#undef RADIUS
}

/*
 *
 */
void renderWireTetrahedron( void )
{
	/* Magic Numbers:  r0 = ( 1, 0, 0 )
	 *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
	 *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
	 *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
	 * |r0| = |r1| = |r2| = |r3| = 1
	 * Distance between any two points is 2 sqrt(6) / 3
	 *
	 * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
	 */

	double r0[3] = {             1.0,             0.0,             0.0 } ;
	double r1[3] = { -0.333333333333,  0.942809041582,             0.0 } ;
	double r2[3] = { -0.333333333333, -0.471404520791,  0.816496580928 } ;
	double r3[3] = { -0.333333333333, -0.471404520791, -0.816496580928 } ;

	glBegin( GL_LINE_LOOP ) ;
	glNormal3d (           -1.0,             0.0,             0.0 ) ;
	glVertex3dv ( r1 ) ;
	glVertex3dv ( r3 ) ;
	glVertex3dv ( r2 ) ;
	glNormal3d ( 0.333333333333, -0.942809041582,             0.0 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r2 ) ;
	glVertex3dv ( r3 ) ;
	glNormal3d ( 0.333333333333,  0.471404520791, -0.816496580928 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r3 ) ;
	glVertex3dv ( r1 ) ;
	glNormal3d ( 0.333333333333,  0.471404520791,  0.816496580928 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r1 ) ;
	glVertex3dv ( r2 ) ;
	glEnd() ;
}

/*
 *
 */
void renderSolidTetrahedron( void )
{
	/* Magic Numbers:  r0 = ( 1, 0, 0 )
	 *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
	 *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
	 *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
	 * |r0| = |r1| = |r2| = |r3| = 1
	 * Distance between any two points is 2 sqrt(6) / 3
	 *
	 * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
	 */

	double r0[3] = {             1.0,             0.0,             0.0 } ;
	double r1[3] = { -0.333333333333,  0.942809041582,             0.0 } ;
	double r2[3] = { -0.333333333333, -0.471404520791,  0.816496580928 } ;
	double r3[3] = { -0.333333333333, -0.471404520791, -0.816496580928 } ;

	glBegin( GL_TRIANGLES ) ;
	glNormal3d (           -1.0,             0.0,             0.0 ) ;
	glVertex3dv ( r1 ) ;
	glVertex3dv ( r3 ) ;
	glVertex3dv ( r2 ) ;
	glNormal3d ( 0.333333333333, -0.942809041582,             0.0 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r2 ) ;
	glVertex3dv ( r3 ) ;
	glNormal3d ( 0.333333333333,  0.471404520791, -0.816496580928 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r3 ) ;
	glVertex3dv ( r1 ) ;
	glNormal3d ( 0.333333333333,  0.471404520791,  0.816496580928 ) ;
	glVertex3dv ( r0 ) ;
	glVertex3dv ( r1 ) ;
	glVertex3dv ( r2 ) ;
	glEnd() ;
}

/*
 *
 */
double icos_r[12][3] = { { 1.0, 0.0, 0.0 },
	{  0.447213595500,  0.894427191000, 0.0 }, {  0.447213595500,  0.276393202252, 0.850650808354 }, {  0.447213595500, -0.723606797748, 0.525731112119 }, {  0.447213595500, -0.723606797748, -0.525731112119 }, {  0.447213595500,  0.276393202252, -0.850650808354 },
	{ -0.447213595500, -0.894427191000, 0.0 }, { -0.447213595500, -0.276393202252, 0.850650808354 }, { -0.447213595500,  0.723606797748, 0.525731112119 }, { -0.447213595500,  0.723606797748, -0.525731112119 }, { -0.447213595500, -0.276393202252, -0.850650808354 },
	{ -1.0, 0.0, 0.0 }
} ;
int icos_v [20][3] = { { 0, 1, 2 }, { 0, 2, 3 }, { 0, 3, 4 }, { 0, 4, 5 }, { 0, 5, 1 },
	{ 1, 8, 2 }, { 2, 7, 3 }, { 3, 6, 4 }, { 4, 10, 5 }, { 5, 9, 1 },
	{ 1, 9, 8 }, { 2, 8, 7 }, { 3, 7, 6 }, { 4, 6, 10 }, { 5, 10, 9 },
	{ 11, 9, 10 }, { 11, 8, 9 }, { 11, 7, 8 }, { 11, 6, 7 }, { 11, 10, 6 }
} ;

void renderWireIcosahedron( void )
{
	int i ;

	for ( i = 0; i < 20; i++ )
	{
		double normal[3] ;
		normal[0] = ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) - ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) ;
		normal[1] = ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) - ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) ;
		normal[2] = ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) - ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) ;
		glBegin ( GL_LINE_LOOP ) ;
		glNormal3dv ( normal ) ;
		glVertex3dv ( icos_r[icos_v[i][0]] ) ;
		glVertex3dv ( icos_r[icos_v[i][1]] ) ;
		glVertex3dv ( icos_r[icos_v[i][2]] ) ;
		glEnd () ;
	}
}

/*
 *
 */
void renderSolidIcosahedron( void )
{
	int i ;

	glBegin ( GL_TRIANGLES ) ;

	for ( i = 0; i < 20; i++ )
	{
		double normal[3] ;
		normal[0] = ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) - ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) ;
		normal[1] = ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) - ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) ;
		normal[2] = ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) - ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) ;
		glNormal3dv ( normal ) ;
		glVertex3dv ( icos_r[icos_v[i][0]] ) ;
		glVertex3dv ( icos_r[icos_v[i][1]] ) ;
		glVertex3dv ( icos_r[icos_v[i][2]] ) ;
	}

	glEnd () ;
}

/*
 *
 */
double rdod_r[14][3] = { { 0.0, 0.0, 1.0 },
	{  0.707106781187,  0.000000000000,  0.5 }, {  0.000000000000,  0.707106781187,  0.5 }, { -0.707106781187,  0.000000000000,  0.5 }, {  0.000000000000, -0.707106781187,  0.5 },
	{  0.707106781187,  0.707106781187,  0.0 }, { -0.707106781187,  0.707106781187,  0.0 }, { -0.707106781187, -0.707106781187,  0.0 }, {  0.707106781187, -0.707106781187,  0.0 },
	{  0.707106781187,  0.000000000000, -0.5 }, {  0.000000000000,  0.707106781187, -0.5 }, { -0.707106781187,  0.000000000000, -0.5 }, {  0.000000000000, -0.707106781187, -0.5 },
	{  0.0, 0.0, -1.0 }
} ;
int rdod_v [12][4] = { { 0,  1,  5,  2 }, { 0,  2,  6,  3 }, { 0,  3,  7,  4 }, { 0,  4,  8, 1 },
	{ 5, 10,  6,  2 }, { 6, 11,  7,  3 }, { 7, 12,  8,  4 }, { 8,  9,  5, 1 },
	{ 5,  9, 13, 10 }, { 6, 10, 13, 11 }, { 7, 11, 13, 12 }, { 8, 12, 13, 9 }
} ;
double rdod_n[12][3] =
{
	{  0.353553390594,  0.353553390594,  0.5 }, { -0.353553390594,  0.353553390594,  0.5 }, { -0.353553390594, -0.353553390594,  0.5 }, {  0.353553390594, -0.353553390594,  0.5 },
	{  0.000000000000,  1.000000000000,  0.0 }, { -1.000000000000,  0.000000000000,  0.0 }, {  0.000000000000, -1.000000000000,  0.0 }, {  1.000000000000,  0.000000000000,  0.0 },
	{  0.353553390594,  0.353553390594, -0.5 }, { -0.353553390594,  0.353553390594, -0.5 }, { -0.353553390594, -0.353553390594, -0.5 }, {  0.353553390594, -0.353553390594, -0.5 }
} ;

void renderWireRhombicDodecahedron( void )
{
	int i ;

	for ( i = 0; i < 12; i++ )
	{
		glBegin ( GL_LINE_LOOP ) ;
		glNormal3dv ( rdod_n[i] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][0]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][1]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][2]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][3]] ) ;
		glEnd () ;
	}
}

/*
 *
 */
void renderSolidRhombicDodecahedron( void )
{
	int i ;

	glBegin ( GL_QUADS ) ;

	for ( i = 0; i < 12; i++ )
	{
		glNormal3dv ( rdod_n[i] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][0]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][1]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][2]] ) ;
		glVertex3dv ( rdod_r[rdod_v[i][3]] ) ;
	}

	glEnd () ;
}

#define NUM_FACES     4

static GLdouble tetrahedron_v[4][3] =  /* Vertices */
{
	{ -0.5, -0.288675134595, -0.144337567297 },
	{  0.5, -0.288675134595, -0.144337567297 },
	{  0.0,  0.577350269189, -0.144337567297 },
	{  0.0,  0.0,             0.672159013631 }
} ;

static GLint tetrahedron_i[4][3] =  /* Vertex indices */
{
	{ 0, 1, 2 }, { 0, 2, 3 }, { 0, 3, 1 }, { 1, 3, 2 }
} ;

static GLdouble tetrahedron_n[4][3] =  /* Normals */
{
	{  0.0,             0.0,            -1.0 },
	{ -0.816496580928,  0.471404520791,  0.333333333333 },
	{  0.0,            -0.942809041582,  0.333333333333 },
	{  0.816496580928,  0.471404520791,  0.333333333333 }
} ;

void renderWireSierpinskiSponge ( int num_levels, GLdouble offset[3], GLdouble scale )
{
	int i, j ;

	if ( num_levels == 0 )
	{

		for ( i = 0 ; i < NUM_FACES ; i++ )
		{
			glBegin ( GL_LINE_LOOP ) ;
			glNormal3dv ( tetrahedron_n[i] ) ;

			for ( j = 0; j < 3; j++ )
			{
				double x = offset[0] + scale * tetrahedron_v[tetrahedron_i[i][j]][0] ;
				double y = offset[1] + scale * tetrahedron_v[tetrahedron_i[i][j]][1] ;
				double z = offset[2] + scale * tetrahedron_v[tetrahedron_i[i][j]][2] ;
				glVertex3d ( x, y, z ) ;
			}

			glEnd () ;
		}
	}
	else
	{
		GLdouble local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
		num_levels -- ;
		scale /= 2.0 ;
		local_offset[0] = offset[0] + scale * tetrahedron_v[0][0] ;
		local_offset[1] = offset[1] + scale * tetrahedron_v[0][1] ;
		local_offset[2] = offset[2] + scale * tetrahedron_v[0][2] ;
		renderWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[0] += scale ;
		renderWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[0] -= 0.5            * scale ;
		local_offset[1] += 0.866025403784 * scale ;
		renderWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[1] -= 0.577350269189 * scale ;
		local_offset[2] += 0.816496580928 * scale ;
		renderWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
	}
}

void renderSolidSierpinskiSponge ( int num_levels, GLdouble offset[3], GLdouble scale )
{
	int i, j ;

	if ( num_levels == 0 )
	{
		glBegin ( GL_TRIANGLES ) ;

		for ( i = 0 ; i < NUM_FACES ; i++ )
		{
			glNormal3dv ( tetrahedron_n[i] ) ;

			for ( j = 0; j < 3; j++ )
			{
				double x = offset[0] + scale * tetrahedron_v[tetrahedron_i[i][j]][0] ;
				double y = offset[1] + scale * tetrahedron_v[tetrahedron_i[i][j]][1] ;
				double z = offset[2] + scale * tetrahedron_v[tetrahedron_i[i][j]][2] ;
				glVertex3d ( x, y, z ) ;
			}
		}

		glEnd () ;
	}
	else
	{
		GLdouble local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
		num_levels -- ;
		scale /= 2.0 ;
		local_offset[0] = offset[0] + scale * tetrahedron_v[0][0] ;
		local_offset[1] = offset[1] + scale * tetrahedron_v[0][1] ;
		local_offset[2] = offset[2] + scale * tetrahedron_v[0][2] ;
		renderSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[0] += scale ;
		renderSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[0] -= 0.5            * scale ;
		local_offset[1] += 0.866025403784 * scale ;
		renderSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
		local_offset[1] -= 0.577350269189 * scale ;
		local_offset[2] += 0.816496580928 * scale ;
		renderSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
	}
}

#undef NUM_FACES

/*
 * freeglut_teapot.c
 *
 * Teapot(tm) rendering code.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Fri Dec 24 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Original teapot code copyright follows:
 */

/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *
 * ALL RIGHTS RESERVED
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Silicon
 * Graphics, Inc. not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
 * "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
 * OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
 * EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
 * ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
 * SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
 * NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer
 * Software clause at DFARS 252.227-7013 and/or in similar or
 * successor clauses in the FAR or the DOD or NASA FAR
 * Supplement.  Unpublished-- rights reserved under the copyright
 * laws of the United States.  Contractor/manufacturer is Silicon
 * Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
 * 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

/* -- PRIVATE FUNCTIONS ---------------------------------------------------- */

/*
 * Rim, body, lid, and bottom data must be reflected in x and y;
 * handle and spout data across the y axis only.
 */
static int patchdata[][16] =
{
	{ 102, 103, 104, 105,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15 }, /* rim    */
	{  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27 }, /* body   */
	{  24,  25,  26,  27,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40 },
	{  96,  96,  96,  96,  97,  98,  99, 100, 101, 101, 101, 101,   0,   1,   2,   3 }, /* lid    */
	{   0,   1,   2,   3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117 },
	{ 118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120,  40,  39,  38,  37 }, /* bottom */
	{  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56 }, /* handle */
	{  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  28,  65,  66,  67 },
	{  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83 }, /* spout  */
	{  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95 }
};

static double cpdata[][3] =
{
	{0.2, 0, 2.7}, {0.2, -0.112, 2.7}, {0.112, -0.2, 2.7}, {
		0,
		-0.2, 2.7
	}, {1.3375, 0, 2.53125}, {1.3375, -0.749, 2.53125},
	{0.749, -1.3375, 2.53125}, {0, -1.3375, 2.53125}, {
		1.4375,
		0, 2.53125
	}, {1.4375, -0.805, 2.53125}, {
		0.805, -1.4375,
		2.53125
	}, {0, -1.4375, 2.53125}, {1.5, 0, 2.4}, {
		1.5, -0.84,
		2.4
	}, {0.84, -1.5, 2.4}, {0, -1.5, 2.4}, {1.75, 0, 1.875},
	{1.75, -0.98, 1.875}, {0.98, -1.75, 1.875}, {
		0, -1.75,
		1.875
	}, {2, 0, 1.35}, {2, -1.12, 1.35}, {1.12, -2, 1.35},
	{0, -2, 1.35}, {2, 0, 0.9}, {2, -1.12, 0.9}, {
		1.12, -2,
		0.9
	}, {0, -2, 0.9}, { -2, 0, 0.9}, {2, 0, 0.45}, {
		2, -1.12,
		0.45
	}, {1.12, -2, 0.45}, {0, -2, 0.45}, {1.5, 0, 0.225},
	{1.5, -0.84, 0.225}, {0.84, -1.5, 0.225}, {0, -1.5, 0.225},
	{1.5, 0, 0.15}, {1.5, -0.84, 0.15}, {0.84, -1.5, 0.15}, {
		0,
		-1.5, 0.15
	}, { -1.6, 0, 2.025}, { -1.6, -0.3, 2.025}, {
		-1.5,
		-0.3, 2.25
	}, { -1.5, 0, 2.25}, { -2.3, 0, 2.025}, {
		-2.3, -0.3,
		2.025
	}, { -2.5, -0.3, 2.25}, { -2.5, 0, 2.25}, {
		-2.7, 0,
		2.025
	}, { -2.7, -0.3, 2.025}, { -3, -0.3, 2.25}, {
		-3, 0,
		2.25
	}, { -2.7, 0, 1.8}, { -2.7, -0.3, 1.8}, { -3, -0.3, 1.8},
	{ -3, 0, 1.8}, { -2.7, 0, 1.575}, { -2.7, -0.3, 1.575}, {
		-3,
		-0.3, 1.35
	}, { -3, 0, 1.35}, { -2.5, 0, 1.125}, {
		-2.5, -0.3,
		1.125
	}, { -2.65, -0.3, 0.9375}, { -2.65, 0, 0.9375}, {
		-2,
		-0.3, 0.9
	}, { -1.9, -0.3, 0.6}, { -1.9, 0, 0.6}, {
		1.7, 0,
		1.425
	}, {1.7, -0.66, 1.425}, {1.7, -0.66, 0.6}, {
		1.7, 0,
		0.6
	}, {2.6, 0, 1.425}, {2.6, -0.66, 1.425}, {
		3.1, -0.66,
		0.825
	}, {3.1, 0, 0.825}, {2.3, 0, 2.1}, {2.3, -0.25, 2.1},
	{2.4, -0.25, 2.025}, {2.4, 0, 2.025}, {2.7, 0, 2.4}, {
		2.7,
		-0.25, 2.4
	}, {3.3, -0.25, 2.4}, {3.3, 0, 2.4}, {
		2.8, 0,
		2.475
	}, {2.8, -0.25, 2.475}, {3.525, -0.25, 2.49375},
	{3.525, 0, 2.49375}, {2.9, 0, 2.475}, {2.9, -0.15, 2.475},
	{3.45, -0.15, 2.5125}, {3.45, 0, 2.5125}, {2.8, 0, 2.4},
	{2.8, -0.15, 2.4}, {3.2, -0.15, 2.4}, {3.2, 0, 2.4}, {
		0, 0,
		3.15
	}, {0.8, 0, 3.15}, {0.8, -0.45, 3.15}, {
		0.45, -0.8,
		3.15
	}, {0, -0.8, 3.15}, {0, 0, 2.85}, {1.4, 0, 2.4}, {
		1.4,
		-0.784, 2.4
	}, {0.784, -1.4, 2.4}, {0, -1.4, 2.4}, {
		0.4, 0,
		2.55
	}, {0.4, -0.224, 2.55}, {0.224, -0.4, 2.55}, {
		0, -0.4,
		2.55
	}, {1.3, 0, 2.55}, {1.3, -0.728, 2.55}, {
		0.728, -1.3,
		2.55
	}, {0, -1.3, 2.55}, {1.3, 0, 2.4}, {1.3, -0.728, 2.4},
	{0.728, -1.3, 2.4}, {0, -1.3, 2.4}, {0, 0, 0}, {
		1.425,
		-0.798, 0
	}, {1.5, 0, 0.075}, {1.425, 0, 0}, {
		0.798, -1.425,
		0
	}, {0, -1.5, 0.075}, {0, -1.425, 0}, {1.5, -0.84, 0.075},
	{0.84, -1.5, 0.075}
};

static double tex[2][2][2] =
{
	{ {0.0, 0.0}, {1.0, 0.0} },
	{ {0.0, 1.0}, {1.0, 1.0} }
};

static void teapot( GLint grid, GLdouble scale, GLenum type )
{
	double p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
	long i, j, k, l;

	glPushAttrib( GL_ENABLE_BIT | GL_EVAL_BIT );
	glEnable( GL_AUTO_NORMAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_MAP2_VERTEX_3 );
	glEnable( GL_MAP2_TEXTURE_COORD_2 );

	glPushMatrix();
	glRotated(270.0, 1.0, 0.0, 0.0);
	glScaled(0.5 * scale, 0.5 * scale, 0.5 * scale);
	glTranslated(0.0, 0.0, -1.5);

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 4; k++)
			{
				for (l = 0; l < 3; l++)
				{
					p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
					q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];

					if (l == 1)
					{
						q[j][k][l] *= -1.0;
					}

					if (i < 6)
					{
						r[j][k][l] =
							cpdata[patchdata[i][j * 4 + (3 - k)]][l];

						if (l == 0)
						{
							r[j][k][l] *= -1.0;
						}

						s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];

						if (l == 0)
						{
							s[j][k][l] *= -1.0;
						}

						if (l == 1)
						{
							s[j][k][l] *= -1.0;
						}
					}
				}
			}
		}

		glMap2d(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
				&tex[0][0][0]);
		glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
				&p[0][0][0]);
		glMapGrid2d(grid, 0.0, 1.0, grid, 0.0, 1.0);
		glEvalMesh2(type, 0, grid, 0, grid);
		glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
				&q[0][0][0]);
		glEvalMesh2(type, 0, grid, 0, grid);

		if (i < 6)
		{
			glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
					&r[0][0][0]);
			glEvalMesh2(type, 0, grid, 0, grid);
			glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
					&s[0][0][0]);
			glEvalMesh2(type, 0, grid, 0, grid);
		}
	}

	glPopMatrix();
	glPopAttrib();
}


/* -- INTERFACE FUNCTIONS -------------------------------------------------- */

/*
 * Renders a beautiful wired teapot...
 */
void renderWireTeapot( GLdouble size )
{
	/*
	 * We will use the general teapot rendering code
	 */
	teapot( 10, size, GL_LINE );
}

/*
 * Renders a beautiful filled teapot...
 */
void renderSolidTeapot( GLdouble size )
{
	/*
	 * We will use the general teapot rendering code
	 */
	teapot( 7, size, GL_FILL );
}

#endif // _GEOMETRY_H_
