






float Cube::intersectCube(SbVec3f rayDirection,SbVec3f rayStart){
 float t1,t2,tnear = -1000.0f,tfar = 1000.0f,temp,tCube;
 SbVec3f b1 = getNearPoint();
 SbVec3f b2 = getFarPoint();
 bool intersectFlag = true;
 for(int i =0 ;i < 3; i++){
  if(rayDirection[i] == 0){
   if(rayStart[i] < b1[i] || rayStart[i] > b2[i])
    intersectFlag = false;
  }
  else{
   t1 = (b1[i] - rayStart[i])/rayDirection[i];
   t2 = (b2[i] - rayStart[i])/rayDirection[i];
  if(t1 > t2){
   temp = t1;
   t1 = t2;
   t2 = temp;
  }
  if(t1 > tnear)
   tnear = t1;
  if(t2 < tfar)
   tfar = t2;
  if(tnear > tfar)
   intersectFlag = false;
  if(tfar < 0)
   intersectFlag = false;
  }
 }
 if(intersectFlag == false)
  tCube = -1;
 else
  tCube = tnear;
 
 return tCube;
}