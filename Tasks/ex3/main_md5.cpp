
#include <stdio.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <sys/dir.h>
#include <sys/stat.h>

using namespace std;
 
// Constants are the integer part of the sines of integers (in radians) * 2^32.
const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
 
// r specifies the per-round shift amounts
const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
 
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
 
void to_bytes(uint32_t val, uint8_t *bytes)
{
   bytes[0] = (uint8_t) val;
   bytes[1] = (uint8_t) (val >> 8);
   bytes[2] = (uint8_t) (val >> 16);
   bytes[3] = (uint8_t) (val >> 24);
}
 
uint32_t to_int32(const uint8_t *bytes)
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
}
 
void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest) {
 
    // These vars will contain the hash
    uint32_t h0, h1, h2, h3;
 
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    size_t new_len, offset;
    uint32_t w[16];
    uint32_t a, b, c, d, i, f, g, temp;
 
    // Initialize variables - simple count in nibbles:
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 
    //Pre-processing:
    //append "1" bit to message    
    //append "0" bits until message length in bits ≡ 448 (mod 512)
    //append length mod (2^64) to message
 
    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)
        ;
 
    msg = (uint8_t*)malloc(new_len + 8);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 0x80; // append the "1" bit; most significant bit is "first"
    for (offset = initial_len + 1; offset < new_len; offset++)
        msg[offset] = 0; // append "0" bits
 
    // append the len in bits at the end of the buffer.
    to_bytes(initial_len*8, msg + new_len);
    // initial_len>>29 == initial_len*8>>32, but avoids overflow.
    to_bytes(initial_len>>29, msg + new_len + 4);
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        for (i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i*4);
 
        // Initialize hash value for this chunk:
        a = h0;
        b = h1;
        c = h2;
        d = h3;
 
        // Main loop:
        for(i = 0; i<64; i++) {
 
            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }
 
            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
 
        }
 
        // Add this chunk's hash to result so far:
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
 
    // cleanup
    free(msg);
 
    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
    to_bytes(h0, digest);
    to_bytes(h1, digest + 4);
    to_bytes(h2, digest + 8);
    to_bytes(h3, digest + 12);
}
 


void WriteArray(int*arr,int arrsize)
{
	for(int i =0;i<arrsize;i++)
	{
		printf("%d",arr[i]);
	}

}


void WriteArray(char*arr,int arrsize)
{
	for(int i =0;i<arrsize;i++)
	{
		printf("%c",arr[i]);
	}
	
}


char* gethash(char*msg,size_t len)
{
	 
    int i;
    uint8_t result[16];
 

   // len = strlen(msg);
	
    // benchmark
    for (i = 0; i < 1; i++) {
        md5((uint8_t*)msg, len, result);
    }
 
    // display result
	static char* outbuf=new char[32];
    for (i = 0; i < 32; i++)
	{	
		int j =2*i;
		sprintf(&outbuf[j],"%x", result[i]);
		
	
	}
 
    return outbuf;
}





void GenerateSeries(char *characters,unsigned int charsetlen, int len,unsigned long long beginpassindex,unsigned long long stoppassindex)
{
	char *hashstr=new char[32];
charsetlen-=1;
	
	if(len <= 0)
	{
		len=charsetlen;
	}
	if(beginpassindex < 0)
	{
		beginpassindex=0;
	}
	if(stoppassindex <= 0 || stoppassindex < beginpassindex)
	{

		stoppassindex =  pow(charsetlen,len);
	}
	char result[len]={0};
	unsigned long long indexarray[len]={0};


	unsigned long long curindex=beginpassindex;

	for(int i=len-1;i >= 0;i--)
	{

		unsigned long long in=curindex%charsetlen;
		indexarray[i] = in;
		curindex = curindex/charsetlen;

	}
printf("B: %llu\n",beginpassindex);
printf("S: %llu\n",stoppassindex);
	/*int l=1;
   for(int i = 0;i < len;i++)
   {
	char c=characters[indexarray[i]];
	result[i]=c;
   }

   WriteArray(result,len);
	*/
	for(unsigned long long i = beginpassindex;i < stoppassindex;i++)
      {

        for(int j = len-1;j > 0;j--)
		{
			//printf("%d against %d\n",indexarray[j],charsetlen);
			if(indexarray[j] >= charsetlen )
			{
				
				indexarray[j]=0;
				indexarray[j-1]++;
			}
		}
		   for(int k = 0;k < len;k++)
			{
				char c=characters[indexarray[k]];
				result[k]=c;
			}
		 //WriteArray(indexarray,len);
		
		/*printf("Child with interval %llu - %llu computed: \t",beginpassindex,stoppassindex);
		 WriteArray(result,len);
		 hashstr=gethash(result,len);
		 printf("\t");
		 WriteArray(hashstr,32);
		 puts("");*/
		indexarray[len -1]++;
		
		
		
      

		


      }
delete hashstr;
}

int main( int num_arg, char **args )
{

	char charset[] = "abcd";
	int passlength ;
	unsigned int processcount;
	//int beginpassindex = 0;
	//int stoppassindex = 0;
	int pid = -1;
		unsigned int i;
		
	FILE *outfile;
	mkdir("Results",ACCESSPERMS);
	for(passlength=1;passlength < 3;passlength++)
	{	char filename[64];
		sprintf(filename,"Results/%d_longs.csv",passlength);
		outfile=fopen(filename,"w");
		
		fprintf(outfile,"Processes\tSeconds\n");
		fclose(outfile);
		unsigned long long totalcount = (unsigned long long)(pow(sizeof(charset)-1,passlength));
		for(processcount=1;processcount < 3 && processcount<=totalcount;processcount++)
		{
		//GenerateSeries(charset,sizeof(charset),passlength,beginpassindex,stoppassindex);
			
			printf("Totalcount %llu\nProcesscount %d\n",totalcount,processcount);
			unsigned long long singlep=totalcount/processcount;
			unsigned long long remainder = totalcount%processcount;
/*			
			for(int i =0;i < processcount;i++)
			{
				int bindex = (i)*singlep;
				int eindex = (i+1)*singlep -1;
				if(i == processcount-1){eindex+=remainder;}
				printf("%d - %d\n",bindex,eindex);
				
			}
		
*/		
			
			for ( i = 0; i < processcount; i++ )
			{
				pid = fork();
				if ( pid == 0 ) break;
			}
		
			//printf( "Ja jsem proces PID %d\n", getpid() );
			//getchar();
		
			if ( pid )
			{ // kod rodice
			 struct timeval start, end;
				int  status=-1;
				printf( "I am parent with PID %d\n", getpid() );
				int wpid = -1;
				gettimeofday(&start, NULL);
				double cpu_time_used;
				while ( ( wpid = wait( &status ) ) > 0 )
				{
					printf( "Child PID %d ended with  CPU time status %d\n", wpid,WEXITSTATUS(status) );
	
				}
				gettimeofday(&end, NULL);
				cpu_time_used = (end.tv_sec - start.tv_sec) * 1000.0;
		
				//getchar();
	
				outfile=fopen(filename,"a");
				
				printf("%llu passwords generated in %f seconds\n",totalcount,cpu_time_used);
				
				fprintf(outfile,"%d\t\t%f\n",processcount,cpu_time_used);
				fclose(outfile);
				printf( "Parent finish...\n-------------------------------------------------------\n" );
		
			}
			else
			{
	
				
				
			//Swap(sharmem,i,i+1);
				
				unsigned long long bindex = (i)*singlep;
				unsigned long long eindex = (i+1)*singlep -1;
				if(i == processcount-1){eindex+=remainder;}
				printf("CHild %d manages %llu - %llu\n",i,bindex,eindex);
				
				GenerateSeries(charset,sizeof(charset),passlength,bindex,eindex);
				
				
				//printf("CHild %d worked for %f seconds\n",i,cpu_time_used);
				
	
				exit(eindex-bindex+1);
			}
		
		
		
		
			printf("\n");
		}
	}
return 0;
}
