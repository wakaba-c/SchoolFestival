xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 84;
 -0.12384;-0.00088;-2.66232;,
 -0.12384;0.13508;-2.66232;,
 0.12384;0.13508;-2.66232;,
 0.12384;-0.00088;-2.66232;,
 -0.05305;4.07536;-2.66232;,
 -0.05305;4.07536;-2.52637;,
 0.05305;4.07536;-2.52637;,
 0.05305;4.07536;-2.66232;,
 -0.12384;0.13508;-2.52637;,
 -0.12384;-0.00088;-2.52637;,
 0.12384;-0.00088;-2.52637;,
 0.12384;0.13508;-2.52637;,
 -0.12384;-0.00088;-2.66232;,
 0.12384;-0.00088;-2.66232;,
 -0.61922;-0.00088;-2.66232;,
 -0.61922;-0.00088;-2.52637;,
 -0.61922;0.13508;-2.52637;,
 -0.61922;0.13508;-2.66232;,
 0.61922;-0.00088;-2.52637;,
 0.61922;-0.00088;-2.66232;,
 0.61922;0.13508;-2.66232;,
 0.61922;0.13508;-2.52637;,
 -0.61922;0.13508;-2.52637;,
 -0.61922;-0.00088;-2.52637;,
 -0.61922;-0.00088;-2.66232;,
 0.61922;-0.00088;-2.52637;,
 0.61922;-0.00088;-2.66232;,
 0.61922;0.13508;-2.52637;,
 -0.05305;0.23972;-2.52637;,
 -0.05305;0.23972;-2.66232;,
 0.05305;0.23972;-2.52637;,
 0.05305;0.23972;-2.66232;,
 -0.05305;1.64841;-2.66232;,
 -0.05305;1.64841;-2.52637;,
 -0.05305;1.76935;-2.52637;,
 -0.05305;1.76935;-2.66232;,
 -0.05305;1.64841;-0.04549;,
 0.05305;1.64841;-0.04549;,
 0.05305;1.76935;-0.04549;,
 -0.05305;1.76935;-0.04549;,
 0.05305;1.64841;-2.52637;,
 0.05305;1.64841;-2.66232;,
 0.05305;1.76935;-2.66232;,
 0.05305;1.76935;-2.52637;,
 -0.12384;-0.00088;2.57135;,
 0.12384;-0.00088;2.57135;,
 0.12384;0.13508;2.57135;,
 -0.12384;0.13508;2.57135;,
 -0.05305;4.07536;2.57135;,
 0.05305;4.07536;2.57135;,
 0.05305;4.07536;2.43539;,
 -0.05305;4.07536;2.43539;,
 -0.12384;0.13508;2.43539;,
 0.12384;0.13508;2.43539;,
 0.12384;-0.00088;2.43539;,
 -0.12384;-0.00088;2.43539;,
 0.12384;-0.00088;2.57135;,
 -0.12384;-0.00088;2.57135;,
 -0.61922;-0.00088;2.57135;,
 -0.61922;0.13508;2.57135;,
 -0.61922;0.13508;2.43539;,
 -0.61922;-0.00088;2.43539;,
 0.61922;-0.00088;2.43539;,
 0.61922;0.13508;2.43539;,
 0.61922;0.13508;2.57135;,
 0.61922;-0.00088;2.57135;,
 -0.61922;0.13508;2.43539;,
 -0.61922;-0.00088;2.43539;,
 -0.61922;-0.00088;2.57135;,
 0.61922;-0.00088;2.43539;,
 0.61922;-0.00088;2.57135;,
 0.61922;0.13508;2.43539;,
 -0.05305;0.23972;2.57135;,
 -0.05305;0.23972;2.43539;,
 0.05305;0.23972;2.43539;,
 0.05305;0.23972;2.57135;,
 -0.05305;1.64841;2.57135;,
 -0.05305;1.76935;2.57135;,
 -0.05305;1.76935;2.43539;,
 -0.05305;1.64841;2.43539;,
 0.05305;1.64841;2.43539;,
 0.05305;1.76935;2.43539;,
 0.05305;1.76935;2.57135;,
 0.05305;1.64841;2.57135;;
 
 68;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;1,0,14,17;,
 4;8,1,17,22;,
 4;9,8,22,23;,
 4;12,9,23,24;,
 4;25,10,13,26;,
 4;27,11,10,25;,
 4;20,2,11,27;,
 4;19,3,2,20;,
 4;1,8,28,29;,
 4;8,11,30,28;,
 4;11,2,31,30;,
 4;2,1,29,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;41,32,35,42;,
 4;35,34,5,4;,
 4;34,43,6,5;,
 4;43,42,7,6;,
 4;42,35,4,7;,
 4;29,28,33,32;,
 4;28,30,40,33;,
 4;30,31,41,40;,
 4;31,29,32,41;,
 4;33,40,37,36;,
 4;40,43,38,37;,
 4;43,34,39,38;,
 4;34,33,36,39;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;55,54,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;47,59,58,44;,
 4;52,66,59,47;,
 4;55,67,66,52;,
 4;57,68,67,55;,
 4;69,70,56,54;,
 4;71,69,54,53;,
 4;64,71,53,46;,
 4;65,64,46,45;,
 4;47,72,73,52;,
 4;52,73,74,53;,
 4;53,74,75,46;,
 4;46,75,72,47;,
 4;76,77,78,79;,
 4;36,39,38,37;,
 4;80,81,82,83;,
 4;83,82,77,76;,
 4;77,48,51,78;,
 4;78,51,50,81;,
 4;81,50,49,82;,
 4;82,49,48,77;,
 4;72,76,79,73;,
 4;73,79,80,74;,
 4;74,80,83,75;,
 4;75,83,76,72;,
 4;79,36,37,80;,
 4;80,37,38,81;,
 4;81,38,39,78;,
 4;78,39,36,79;;
 
 MeshMaterialList {
  1;
  68;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.956105;0.293024;0.000000;,
  0.956105;0.293024;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.468869;0.883268;0.000000;,
  0.468869;0.883268;0.000000;;
  68;
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;3,3,3,3;,
  4;7,7,7,7;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;8,8,6,6;,
  4;3,3,3,3;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;6,9,9,6;,
  4;2,2,2,2;,
  4;8,8,4,4;,
  4;3,3,3,3;,
  4;9,9,5,5;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;4,4,1,1;,
  4;3,3,3,3;,
  4;5,5,0,0;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;6,6,6,6;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;8,6,6,8;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;2,2,2,2;,
  4;6,6,9,9;,
  4;3,3,3,3;,
  4;8,4,4,8;,
  4;2,2,2,2;,
  4;9,5,5,9;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;4,1,1,4;,
  4;2,2,2,2;,
  4;5,0,0,5;,
  4;3,3,3,3;,
  4;7,7,7,7;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;1,1,1,1;;
 }
 MeshTextureCoords {
  84;
  0.525000;0.000000;,
  0.525000;0.250000;,
  0.475000;0.250000;,
  0.475000;0.000000;,
  0.525000;0.250000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.250000;,
  0.525000;0.500000;,
  0.525000;0.750000;,
  0.475000;0.750000;,
  0.475000;0.500000;,
  0.525000;1.000000;,
  0.475000;1.000000;,
  0.625000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.625000;0.250000;,
  0.125000;0.000000;,
  0.375000;0.000000;,
  0.375000;0.250000;,
  0.125000;0.250000;,
  0.625000;0.500000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.375000;0.500000;,
  0.525000;0.500000;,
  0.525000;0.250000;,
  0.475000;0.500000;,
  0.475000;0.250000;,
  0.525000;0.250000;,
  0.525000;0.500000;,
  0.525000;0.500000;,
  0.525000;0.250000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.500000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.250000;,
  0.475000;0.250000;,
  0.475000;0.500000;,
  0.525000;0.000000;,
  0.475000;0.000000;,
  0.475000;0.250000;,
  0.525000;0.250000;,
  0.525000;0.250000;,
  0.475000;0.250000;,
  0.475000;0.500000;,
  0.525000;0.500000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.750000;,
  0.525000;0.750000;,
  0.475000;1.000000;,
  0.525000;1.000000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.875000;0.250000;,
  0.875000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.250000;,
  0.375000;0.250000;,
  0.375000;0.000000;,
  0.625000;0.500000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.375000;0.500000;,
  0.525000;0.250000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.250000;,
  0.525000;0.250000;,
  0.525000;0.250000;,
  0.525000;0.500000;,
  0.525000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.500000;,
  0.475000;0.250000;,
  0.475000;0.250000;;
 }
}