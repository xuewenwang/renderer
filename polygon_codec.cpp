//https://fabiensanglard.net/polygon_codec/


enum { X, Y, Z, W };

typedef float vec4_t[4];

typedef float vec2_t[2];

#define MAX_VERTICES_PRE_POLYGON  10 

typedef struct polygon_t
{
    //Homogenous space vertices
    unsigned char	hs_numVertices;
    vec4_t	hs_vertices[MAX_VERTICES_PRE_POLYGON];

} polygon_t;




#define vector4Subtract( a, b, c )   ((c)[ 0 ] = (a)[ 0 ] - (b)[ 0 ], \
                                          (c)[ 1 ] = (a)[ 1 ] - (b)[ 1 ], \
                                          (c)[ 2 ] = (a)[ 2 ] - (b)[ 2 ], \
                                          (c)[ 3 ] = (a)[ 3 ] - (b)[ 3 ] )

#define vector4Add( a, b, c )        ((c)[ 0 ] = (a)[ 0 ] + (b)[ 0 ],\
                                          (c)[ 1 ] = (a)[ 1 ] + (b)[ 1 ],\
                                          (c)[ 2 ] = (a)[ 2 ] + (b)[ 2 ],\
                                          (c)[ 3 ] = (a)[ 3 ] + (b)[ 3 ] )

#define vector4Copy( a, b )          ((b)[ 0 ] = (a)[ 0 ],\
                                          (b)[ 1 ] = (a)[ 1 ],\
                                          (b)[ 2 ] = (a)[ 2 ],\
                                          (b)[ 3 ] = (a)[ 3 ] )

#define vector4Scale( v, s, o )      ((o)[ 0 ] = (v)[ 0 ] * (s),\
                                          (o)[ 1 ] = (v)[ 1 ] * (s),\
                                          (o)[ 2 ] = (v)[ 2 ] * (s),\
                                          (o)[ 3 ] = (v)[ 3 ] * (s) )


void copyVertices(vec4_t* from, polygon_t* to, int numVertices)
{
    int i;

    to->hs_numVertices = numVertices;
    for (i = 0; i < numVertices; i++)
    {
        vector4Copy(from[i], to->hs_vertices[i]);
    }
}


#define W_CLIPPING_PLANE 0.00001 

//Clip against plane w=W_CLIPPING_PLANE
/*
We can then write the first method that will remove any point with W negative and bring them just above zero to
avoid a divive by zero during the perspective divide phase. This will avoid the special case mentionned previously.
该方法将删除W为负的任何点，并使它们刚好高于零，以避免在透视除法阶段被零除。这将避免前面提到的特殊情况。
*/
void ClipPolygonOnWAxis(polygon_t* face)
{
    int i;

    vec4_t* currentVertice;
    vec4_t* previousVertice;


    unsigned char in_numVertices = 0;
    vec4_t in_vertices[MAX_VERTICES_PRE_POLYGON];

    char previousDot;
    char currentDot;

    float intersectionFactor;
    vec4_t intersectionPoint;


    previousVertice = &face->hs_vertices[face->hs_numVertices - 1];
    previousDot = ((*previousVertice)[W] < W_CLIPPING_PLANE) ? -1 : 1;
    currentVertice = &face->hs_vertices[0];
    while (currentVertice != &face->hs_vertices[face->hs_numVertices])
    {
        currentDot = ((*currentVertice)[W] < W_CLIPPING_PLANE) ? -1 : 1;

        if (previousDot * currentDot < 0)
        {
            //Need to clip against plan w=0

            intersectionFactor = (W_CLIPPING_PLANE - (*previousVertice)[W]) / ((*previousVertice)[W] - (*currentVertice)[W]);

            // I = Qp + f(Qc-Qp))
            vector4Copy(*currentVertice, intersectionPoint);                              //          Qc
            vector4Subtract(intersectionPoint, *previousVertice, intersectionPoint);    //         (Qc-Qp)
            vector4Scale(intersectionPoint, intersectionFactor, intersectionPoint);        //        f(Qc-Qp))
            vector4Add(intersectionPoint, *previousVertice, intersectionPoint);            //Qp    + f(Qc-Qp))

            // Insert
            vector4Copy(intersectionPoint, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        if (currentDot > 0)
        {
            //Insert
            vector4Copy(*currentVertice, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        previousDot = currentDot;

        //Move forward
        previousVertice = currentVertice;
        currentVertice++;
    }

    //Copy the output(in_vertices) into the source (face)
    copyVertices(in_vertices, face, in_numVertices);
    in_numVertices = 0;
}



void ClipPolygonForAxis(polygon_t* face, int AXIS)
{
    int i;

    vec4_t* currentVertice;
    vec4_t* previousVertice;


    unsigned char in_numVertices = 0;
    vec4_t in_vertices[MAX_VERTICES_PRE_POLYGON];

    char previousDot;
    char currentDot;

    float intersectionFactor;
    vec4_t intersectionPoint;

    //Clip against first plane
    previousVertice = &face->hs_vertices[face->hs_numVertices - 1];
    previousDot = ((*previousVertice)[AXIS] <= (*previousVertice)[W]) ? 1 : -1;
    currentVertice = &face->hs_vertices[0];
    while (currentVertice != &face->hs_vertices[face->hs_numVertices])
    {
        currentDot = ((*currentVertice)[AXIS] <= (*currentVertice)[W]) ? 1 : -1;

        if (previousDot * currentDot < 0)
        {
            //Need to clip against plan w=0

            intersectionFactor =
                ((*previousVertice)[W] - (*previousVertice)[AXIS]) /
                (((*previousVertice)[W] - (*previousVertice)[AXIS]) - ((*currentVertice)[W] - (*currentVertice)[AXIS]));

            vector4Copy(*currentVertice, intersectionPoint);
            vector4Subtract(intersectionPoint, *previousVertice, intersectionPoint);
            vector4Scale(intersectionPoint, intersectionFactor, intersectionPoint);
            vector4Add(intersectionPoint, *previousVertice, intersectionPoint);

            // Insert
            vector4Copy(intersectionPoint, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        if (currentDot > 0)
        {
            //Insert
            vector4Copy(*currentVertice, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        previousDot = currentDot;

        //Move forward
        previousVertice = currentVertice;
        currentVertice++;
    }

    //Copy the output(in_vertices) into the source (face)
    copyVertices(in_vertices, face, in_numVertices);
    in_numVertices = 0;





    //Clip against opposite plane,  w = -x
    previousVertice = &face->hs_vertices[face->hs_numVertices - 1];
    previousDot = (-(*previousVertice)[AXIS] <= (*previousVertice)[W]) ? 1 : -1;
    currentVertice = &face->hs_vertices[0];
    while (currentVertice != &face->hs_vertices[face->hs_numVertices])
    {
        currentDot = (-(*currentVertice)[AXIS] <= (*currentVertice)[W]) ? 1 : -1;

        if (previousDot * currentDot < 0)
        {
            //Need to clip against plan w=0

            intersectionFactor =
                ((*previousVertice)[W] + (*previousVertice)[AXIS]) /
                (((*previousVertice)[W] + (*previousVertice)[AXIS]) - ((*currentVertice)[W] + (*currentVertice)[AXIS]));

            vector4Copy(*currentVertice, intersectionPoint);
            vector4Subtract(intersectionPoint, *previousVertice, intersectionPoint);
            vector4Scale(intersectionPoint, intersectionFactor, intersectionPoint);
            vector4Add(intersectionPoint, *previousVertice, intersectionPoint);

            // Insert
            vector4Copy(intersectionPoint, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        if (currentDot > 0)
        {
            //Insert
            vector4Copy(*currentVertice, in_vertices[in_numVertices]);
            in_numVertices++;
        }

        previousDot = currentDot;

        //Move forward
        previousVertice = currentVertice;
        currentVertice++;
    }

    //Copy the output(in_vertices) into the source (face)
    copyVertices(in_vertices, face, in_numVertices);
    in_numVertices = 0;
}

void ClipPolygon(polygon_t* face)
{
    ClipPolygonOnWAxis(face);       // w=W_CLIPPING_PLANE
    ClipPolygonForAxis(face, X);     // w=x   ,  w=-x
    ClipPolygonForAxis(face, Y);     // w=y   ,  w=-y
    ClipPolygonForAxis(face, Z);     // w=z   ,  w=-z
}

