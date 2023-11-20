#include "utils.h"

char* str_concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

char* alias_img(void) { 
    CURL *curl;
  	FILE *fp;
  	CURLcode res;
  	char *url = ALIAS_IMG_URL;
  	char outfilename[FILENAME_MAX] = ALIAS_IMG_NAME;
  	curl = curl_easy_init();                                                                                                                                                                                                                                                           
  	if (curl) {   
      fp = fopen(outfilename,"wb");
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      fclose(fp);
  	}  
  	char *open_cmd = str_concat("open ", ALIAS_IMG_NAME);
	 return open_cmd;
}

int respond(int s, char *msg_buf) {
    //write the contents of msg_buf into socket s and return status
}

int recieve(int s, char *msg) {
    //reset the msg buffer
    int read_status = //read contents of socket s into msg
    if (read_status) {
      perror("log:");
      exit(1);
    }
    return 0;
}