#include "AkgLoadObj.h"

#include <string.h>

void AkgLoadObj::Load(const char* fileName, 
    std::vector<float>& vert, std::vector<unsigned int>& index)
{
  FILE* fp = fopen(fileName, "r");
  char buff[512];

  //‚Æ‚è‚ ‚¦‚¸’¸“_‚ğ’Šo‚µ‚ÄC‡”Ô‚Évector‚É“Ë‚Á‚Ş
  while (fgets(buff, 128, fp) != NULL) {
    if (strncmp(buff, "vn", 2) == 0) {
      continue;
    }
    else if (strncmp(buff, "v ", 2) == 0) {
      float v[3];
      sscanf(buff, "v %f %f %f", &v[0], &v[1], &v[2]);
      vert.push_back(v[0]);
      vert.push_back(v[1]);
      vert.push_back(v[2]);
    }
    else if (strncmp(buff, "f ", 2) == 0) {
      int indexes_v[3], indexes_vt[3];
      sscanf(buff, "f %u//%u %u//%u %u//%u", 
        &indexes_v[0], &indexes_vt[0],&indexes_v[1], 
        &indexes_vt[1], &indexes_v[2], &indexes_vt[2]); 

      index.push_back(indexes_v[0]-1);
      index.push_back(indexes_v[1]-1);
      index.push_back(indexes_v[2]-1);
    }
  }

  fclose(fp);  
}